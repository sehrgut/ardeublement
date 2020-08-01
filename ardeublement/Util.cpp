#include <arduino.h>
#include <math.h>
#include "util.hpp"

#define GAUSS_DEPTH 3
#define GAUSSINT_STDEVS 3

// todo: cpp-ify for convenience?
// todo: printf-with-floats

double Gaussian::gaussrand() {
  double x = 0;
  int i;
  for(i = 0; i < GAUSS_DEPTH; i++)
    x += (double)random() / RANDOM_MAX;

  x -= GAUSS_DEPTH / 2.0;
  x /= sqrt(GAUSS_DEPTH / 12.0);

  return x;
}

double Gaussian::gauss_in_dist(double mean, double stdev) {
  return gaussrand() * stdev + mean;
}

double Gaussian::clamped_gauss(double a, double b) {
  double out;
  double lo = min(a,b);
  double hi = max(a,b);
  double mean = (a + b) / 2;
  double stdev = (abs(a - b)) / 6;

  { // todo: count clamping rounds
    out = gauss_in_dist(mean, stdev);
  } while (out < lo || out > hi);

  return out;
}

int Gaussian::gaussint(int lo, int hi) {
// https://stackoverflow.com/a/2751953/2255888
  int x = (hi + lo) / 2;
  int s = (hi - lo) / 2;
  return round(gaussrand() * s + x);
}
