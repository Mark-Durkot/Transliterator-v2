#ifndef SYLLABLEPAIRSET_H
#define SYLLABLEPAIRSET_H

#include "SyllablePair.h"

#include <QList>

class SyllablePairSet : public QList<SyllablePair>
{
public:
    void insert(const SyllablePair &p)
    {
        if (!this->contains(p))
        {
            this->append(p);
        }
    }

    bool containsString(const QString &s) const
    {
        for (const auto &syllable : *this)
        {
            if (syllable.getFirst() == s || syllable.getSecond() == s)
            {
                return true;
            }
        }

        return false;
    }

    void sortByDescendingSyllableLenght()
    {
        auto &syllables = *this;
        bool flag = false;
        for (int i = 0; i < syllables.size() - 1; ++i)
        {
            flag = false;
            for (int j = 0; j < syllables.size() - 1 - i; ++j)
            {
                if (syllables[j].isShorterThan(syllables[j + 1]))
                {
                    flag = true;
                    qSwap(syllables[j], syllables[j + 1]);
                }
            }
            if (!flag)
            {
                break;
            }
        }
    }

    void swap()
    {
        for (auto &syllable : *this)
        {
            syllable.swap();
        }

        sortByDescendingSyllableLenght();
    }
};

#endif // SYLLABLEPAIRSET_H
