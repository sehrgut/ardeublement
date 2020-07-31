#ifndef SCALE_HPP
#define SCALE_HPP

#include <arduino.h>

class Scale {

  private:
    bool sc[12];
    int root;

  public:
    Scale ();
    Scale (int root);
    int up(int n);
    int down(int n);
    bool in(int n);

};

#endif
