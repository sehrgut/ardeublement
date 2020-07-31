#ifndef DRUNK_HPP
#define DRUNK_HPP

#include <arduino.h>
#include "Scale.hpp"

class Drunk {
  private:
    int prev;
    int stepSize = 1;
    Scale *sc;

  public:
    Drunk(int startNote);
    void setStep(int n);
    int next();
    int nextDiatonic();
};

#endif
