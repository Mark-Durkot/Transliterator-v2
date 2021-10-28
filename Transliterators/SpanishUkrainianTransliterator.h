#ifndef SPANISHUKRAINIANTRANSLITERATOR_H
#define SPANISHUKRAINIANTRANSLITERATOR_H

#include "Transliterator.h"

class SpanishUkrainianTransliterator : public Transliterator
{
public:
    SpanishUkrainianTransliterator(LanguagePair *l, QObject *parent=nullptr) : Transliterator(l, parent) {};
};

#endif // SPANISHUKRAINIANTRANSLITERATOR_H
