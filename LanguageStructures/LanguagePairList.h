#ifndef LANGUAGEPAIRLIST_H
#define LANGUAGEPAIRLIST_H

#include "LanguagePair.h"

#include <QList>
#include <QObject>

class LanguagePairList : public  QObject, public QList<LanguagePair *>
{
    Q_OBJECT
public:
    void add(LanguagePair *l)
    {
        if (l == nullptr) { return; }

        l->setParent(this);

        this->append(l);
    }

    QStringList getSourceLanguages() const
    {
        QStringList sourceLanguages;

        for (const auto language : *this)
        {
            auto source = language->getSourceLanguageName();
            auto target = language->getTargetLanguageName();

            if (!sourceLanguages.contains(source))
                sourceLanguages << source;

            if (language->isTwoWayTransliteration())
            {
                if (!sourceLanguages.contains(target))
                    sourceLanguages << target;
            }
        }

        return sourceLanguages;
    }

    QStringList getAvailableTargetLanguages(const QString &currentSource)
    {
        QStringList targetLanguages;

        for (const auto language : *this)
        {
            auto source = language->getSourceLanguageName();
            auto target = language->getTargetLanguageName();

            if (source == currentSource)
            {
                if (!targetLanguages.contains(source))
                    targetLanguages  << target;
            }

            if (target == currentSource && language->isTwoWayTransliteration())
            {
                if (!targetLanguages.contains(source))
                    targetLanguages << source;
            }
        }

        return targetLanguages;
    }

    QStringList getAllTargetLanguages() const
    {
        QStringList targetLanguages;

        for (const auto language : *this)
        {
            auto source = language->getSourceLanguageName();
            auto target = language->getTargetLanguageName();

            if (!targetLanguages.contains(target))
                targetLanguages << target;

            if (language->isTwoWayTransliteration())
            {
                if (!targetLanguages.contains(source))
                    targetLanguages << source;
            }
        }

        return targetLanguages;
    }

    bool autoUpdateLanguage(LanguagePair *language)
    {
        if (!language) { return false; }

        for (auto &l : *this)
        {
            if (l->getLanguagePairId() == language->getLanguagePairId())
            {
                l = language;
                return true;
            }
        }

        return false;
    }

    void clearData()
    {
        for (auto l : *this)
        {
            l->deleteLater();
        }

        this->clear();
    }
};

#endif // LANGUAGEPAIRLIST_H
