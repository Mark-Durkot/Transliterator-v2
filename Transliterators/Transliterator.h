#ifndef TRANSLITERATOR_H
#define TRANSLITERATOR_H

#include "LanguageStructures/LanguagePair.h"
#include "SyllableTree/SyllableTree.h"
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

    virtual WordList transliterate(const QString &text) const
    {
        auto words = WordCutter::splitTextByWords(text, *languagePair);

        for (auto *word : words.getAlphabeticWordsOnly())
        {
            if (word->isNonException())
            {
                transliterateWord(*word);
            }
            else if (word->isException())
            {
                transliterateException(*word);
            }
        }

        return words;
    }

    bool isTransliteratorForLanguages(const QString &lang1, const QString &lang2) const
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
    virtual void transliterateWord(Word &word) const
    {
        SyllableTree syllableTree;

        stringSearch(word.text, syllableTree.getRootNode());

        auto transliterationSyllables = getCorrectVariant(word.text.toLower(), syllableTree.makeList());

        if (transliterationSyllables.isEmpty())
        {
            word.type = WordType::Incorrect;
            return;
        }

        QString newText;

        for (auto syllable : transliterationSyllables)
        {
            newText += syllable.getSecond();
        }

        word.replaceText(newText);
    }

    virtual void transliterateException(Word &word) const
    {
        if (auto exception = languagePair->getExceptions().findSyllable(word.text))
        {
            word.text = exception->getSecond();
        }
    }

    virtual void stringSearch(QString str, SyllableTreeNode *treeNode) const
    {
        for (int i = 0; i < str.length(); ++i)
        {
            if (auto syl = syllables.findSyllable(str.left(i + 1)))
            {
                auto childNode = treeNode->addChild(*syl);
                stringSearch(str.right(str.length() - i - 1), childNode);
            }
        }
    }

private:
    const QList<SyllablePair> getCorrectVariant(const QString &text, const QList<QList<SyllablePair>> &variants) const
    {
        if (variants.isEmpty()) { return {}; }

        auto correctVariants = getCorrectVariants(text, variants);
        auto shortestVariant = getShortestVariant(correctVariants);

        return shortestVariant;
    }

    QList<QList<SyllablePair>> getCorrectVariants(const QString &text, const QList<QList<SyllablePair>> &variants) const
    {
        QList<QList<SyllablePair>> correctVariants;

        for (auto v : variants)
        {
            QString s;
            for (auto syllable : v) { s += syllable.getFirst(); }
            if (text == s) { correctVariants.append(v); }
        }

        return correctVariants;
    }

    QList<SyllablePair> getShortestVariant(const QList<QList<SyllablePair>> &variants) const
    {
        if (variants.isEmpty()) { return {}; }

        int minLengthVariant = variants.at(0).count();
        int minLengthIndex = 0;

        for (int i = 0; i < variants.count(); ++i)
        {
            if (variants[i].count() < minLengthVariant)
            {
                minLengthVariant = variants[i].count();
                minLengthIndex = i;
            }
        }

        return variants[minLengthIndex];
    }

private:
    QString  firstLanguageName;
    QString secondLanguageName;
    LanguagePair *languagePair;

    const SyllablePairSet &syllables  { languagePair->getSyllables()  };
    const SyllablePairSet &exceptions { languagePair->getExceptions() };
};

#endif // TRANSLITERATOR_H
