#ifndef PASSPORTUKRAINIANTRANSLITERATOR_H
#define PASSPORTUKRAINIANTRANSLITERATOR_H

#include "Transliterator.h"

class PassportUkrainianTransliterator : public Transliterator
{
public:
    PassportUkrainianTransliterator(LanguagePair *l, QObject *parent=nullptr) : Transliterator(l, parent) {}
};

#endif // PASSPORTUKRAINIANTRANSLITERATOR_H
