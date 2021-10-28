#ifndef TRANSLITERATORLIST_H
#define TRANSLITERATORLIST_H

#include "Transliterator.h"
#include "LanguageStructures/LanguagePair.h"

#include <QList>
#include <QObject>


class TransliteratorList : public QObject, public QList<Transliterator*>
{
    Q_OBJECT
public:
    static TransliteratorList createTransliteratorList()
    {
        return {};
    }

    Transliterator *getTransliterator(const QString &language1, const QString &language2)
    {
        for (auto *transliterator : *this)
        {
            if (transliterator->isTransliteratorForLanguages(language1, language2))
            {
                transliterator->prepareLanguagePair(language1, language2);
                return transliterator;
            }
        }

        return nullptr;
    }

    Transliterator *getDefaultForSourceLanguage(const QString &sourceLanguage)
    {
        for (auto transliterator : *this)
        {
            if (transliterator->getCurrentSourceLanguage() == sourceLanguage)
            {
                return transliterator;
            }

            if (transliterator->getCurrentTargetLanguage() == sourceLanguage && transliterator->isTwoWayTransliteration())
            {
                transliterator->prepareLanguagePair(sourceLanguage, transliterator->getCurrentSourceLanguage());
                return transliterator;
            }
        }

        return nullptr;
    }

    void add(Transliterator *l)
    {
        if (l == nullptr) { return; }

        l->setParent(this);
        this->append(l);
    }

    QStringList getSourceComboBoxLanguages() const
    {
        QStringList sourceLanguages;
        for (const auto transliterator : *this)
        {
            auto source = transliterator->getSourceLanguageName();
            if (!sourceLanguages.contains(source))
            {
                sourceLanguages << source;
            }
        }

        return sourceLanguages;
    }

    QStringList getTargetComboBoxLanguages(const QString &source)
    {
        QStringList targetLanguages;
        for (const auto transliterator : *this)
        {
            if (transliterator->getSourceLanguageName() == source)
            {
                QString target = transliterator->getTargetLanguageName();
                if (!targetLanguages.contains(target))
                {
                    targetLanguages << target;
                }
            }
        }

        return targetLanguages;
    }
};

#endif // TRANSLITERATORLIST_H
