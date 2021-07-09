#ifndef TRANSLITERATOR_H
#define TRANSLITERATOR_H

#include "LanguageStructures/LanguagePair.h"
#include "Parsers/Words.h"

#include <QString>
#include <QObject>

class Transliterator : public QObject
{
    Q_OBJECT
public:
    Transliterator(LanguagePair *l, QObject *parent=nullptr)
        : QObject(parent),
          firstLanguageName(l->getFirstLanguageName()),
          secondLanguageName(l->getSecondLanguageName()),
          languagePair(l)
    {
    }


    virtual WordList transliterate(QString text)
    {
        auto words = WordCutter::splitTextByWords(text, *languagePair);

        for (auto *word : words.getAlphabeticWordsOnly())
        {
            transliterateWord(*word);
        }

        return words;
    }

    bool isTransliteratorForLanguages(const QString &lang1, const QString &lang2)
    {
        if (firstLanguageName == lang1 && secondLanguageName == lang2) { return true; }
        if (firstLanguageName == lang2 && secondLanguageName == lang1) { return true; }

        return false;
    }

    void prepareLanguagePair(const QString &lang1, const QString &lang2)
    {
        if (languagePair->getFirstLanguageName() == lang2 && languagePair->getSecondLanguageName() == lang1)
        {
            languagePair->swap();
        }
    }

protected:
    /* this method needs to be overwritten */
    virtual void transliterateWord(Word &word)
    {
        Q_UNUSED(word);
    }

private:
    QString  firstLanguageName;
    QString secondLanguageName;
    LanguagePair *languagePair;
};

#endif // TRANSLITERATOR_H
