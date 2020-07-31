#include <arduino.h>
#include <math.h>
#include "util.h"

#define GAUSS_DEPTH 3
#define GAUSSINT_STDEVS 3

// todo: cpp-ify for convenience?

double gaussrand()
{
  double x = 0;
  int i;
  for(i = 0; i < GAUSS_DEPTH; i++)
    x += (double)random() / RANDOM_MAX;

  x -= GAUSS_DEPTH / 2.0;
  x /= sqrt(GAUSS_DEPTH / 12.0);

  return x;
}

int gaussint(int lo, int hi) {
// https://stackoverflow.com/a/2751953/2255888
  int x = (hi + lo) / 2;
  int s = (hi - lo) / 2;
  return round(gaussrand() * s + x);
}
