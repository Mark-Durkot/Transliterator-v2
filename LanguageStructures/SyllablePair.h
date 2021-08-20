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

    SyllablePair(const QString &source, const QString &target, SyllableType type=Default)
    {
        this->source = source;
        this->target = target;
        this->type   = type;
    }

    const QString &getSource()    const   { return source; }
    const QString &getTarget()    const   { return target; }
    const SyllableType &getType() const   { return type;   }

    void setSource(const QString &source) { this->source  = source; }
    void setTarget(const QString &target) { this->target  = target; }
    void setType(SyllableType type)       { this->type    = type;   }

    void swap() { qSwap(source, target); }

    bool operator==(const SyllablePair &p) const { return source == p.source && target == p.target; }

    bool isShorterThan(const SyllablePair &p) const { return source.length() < p.source.length(); }

private:
    QString source;
    QString target;
    SyllableType type;
};

using SyllableList   = QList<const SyllablePair *>;
using SyllableList2D = QList<QList<const SyllablePair *>>;

#endif // SYLLABLEPAIR_H
