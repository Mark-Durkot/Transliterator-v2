#ifndef WORDS_H
#define WORDS_H

#include "../LanguageStructures/LanguagePair.h"

#include <QString>
#include <QStringList>
#include <QList>
#include <QColor>

enum class WordType
{
    NonException,
    Exception,
    PotentionException,
    Space,
    Incorrect,
    Punct,
    Undefined
};


class Word
{
public:
    Word(const QString &s="", WordType t=WordType::Undefined)
    {
        word = s;
        type = t;
    }

    bool isSpace()     const { return type == WordType::Space;     }
    bool isException() const { return type == WordType::Exception; }
    bool isIncorrect() const { return type == WordType::Incorrect; }

    QString getHtml() const
    {
        if (this->isSpace())
        {
            if (word == "\t") { return "&emsp;"; }
            if (word == "\n") { return "<br>";   }
        }

        return word;
    }    

    void markWithColor(const QColor &color)
    {
        QString rgb = QString("rgb(%1,%2,%3)").arg(color.red(), color.green(), color.blue());
        word = QString("<span style=\"color: %1\">%2</span>").arg(rgb, word);
    }

    bool isAlphabetic() const
    {
        return (type == WordType::NonException || type == WordType::Exception || type == WordType::PotentionException);
    }

public:
    QString word;
    WordType type;
};


class WordList : public QList<Word>
{
public:
    QList<Word*> getAlphabeticWordsOnly()
    {
        QList<Word*> alphabeticWords;

        for (auto &word : *this)
        {
            if (word.isAlphabetic())
            {
                alphabeticWords.append(&word);
            }
        }

        return alphabeticWords;
    }

    QString toHtmlText()
    {
        QString text;
        for (const auto &word : *this)
        {
            text += word.getHtml();
        }

        return text;
    }

    Word &mergeWords(int startIndex, int endIndex)
    {
        auto &words = *this;
        int replaceCount = endIndex - startIndex;
        for (int i = 0; i < replaceCount - 1; ++i)
        {
            words[startIndex].word += this->at(startIndex + 1).word;
            this->removeAt(i);
        }

        return words[startIndex];
    }

    QString join(const QString &separator="")
    {
        QString result;
        foreach(const auto &word, *this)
        {
            result += word.word + separator;
        }

        // removing last separator
        result.remove(result.length() - 1 - separator.length(), separator.length());

        return result;
    }
};

class WordPainter
{
public:
    void setExceptionColor(const QColor &c) { exceptionColor = c; }
    void setIncorrectColor(const QColor &c) { incorrectColor = c; }

    void paintWords(WordList &words)
    {
        for (auto &word : words)
        {
            if (word.isException()) { word.markWithColor(exceptionColor); }
            if (word.isIncorrect()) { word.markWithColor(incorrectColor); }
        }
    }

private:
    QColor exceptionColor;
    QColor incorrectColor;
};


class WordCutter
{
public:
    static WordList splitTextByWords(QString text, const LanguagePair &l)
    {
        WordList wordList;
        QString currentWord;
        languagePair = LanguagePair(l);

        unifySpecialCharacters(text);

        for (int i = 0; i < text.length(); ++i)
        {
            if (text[i].isSpace())
            {
                appendWordToList(currentWord, wordList);
                currentWord.clear();
                wordList.append(Word(QString(text[i]), WordType::Space));
            }
            else if (isPunct(text[i]))
            {
                appendWordToList(currentWord, wordList);
                currentWord.clear();
                wordList.append(Word(QString(text[i]), WordType::Punct));
            }
            else
            {
                currentWord += text[i];
            }
        }

        mergeExceptions(wordList);

        return wordList;
    }

private:
    static WordType getTypeForWord(const QString &word)
    {
        if (languagePair.isPartOfException(word))
        {
            return WordType::PotentionException;
        }
        else
        {
            return WordType::NonException;
        }
    }

    /*
     * for cases when an exception consists of more than one word
    */
    static void mergeExceptions(WordList &words)
    {
        auto exceptions = languagePair.getExceptions();

        for (int i = 0; i < words.count(); ++i)
        {
            WordList potentialExceptionsList;
            int exceptionEndIndex = -1;
            while (words[i].type == WordType::PotentionException && i < words.count())
            {
                potentialExceptionsList.append(words[i]);
                exceptionEndIndex = i;
                ++i;
            }

            QString potentialException = potentialExceptionsList.join(" ");
            if (exceptions.containsString(potentialException))
            {
                auto &word = words.mergeWords(exceptionEndIndex - potentialExceptionsList.count(), exceptionEndIndex);
                word.type = WordType::Exception;
            }
        }
    }

    static void appendWordToList(const QString &word, WordList &list)
    {
        if (!word.isEmpty())
        {
            auto type = getTypeForWord(word);
            list.append({ word, type });
        }
    }

    static void unifySpecialCharacters(QString &text)
    {
        // unifying apostrophes
        foreach(const auto &apostroph, apostrophes)
        {
            if (text.contains(apostroph))
            {
                text.replace(apostroph, "\'");
            }
        }
    }

    static bool isPunct(const QChar &c)
    {
        return c.isPunct() && !isApostrophe(c);
    }

    static bool isApostrophe(const QChar &c)
    {
        return c == '\'';
    }

private:
    inline static LanguagePair languagePair;

    inline static const QStringList apostrophes = { };
};

#endif // WORDS_H
