#ifndef COMPOSEGAUSSIAN_HPP
#define COMPOSEGAUSSIAN_HPP

#include "ComposeModule.hpp"

class ComposeGaussian : public ComposeModule {
  private:
    byte center = 0;
    int dev = 4;

  public:
  // ComposeModule
    void init(Params p);
    void set(Params p);
    byte next();

};

#endif
