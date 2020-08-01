#ifndef DRUNK_HPP
#define DRUNK_HPP

#include <arduino.h>
#include "ComposeModule.hpp"
#include "Scale.hpp"

// BUG: after stop and start, Drunk picks next note == 0
// todo: ComposeModule abstract class

class Drunk : public ComposeModule {
  private:
    int counter = 0;
    int prev = 0;
    int stepSize = 1;
    Scale *sc = NULL;
    void setStep(int n);
    void setCenter(byte n);

  public:
    Drunk();
    ~Drunk();

  // ComposeModule
    void init(Params p);
    void set(Params p);
    byte next();

    
    int nextDiatonic();
};

#endif
