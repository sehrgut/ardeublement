#include <arduino.h>
#include <math.h>
#include "util.hpp"

#define GAUSS_DEPTH 3
#define GAUSSINT_STDEVS 3

// todo: cpp-ify for convenience?
// todo: printf-with-floats

uint16_t FastRand::state16 = 0;
uint32_t FastRand::state32 = 0;

void FastRand::srand(uint16_t s) {
  if (s == 0)
    s = micros() && 0x1FFF; // got this from elsewhere, but why the mask? Did they mean to shift it?
  FastRand::state16 = s;
}

void FastRand::srandom(uint32_t s) {
  if (s == 0)
    s = micros();
  FastRand::state32 = s;
}

// todo: is this faster with y being static and no way to reseed?
uint16_t FastRand::rand() {
  // http://www.retroprogramming.com/2017/07/xorshift-pseudorandom-numbers-in-z80.html
  // Valid tuples are (6, 7, 13); (7, 9, 8); (7, 9, 13); (9, 7, 13)
  uint16_t y = FastRand::state16;
  y ^= y << 6;
  y ^= y >> 7;
  y ^= y << 13;
  return (FastRand::state16 = y);
}

uint32_t FastRand::random() {
  // (13, 17, 5) qv Marsaglia 2003
  uint32_t y = FastRand::state32;
  y ^= y << 13;
  y ^= y >> 17;
  y ^= y << 5;
  return (FastRand::state32 = y);
}

double Gaussian::gaussrand() {
  double x = 0;
  int i;
  for(i = 0; i < GAUSS_DEPTH; i++)
    x += (double)rand() / RAND_MAX;

  // todo: will the compiler optimize out these constant expressions?
  x -= 1.5; // GAUSS_DEPTH / 2.0 for depth=3
  x /= 0.5; //sqrt(GAUSS_DEPTH / 12.0) for depth=3

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

// todo: efficiently use random numbers.
// Are bitmask/shif ops fast enough to justify the space?
// Could easily support fine-grained probability with 8 bits at a time instead of wasting 32 bits on it.
bool Uniform::random_bool(double p) {
  return (double)FastRand::random() / RANDOM_MAX < p;
}
