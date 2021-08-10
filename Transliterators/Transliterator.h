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
          languagePair(l)
    {
    }

    virtual void prepare(QString &word)  const { Q_UNUSED(word); }
    virtual void postwork(QString &word) const { Q_UNUSED(word); }

    virtual WordList transliterate(const QString &text) const
    {
        auto words = wordCutter.splitTextByWords(text);

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
        if (languagePair->getFirstLanguageName() == lang1 && languagePair->getSecondLanguageName() == lang2) { return true; }
        if (languagePair->getFirstLanguageName() == lang2 && languagePair->getSecondLanguageName() == lang1) { return true; }

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
    virtual void transliterateWord(Word &word) const
    {
        prepare(word.text);

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
            newText += syllable->getSecond();
        }

        word.replaceText(newText);

        postwork(word.text);
    }

    virtual void transliterateException(Word &word) const
    {
        if (auto exception = languagePair->getExceptions().findSyllable(word.text))
        {
            word.text = exception->getSecond();
        }
    }

    virtual void stringSearch(const QString &str, SyllableTreeNode *node) const
    {
        for (int i = 0; i < str.length(); ++i)
        {
            if (str[i] == '\'')
            {
                stringSearch(str.right(str.length() - i - 1), node);
                break;
            }

            if (auto syllable = syllables.findSyllable(str.left(i + 1)))
            {
                auto childNode = node->addChild(syllable);
                stringSearch(str.right(str.length() - i - 1), childNode);
            }
        }
    }

private:
    SyllablesList &&getCorrectVariant(const QString &text, const SyllablesList2D &&variants) const
    {
        if (variants.isEmpty()) { return std::move(SyllablesList()); }

        auto correctVariants = getCorrectVariants(text, std::move(variants));
        auto shortestVariant = getShortestVariant(std::move(correctVariants));

        return std::move(shortestVariant);
    }

    SyllablesList2D &&getCorrectVariants(QString text, const SyllablesList2D &&variants) const
    {
        SyllablesList2D correctVariants;

        for (auto v : variants)
        {
            QString s;
            for (auto syllable : v) { s += syllable->getFirst(); }
            if (text.remove('\'') == s) { correctVariants.append(v); }
        }

        return std::move(correctVariants);
    }

    SyllablesList getShortestVariant(const SyllablesList2D &&variants) const
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
    LanguagePair *languagePair;

protected:
    const SyllablePairSet &syllables  { languagePair->getSyllables()  };
    const SyllablePairSet &exceptions { languagePair->getExceptions() };
    WordCutter wordCutter { languagePair };
};

#endif // TRANSLITERATOR_H
