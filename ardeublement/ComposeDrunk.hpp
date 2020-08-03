#ifndef COMPOSEDRUNK_HPP
#define COMPOSEDRUNK_HPP

#include <arduino.h>
#include "ComposeModule.hpp"
#include "Scale.hpp"

#define MIDDLE_C 60

/** Composes pitches according to a constrained Drunkard's Walk.
 */
class ComposeDrunk : public ComposeModule {
  private:
    int counter = 0;
    int prev = 0;
    int stepSize = 1;
    byte center = MIDDLE_C;
    double tonality = 0.75;
    Scale *sc = NULL;
    void setStep(int n);
    void setCenter(byte n);

  public:
    ComposeDrunk();
    ~ComposeDrunk();

  // ComposeModule
    void init(Params p);
    void set(Params p);
    byte next();
};

#endif
