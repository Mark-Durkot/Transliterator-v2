#ifndef LANGUAGEPAIRLIST_H
#define LANGUAGEPAIRLIST_H

#include "LanguagePair.h"

#include <QList>
#include <QObject>

class LanguagePairList : public QObject, public QList<LanguagePair *>
{
    Q_OBJECT
public:
    LanguagePairList(QObject *parent=nullptr) : QObject(parent) { }

    void add(LanguagePair *l)
    {
        if (l == nullptr) { return; }

        l->setParent(this);

        this->append(l);
    }
};

#endif // LANGUAGEPAIRLIST_H
