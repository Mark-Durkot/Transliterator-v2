#ifndef LANGUAGEPAIRINDEX_H
#define LANGUAGEPAIRINDEX_H

#include "LanguageStructures/LanguagePair.h"
#include "LanguageStructures/SyllablePairSet.h"

class LanguagePairIndex {

public:

    LanguagePairIndex(const LanguagePair *lp) : languagePair(lp)
    {
        indexLanguagePair();
    }

    const SyllablePair *findSyllable(QString s) const
    {
        if (s.length() < 0) { return nullptr; }

        s = s.toLower();

        int startIndex = syllableIndexes.value(s[0].toLower());

        startIndex = startIndex < 0 ? 0 : startIndex;

        const auto &syllables = languagePair->getSyllables();

        for (int i = startIndex; i < syllables.size(); ++i)
        {
            if (syllables[i].getSource() == s) {
                return &syllables[i];
            }
        }

        return nullptr;
    }

private:

    void indexLanguagePair()
    {
        auto syllables  = languagePair->getSyllables();
        auto exceptions = languagePair->getExceptions();

        syllableIndexes  = indexPairSet(syllables);
    }

    QMap<QString, int> indexPairSet(const SyllablePairSet &set)
    {
        QString alphabet = "abcdefghijklmnopqrstuvwxyz";

        QMap<QString, int> map;

        for (const QChar &letter : alphabet)
        {
            int index = findStartIndexOfLetter(letter, set);
            map.insert(letter, index);
        }

        return map;
    }

    int findStartIndexOfLetter(const QChar &letter, const SyllablePairSet &set) {
        for (int i = 0; i < set.size(); ++i) {
            if (set[i].getSource().startsWith(letter)) {
                return i;
            }
        }
        return -1;
    }


private:

    const LanguagePair *languagePair;

    QMap<QString, int> syllableIndexes;
};

#endif // LANGUAGEPAIRINDEX_H
