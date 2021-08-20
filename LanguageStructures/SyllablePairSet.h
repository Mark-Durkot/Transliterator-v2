#ifndef SYLLABLEPAIRSET_H
#define SYLLABLEPAIRSET_H

#include "SyllablePair.h"

#include <QList>
#include <QDebug>

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

    bool containsString(QString s) const
    {
        s = s.toLower();

        for (const auto &syllable : *this)
        {
            if (syllable.getSource().toLower() == s) { return true; }
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

            if (!flag) { return; }
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

    const SyllablePair *findSyllable(QString s) const
    {
        s = s.toLower();

        for(const auto &syllable : *this)
        {
            if (syllable.getSource().toLower() == s) { return &syllable; }
        }

        return nullptr;
    }

    const SyllablePair *getWordStartSyllable(const SyllablePair &s) const
    {
        for (const auto &syllable : *this)
        {
            if (syllable.getType() == SyllableType::WordStart && syllable.getSource() == s.getSource())
            {
                return &syllable;
            }
        }

        return nullptr;
    }

    const SyllablePair *getWordEndSyllable(const SyllablePair &s) const
    {
        for (const auto &syllable : *this)
        {
            if (syllable.getType() == SyllableType::WordEnd && syllable.getSource() == s.getSource())
            {
                return &syllable;
            }
        }

        return nullptr;
    }
};

#endif // SYLLABLEPAIRSET_H
