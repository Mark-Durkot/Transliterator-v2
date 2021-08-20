#ifndef LANGUAGEPAIR_H
#define LANGUAGEPAIR_H

#include "SyllablePairSet.h"
#include "Parsers/XMLParser.h"

#include <QObject>

class LanguagePair : public QObject
{
    Q_OBJECT
public:
    LanguagePair(QObject *parent=nullptr)
        : QObject(parent)
    {
    }

    void operator=(const LanguagePair &l)
    {
        syllables  = l.syllables;
        exceptions = l.exceptions;

        sourceLanguageName     = l.sourceLanguageName;
        targetLanguageName    = l.targetLanguageName;
        twoWayTransliteration = l.twoWayTransliteration;
    }

    static LanguagePair *createLanguagePair(const QString &filename)
    {
        XMLParser parser;
        parser.setContent(filename);

        LanguagePair *languagePair = new LanguagePair();

        languagePair->sourceLanguageName     = parser.parseFirstLanguageName();
        languagePair->targetLanguageName    = parser.parseSecondLanguageName();
        languagePair->twoWayTransliteration = parser.parseTwoWayTransliteration();

        languagePair->syllables  = parser.parseSetByTagName("syllable");
        languagePair->exceptions = parser.parseSetByTagName("exception");

        return languagePair;
    }

    void swap()
    {
        if (twoWayTransliteration)
        {
            syllables.swap();
            exceptions.swap();
            qSwap(sourceLanguageName, targetLanguageName);
            sortByDescendingSyllableLenght();
        }
    }

    void sortByDescendingSyllableLenght()
    {
        syllables.sortByDescendingSyllableLenght();
        exceptions.sortByDescendingSyllableLenght();
    }

    const SyllablePairSet &getSyllables()  const { return syllables;  }
    const SyllablePairSet &getExceptions() const { return exceptions; }

    const QString &getSourceLanguageName() const { return sourceLanguageName;     }
    const QString &getTargetLanguageName() const { return targetLanguageName;    }
    bool isTwoWayTransliteration() const         { return twoWayTransliteration; }

    bool isPartOfException(QString s) const

    {
        s = s.toLower();

        for (const auto &e : exceptions)
        {
            for (auto word : e.getSource().split(" "))
            {
                while (word.endsWith("."))
                {
                    word.remove(word.length() - 1, 1);
                }

                if (word.toLower() == s) { return true; }
            }
        }

        return false;
    }

    bool isException(QString s) const
    {
        s = s.toLower();

        for (const auto &e : exceptions)
        {
            if (e.getSource().toLower() == s) { return true; }
        }

        return false;
    }

private:
    SyllablePairSet syllables;
    SyllablePairSet exceptions;

    QString sourceLanguageName;
    QString targetLanguageName;
    bool twoWayTransliteration;
};

#endif // LANGUAGEPAIR_H
