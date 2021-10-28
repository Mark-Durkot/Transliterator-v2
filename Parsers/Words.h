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
    Word(const QString &s="", WordType t=WordType::Undefined) :
        text(s), type(t)
    {
    }

    bool isSpace()              const { return type == WordType::Space;              }
    bool isPunct()              const { return type == WordType::Punct;              }
    bool isIncorrect()          const { return type == WordType::Incorrect;          }
    bool isException()          const { return type == WordType::Exception;          }
    bool isNonException()       const { return type == WordType::NonException;       }
    bool isPotentialException() const { return type == WordType::PotentionException; }
    bool isAlphabetic() const
    {
        return (type == WordType::NonException || type == WordType::Exception || type == WordType::PotentionException);
    }

    void replaceText(QString newText)
    {
        if (!text.isEmpty())
        {
            if (text[0].isUpper()) { newText[0] = newText[0].toUpper(); }
        }

        text = newText;
    }


    QString getHtml() const
    {
        if (this->isSpace())
        {
            if (text == "\t") { return "&emsp;"; }
            if (text == "\n") { return "<br>";   }
        }

        return text;
    }    

    void markWithColor(const QColor &color)
    {
        QString rgb = QString("rgb(%1,%2,%3)").arg(QString::number(color.red()),
                                                   QString::number(color.green()),
                                                   QString::number(color.blue()));

        text = QString("<span style=\"color: %1\">%2</span>").arg(rgb, text);
    }

public:
    QString  text;
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

    Word &merge(int startIndex, int endIndex, const QChar &separator)
    {
        auto &words = *this;

        for (int i = startIndex + 1; i <= endIndex; ++i)
        {
            if (words[i].isAlphabetic())
            {
                words[startIndex].text += separator + words[i].text;
            }
        }

        // removing words from endIndex until startIndex
        if (words[endIndex].isSpace()) { --endIndex; }
        while (endIndex > startIndex)
        {
            words.removeAt(endIndex--);
        }

        return words[startIndex];
    }

    QString join(const QChar &separator)
    {
        if (this->count() == 1) { return this->first().text; }

        QString result;
        foreach(const auto &word, *this)
        {
            if (word.isAlphabetic())
            {
                result += word.text + separator;
            }
        }

        // removing last separator
        result.resize(result.length() - 1);

        return result;
    }
};


class WordPainter
{
public:
    void setExceptionColor(const QColor &c) { exceptionColor = c; }
    void setIncorrectColor(const QColor &c) { incorrectColor = c; }
    QColor getExceptionColor() const { return exceptionColor; }
    QColor getIncorrectColor() const { return incorrectColor; }

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


struct SpecialCharacter
{
    SpecialCharacter(const QString &c, const QString &u) : characters(c), unifiedCharacter(u) {}

    QString characters;
    QString unifiedCharacter;
};


class CharacterUnifier
{
public:
    CharacterUnifier()
    {
        specialCharacters.append(SpecialCharacter("’‘‛′`´", "\'"));
        specialCharacters.append(SpecialCharacter("āáǎà", "a"));
        specialCharacters.append(SpecialCharacter("ēéěèêê", "e"));
        specialCharacters.append(SpecialCharacter("īíǐì", "i"));
        specialCharacters.append(SpecialCharacter("ōóǒò", "o"));
        specialCharacters.append(SpecialCharacter("ūúǔùǖǘǚǜ", "u"));
    }

    void unifyText(QString &text) const
    {
        for (int i = 0; i < text.length(); ++i)
        {
            if (!text[i].isLetter()) { continue; }

            for (const auto &sc : specialCharacters)
            {
                if (sc.characters.contains(text[i]))
                {
                    if (text[i].isLower())
                    {
                        text.replace(i, 1, sc.unifiedCharacter);
                    }
                    else
                    {
                        text.replace(i, 1, sc.unifiedCharacter.toUpper());
                    }
                }
            }
        }
    }

private:
    QList<SpecialCharacter> specialCharacters;
};


class WordCutter
{
public:
    WordCutter(const LanguagePair *l) : languagePair(l) {}

    WordList splitTextByWords(QString text) const
    {
        WordList wordList;
        QString currentWord;

        characterUnifier.unifyText(text);

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

        appendWordToList(currentWord, wordList); // appending last word

        mergeExceptions(wordList);

        return wordList;
    }

private:
    WordType getTypeForWord(const QString &word) const
    {
        if (languagePair->isException(word))       { return WordType::Exception; }
        if (languagePair->isPartOfException(word)) { return WordType::PotentionException; }
        else                                       { return WordType::NonException; }
    }

    /*
     * for cases when an exception consists of more than one word
    */
    void mergeExceptions(WordList &words) const
    {
        for (int i = 0; i < words.count(); ++i)
        {
            WordList potentialExceptionsList;
            int exceptionEndIndex = -1;
            while (i < words.count() && (words[i].isPotentialException() || words[i].isSpace()))
            {
                potentialExceptionsList.append(words[i]);
                exceptionEndIndex = i;
                ++i;
            }

            if (potentialExceptionsList.isEmpty()) { return; }

            QString potentialException = potentialExceptionsList.join(' ');
            if (languagePair->isException(potentialException))
            {
                auto &word = words.merge(exceptionEndIndex - potentialExceptionsList.count() + 1, exceptionEndIndex, ' ');
                word.type = WordType::Exception;
            }
            else
            {
                for (int i = exceptionEndIndex - potentialExceptionsList.count() + 1; i < exceptionEndIndex + 1; ++i)
                {
                    if (words[i].isAlphabetic()) { words[i].type = WordType::NonException; }
                }
            }
        }
    }

    void appendWordToList(const QString &word, WordList &list) const
    {
        if (!word.isEmpty())
        {
            auto type = getTypeForWord(word);
            list.append({ word, type });
        }
    }

    bool isPunct(const QChar &c) const      { return !isApostrophe(c) && !c.isLetter(); }

    bool isApostrophe(const QChar &c) const { return c == '\''; }

private:
    const LanguagePair  *languagePair;
    CharacterUnifier characterUnifier;
};

#endif // WORDS_H
