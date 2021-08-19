#ifndef SYLLABLEPAIR_H
#define SYLLABLEPAIR_H

#include <QString>

enum SyllableType
{
    Default   = 0,
    WordStart = 1,
    WordEnd   = 2
};

class SyllablePair
{
public:
    SyllablePair() = default;

    SyllablePair(const QString &first, const QString &second, SyllableType type=Default)
    {
        this->first  = first;
        this->second = second;
        this->type   = type;
    }

    const QString &getFirst() const       { return first;  }
    const QString &getSecond() const      { return second; }
    const SyllableType &getType() const   { return type;   }

    QString &getFirst()                   { return first;  }
    QString &getSecond()                  { return second; }
    SyllableType &getType()               { return type;   }

    void setFirst(const QString &first)   { this->first  = first;  }
    void setSecond(const QString &second) { this->second = second; }
    void setType(SyllableType type)       { this->type   = type;   }

    void swap() { qSwap(first, second); }

    bool operator==(const SyllablePair &p) const { return first == p.first && second == p.second; }

    bool isShorterThan(const SyllablePair &p) const { return first.length() < p.first.length(); }

private:
    QString first;
    QString second;
    SyllableType type;
};

using SyllableList   = QList<const SyllablePair *>;
using SyllableList2D = QList<QList<const SyllablePair *>>;

#endif // SYLLABLEPAIR_H
