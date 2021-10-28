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
        if (languagePair->getCurrentSourceLanguage() == lang1 && languagePair->getCurrentTargetLanguage() == lang2)
        {
            return true;
        }

        if (languagePair->getCurrentSourceLanguage() == lang2 && languagePair->getCurrentTargetLanguage() == lang1)
        {
            return true;
        }

        return false;
    }

    void prepareLanguagePair(const QString &lang1, const QString &lang2)
    {
        if (languagePair->getCurrentSourceLanguage() == lang2 && languagePair->getCurrentTargetLanguage() == lang1)
        {
            languagePair->swap();
        }
    }

    const LanguagePair *getLanguagePair()  const     { return languagePair; }

    void setLanguagePair(LanguagePair *languagePair)
    {
        languagePair->setParent(this);
        this->languagePair = languagePair;
    }

    const QString &getSourceLanguageName() const { return languagePair->getSourceLanguageName();   }
    const QString &getTargetLanguageName() const { return languagePair->getTargetLanguageName();   }
    bool isTwoWayTransliteration() const         { return languagePair->isTwoWayTransliteration(); }

    const QString &getCurrentSourceLanguage() const { return languagePair->getCurrentSourceLanguage(); }
    const QString &getCurrentTargetLanguage() const { return languagePair->getCurrentTargetLanguage(); }

protected:
    virtual void prepareWord(QString &word)  const
    { Q_UNUSED(word); }

    virtual void postworkWord(QString &word, const SyllableList &syllableList = {}) const
    { Q_UNUSED(word); Q_UNUSED(syllableList); }

    virtual void transliterateWord(Word &word) const
    {
        // prepare the word if required
        prepareWord(word.text);

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
            newText += syllable->getTarget();
        }

        word.replaceText(newText);

        // modify the word if required
        postworkWord(word.text, transliterationSyllables);
    }

    virtual void transliterateException(Word &word) const
    {
        if (auto exception = exceptions.findSyllable(word.text))
        {
            word.text = exception->getTarget();
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
    SyllableList getCorrectVariant(const QString &text, const SyllableList2D &variants) const
    {
        if (variants.isEmpty()) { return {}; }

        auto correctVariants = getCorrectVariants(text, variants);

        for (auto v : correctVariants)
        {
            replaceWordStartSyllable(v.first());
            replaceWordEndSyllable(v.last());
        }

        auto shortestVariant = getBestVariant(correctVariants);

        return shortestVariant;
    }

    SyllableList2D getCorrectVariants(QString text, const SyllableList2D &variants) const
    {
        SyllableList2D correctVariants;

        for (auto v : variants)
        {
            QString s;
            for (auto syllable : v) { s += syllable->getSource(); }
            if (text.remove('\'') == s) { correctVariants.append(v); }
        }

        return correctVariants;
    }

    SyllableList getBestVariant(const SyllableList2D &variants) const
    {
        SyllableList2D highPriorityVariants;

        for (const auto &v : variants)
        {
            if (v.first()->getType() == SyllableType::WordStart ||
                v.last()->getType()  == SyllableType::WordEnd)
            {
                highPriorityVariants.append(v);
            }
        }

        if (!highPriorityVariants.isEmpty())
        {
            return getShortestVariant(highPriorityVariants);
        }
        else
        {
            return getShortestVariant(variants);
        }
    }

    SyllableList getShortestVariant(const SyllableList2D &variants) const
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

    void replaceWordStartSyllable(const SyllablePair *s) const
    {
        if (auto wordStartSyllable = syllables.getWordStartSyllable(*s))
        {
            s = wordStartSyllable;
        }
    }

    void replaceWordEndSyllable(const SyllablePair *s) const
    {
        if (auto wordEndSyllable = syllables.getWordEndSyllable(*s))
        {
            s = wordEndSyllable;
        }
    }

private:
    LanguagePair *languagePair;

protected:
    const SyllablePairSet &syllables  { languagePair->getSyllables()  };
    const SyllablePairSet &exceptions { languagePair->getExceptions() };
    WordCutter wordCutter { languagePair };
};

#endif // TRANSLITERATOR_H
