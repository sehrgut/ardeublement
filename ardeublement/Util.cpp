#ifdef ARDUINO
#include <arduino.h>
#else

#include <math.h>
#include <cstdlib>
typedef unsigned char byte;
uint32_t micros() { return 0x7FFF; }
#define RANDOM_MAX 0x7FFFFFFF
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))

#endif

#include <math.h>
#include "util.hpp"

#define GAUSS_DEPTH 3
#define GAUSSINT_STDEVS 3

// todo: printf-with-floats

typedef union {
	long longval;
	short shorts[2];
	byte bytes[4];
} long_byte_u;

typedef union {
	long longs[2];
	short shorts[4];
} long_short_u;

uint16_t FastRand::state16 = 0;
uint32_t FastRand::state32 = 0;

FastRand::FastRand() { }

// todo: accept analog pin # and pack bits until enough
void FastRand::srand(uint16_t s) {
	while (s == 0)
		s = micros();

  FastRand::state16 = s;
}

void FastRand::srandom(uint32_t s) {
  while (s == 0)
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


Gaussian::Gaussian() { }

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

float gauss_8bit_resolution() {
	float x = 0;
	long_byte_u r;

	r.longval = FastRand::random();

	int i;
	for (i=0; i<4; i++)
		x += (float)r.bytes[i] / 0x7F;

	x -= 4;

	return x;
}

char Gaussian::gauss8(char lo, char hi) {
/*
 * assuming lo and hi are correct instead of checking for min and max save 3% CPU time,
 * so calling code needs to honour the convention.
 */
	float x;
	int m = (lo + hi)/2;
/*
 * Scaling the gaussian to +/- 3 standard deviations minimizes clamping. If other
 * scalings are needed (read: flatter distributions), perhaps a gauss8_in_dist would
 * be useful.
 */
	int s = (hi - lo)/6; // two-tailed 3*stdev scaling

	float lof = (float)(lo - m) / s;
	float hif = (float)(hi + 1 - m) / s;

/*
 * lof/hif are float values that yield lo and hi when passed through
 * floor(x*s+m). It's 3% faster over 10M runs to precompute, since ~3%
 * of generated values fall outside the clamp using the 3*stdev heuristic.
 */
	
	// Clamp values between lo and hi to avoid rollover issues in calling code
	do {
		x = gauss_8bit_resolution();
	} while (x < lof || x > hif);
	
	return floor(x * s + m);

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

  do {
    out = gauss_in_dist(mean, stdev);
  } while (out < lo || out > hi);

  return out;
}


Uniform::Uniform() { }

// todo: efficiently use random numbers.
// Are bitmask/shif ops fast enough to justify the space?
// Could easily support fine-grained probability with 8 bits at a time instead of wasting 32 bits on it.
bool Uniform::random_bool(double p) {
  return (double)FastRand::rand() / RAND_MAX < p;
}
