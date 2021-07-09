#ifndef TRANSLITERATORLIST_H
#define TRANSLITERATORLIST_H

#include "Transliterator.h"
#include "LanguageStructures/LanguagePair.h"

#include <QList>


class TransliteratorList : public QList<Transliterator*>
{
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

    void add(Transliterator *l) { this->append(l); }
};

#endif // TRANSLITERATORLIST_H
