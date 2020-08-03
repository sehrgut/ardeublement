#ifndef COMPOSEGAUSSIAN_HPP
#define COMPOSEGAUSSIAN_HPP

#include "Scale.hpp"
#include "ComposeModule.hpp"

/** Composes pitches selected from a Gaussian distribution about the center pitch. 
 */

class ComposeGaussian : public ComposeModule {
  private:
    byte center = 0;
    int dev = 4;
    double tonality = 0.0;
    Scale *sc;

    ~ComposeGaussian();

  public:
  // ComposeModule
    void init(Params p);
    void set(Params p);
    byte next();

};

#endif
