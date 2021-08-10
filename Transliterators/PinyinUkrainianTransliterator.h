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
    void prepare(QString &word) const override
    {
        removeInterSyllableApostrophes(word);
    }

    void postwork(QString &word) const override
    {
        addApostropheForUkrainian(word);
    }

    void removeInterSyllableApostrophes(QString &word) const
    {
        int i = 1;
        while (i < word.length() - 1)
        {
            if (word[i] == '\'' &&
                lettersBeforeApostrophe.contains(word[i - 1]) &&
                lettersAfterApostrophe.contains(word[i + 1]))
            {
                word.remove(i, 1);
            }
            else
            {
                ++i;
            }
        }
    }

    void addApostropheForUkrainian(QString &word) const
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
        return lettersBeforeApostrophe.contains(c);
    }

    bool isLetterAfterApostrophe(const QChar &c) const
    {
        return lettersAfterApostrophe.contains(c);
    }

private:
    const QString lettersBeforeApostrophe = "бпмф";
    const QString lettersAfterApostrophe  = "яєї";
};

#endif // PINYINUKRAINIANTRANSLITERATOR_H
