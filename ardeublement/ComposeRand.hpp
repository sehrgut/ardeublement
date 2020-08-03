#ifndef COMPOSERAND_HPP
#define COMPOSERAND_HPP

#include <arduino.h>
#include "ComposeModule.hpp"
#include "Scale.hpp"

#define MIDDLE_C 60

class ComposeRand : public ComposeModule {
  private:
	byte prev;
    Scale *sc = NULL;

    double deviation = 1.0;
    double tonality = 0.75;
    double coherence = 0.5;
    byte center = MIDDLE_C;
    byte range = 87;

	byte gamut[2];
	double deviation_width;

    void setCenter(byte n);
    void compute_derived_parameters();

  public:
    ComposeRand();
    ~ComposeRand();

  // ComposeModule
    void init(Params p);
    void set(Params p);
    byte next();
};


#endif
