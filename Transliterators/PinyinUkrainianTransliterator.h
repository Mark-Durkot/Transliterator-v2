#ifndef PINYINUKRAINIANTRANSLITERATOR_H
#define PINYINUKRAINIANTRANSLITERATOR_H

#include "Transliterator.h"

#include <QObject>

class PinyinUkrainianTransliterator : public Transliterator
{
    Q_OBJECT
public:
    PinyinUkrainianTransliterator(LanguagePair *l, QObject *parent=nullptr) : Transliterator(l, parent) {}

private:
    void prepareWord(QString &word) const override
    {
        removeInterSyllableApostrophes(word);
    }

    void postworkWord(QString &word, const SyllableList &syllableList) const override
    {
        addApostrophes(word, syllableList);
    }

    /*
     * removing apostrophes that stand between бмвфп and яює
     */
    void removeInterSyllableApostrophes(QString &word) const
    {
        int i = 1;
        while (i < word.length() - 1)
        {
            if (word[i] == '\'' &&
                ukrLettersBeforeApostrophe.contains(word[i - 1]) &&
                ukrLettersAfterApostrophe.contains(word[i + 1]))
            {
                word.remove(i, 1);
            }
            else
            {
                ++i;
            }
        }
    }

    void addApostrophes(QString &word, const SyllableList &syllableList) const
    {
        int insertPosition = 0;
        for (int i = 0; i < syllableList.count() - 1; ++i)
        {
            insertPosition += syllableList[i]->getTarget().length();

            if (shouldHaveApostropheBetween(syllableList[i], syllableList[i + 1]))
            {
                word.insert(insertPosition, '\'');
            }
        }

        addIntersyllableApostrophes(word);
    }

    bool shouldHaveApostropheBetween(const SyllablePair *s1, const SyllablePair *s2) const
    {
        return shouldHavePinyinApostrophe(s1, s2) ||
               shouldHaveUkrainianApostrophe(s1, s2);
    }

    bool shouldHaveUkrainianApostrophe(const SyllablePair *s1, const SyllablePair *s2) const
    {
        if (s1->getSource().endsWith("ng") || s1->getSource().endsWith("ou") || s1->getSource().endsWith("iu"))
        {
            // no apostrophe between бмвфп and йо
            return (s2->getSource().startsWith('y') && !s2->getTarget().startsWith("йо"));
        }

        if (s1->getSource().endsWith("ng") && latinVowels.contains(s2->getSource()[0]))
        {
            return true;
        }

        return false;
    }

    bool shouldHavePinyinApostrophe(const SyllablePair *s1, const SyllablePair *s2) const
    {
        if (latinVowels.contains(s2->getTarget()[0]))
        {
            return true;
        }

        if (s1->getSource().endsWith("н") && cyrilicVowels.contains(s2->getSource()[0]))
        {
            return true;
        }

        return false;
    }

    void addIntersyllableApostrophes(QString &word) const
    {
        for (int i = 0; i < word.length() - 1; ++i)
        {
            if (isLetterBeforeApostrophe(word[i]) && isLetterAfterApostrophe(word[i + 1]))
            {
                word.insert(i + 1, "\'");
            }
        }
    }

    bool isLetterBeforeApostrophe(const QChar &c) const
    {
        return ukrLettersBeforeApostrophe.contains(c);
    }

    bool isLetterAfterApostrophe(const QChar &c) const
    {
        return ukrLettersAfterApostrophe.contains(c);
    }

private:
    const QString ukrLettersBeforeApostrophe = "бпмф";
    const QString ukrLettersAfterApostrophe  = "яєї";

    const QString latinVowels   = "aeiou";
    const QString cyrilicVowels = "аеіоу";
};

#endif // PINYINUKRAINIANTRANSLITERATOR_H
