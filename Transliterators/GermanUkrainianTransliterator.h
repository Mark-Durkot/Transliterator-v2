#ifndef GERMANUKRAINIANTRANSLITERATOR_H
#define GERMANUKRAINIANTRANSLITERATOR_H

#include "Transliterator.h"

class GermanUkrainianTransliterator : public Transliterator
{
public:
    GermanUkrainianTransliterator(LanguagePair *l, QObject *parent=nullptr) : Transliterator(l, parent) {}
};

#endif // GERMANUKRAINIANTRANSLITERATOR_H
