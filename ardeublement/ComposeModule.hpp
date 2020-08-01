#ifndef COMPOSEMODULE_HPP
#define COMPOSEMODULE_HPP

#include "Params.hpp"

/*
 * init() - a module MUST discard runtime state and re-initialize with the given parameters
 * set() - a module MUST incorporate the given parameters into its current run, and it MAY re-initialize
 */

 /*
  * Possible interface ideas:
  * Name: for modules to display their name
  * next(voice): for polyphonic or multitimbral compositions, get the next note in a voice
  * SOMETHING for rhythm!
  */

class ComposeModule {
  public:
    virtual void init(Params p) = 0;
    virtual void set(Params p) = 0;
    virtual byte next() = 0;
  //  virtual void next(int n, byte *out);
};

#endif
