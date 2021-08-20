#ifndef SCIENTIFICUKRAINIANTRANSLITERATOR_H
#define SCIENTIFICUKRAINIANTRANSLITERATOR_H

#include "Transliterator.h"

class ScientificUkrainianTransliterator : public Transliterator
{
public:
    ScientificUkrainianTransliterator(LanguagePair *l, QObject *parent=nullptr) : Transliterator(l, parent) {}
};

#endif // SCIENTIFICUKRAINIANTRANSLITERATOR_H
