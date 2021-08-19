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

        firstLanguageName     = l.firstLanguageName;
        secondLanguageName    = l.secondLanguageName;
        twoWayTransliteration = l.twoWayTransliteration;
    }

    static LanguagePair *createLanguagePair(const QString &filename)
    {
        XMLParser parser;
        parser.setContent(filename);

        LanguagePair *languagePair = new LanguagePair();

        languagePair->firstLanguageName     = parser.parseFirstLanguageName();
        languagePair->secondLanguageName    = parser.parseSecondLanguageName();
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
            qSwap(firstLanguageName, secondLanguageName);
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

    const QString  &getFirstLanguageName() { return firstLanguageName;     }
    const QString &getSecondLanguageName() { return secondLanguageName;    }
    bool isTwoWayTransliteration()         { return twoWayTransliteration; }

    bool isPartOfException(QString s) const
    {
        s = s.toLower();

        for (const auto &e : exceptions)
        {
            for (auto word : e.getFirst().split(" "))
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
            if (e.getFirst().toLower() == s) { return true; }
        }

        return false;
    }

private:
    SyllablePairSet syllables;
    SyllablePairSet exceptions;

    QString  firstLanguageName;
    QString secondLanguageName;
    bool twoWayTransliteration;
};

#endif // LANGUAGEPAIR_H
