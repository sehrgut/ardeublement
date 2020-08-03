#include <arduino.h>
#include "Logger.hpp"
#include "Scale.hpp"

// todo: implement scales as bit arrays with bitmask tests
// todo: implement root note offset, so things aren't just C
// todo: modes
// todo: bytes?
// todo: modular index into scales to avoid copy-and-rotate?
// todo: return early if pegged
// todo: Scala support

#define LOGGER "Scale"

void array_rotate_right(bool a[], int len)
{
  int i;
  bool temp;
  temp = a[0];
  for (i = 0; i < len-1; i++)
     a[i] = a[i+1];
  a[i] = temp;
}
 
void array_rotate(bool a[], int len, int d)
{
  int i, j;
  d = d < 0 ? len-d : d; // rotate_left(n, d) == rotate_right(n, n - d)
  for (i = 0; i < d; i++)
    array_rotate_right(a, len);
}


static bool sc_major[12] = { true, false, true, false, true, true, false, true, false, true, false, true };

Scale::Scale () : Scale(0) {};

Scale::Scale (int root) {
  memcpy(this->sc, sc_major, sizeof(sc_major));
  this->root = root;
  array_rotate((this->sc), 12, root%12);
}


// todo: up() and down() only look up to a whole step away, which is fine for church modes, but not for general scales
int Scale::up(int n) {
  Logger::log(LOGGER, "(up) note %d in scale? %s", n, this->sc[n%12] ? "true" : "false");
  if (this->sc[(n+1)%12]) {
    return min(n+1, 127);
  } else if (this->sc[(n+2)%12]) {
    return min(n+2, 127);
  }
}

int Scale::down(int n) {
  Logger::log(LOGGER, "(down) note %d in scale? %s", n, this->sc[n%12] ? "true" : "false");
  if (this->sc[(n-1)%12]) {
    return max(n-1, 0);
  } else if (this->sc[(n-2)%12]) {
    return max(n-2, 0);
  }  
}

// todo: rename? has(), contains()
bool Scale::in(int n) {
  return this->sc[n%12];
}

// todo: this assumes repeating octave scales. stop that.
// todo: what about under/overflow?
byte Scale::nearest(byte n) {
  int i;

  if (in(n)) return n; // the trivial case
  
  for (i=0; i<6; i++) {
    if (in(n+i))
      return n+i;
    if (in(n-i))
      return n-i;
  }
}
