#ifndef UTIL_HPP
#define UTIL_HPP

#include <stdint.h>

union LongBytes {
  long longval;
  byte bytes[sizeof(long)];
};

class FastRand {
  private:
    static uint16_t state16;
    static uint32_t state32;

    FastRand();
    
  public:
    static void srand(uint16_t s);
    static uint16_t rand();
    static void srandom(uint32_t s);
    static uint32_t random();
};

class Gaussian {
  private:
    Gaussian();

  public:
    static double gaussrand();
    static double gauss_in_dist(double mean, double stdev);
    static double clamped_gauss(double a, double b);
    
	static char gauss8(char lo, char hi);

};


class Uniform {
  private:
    Uniform();

  public:
    static bool random_bool(double p);
};

#endif
