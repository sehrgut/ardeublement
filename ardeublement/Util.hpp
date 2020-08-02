#ifndef UTIL_HPP
#define UTIL_HPP

// todo: private constructors

class FastRand {
  private:
    static uint16_t state16;
    static uint32_t state32;
  public:
    static void srand(uint16_t s);
    static uint16_t rand();
    static void srandom(uint32_t s);
    static uint32_t random();
};

class Gaussian {

  public:
    static double gaussrand();
    static double gauss_in_dist(double mean, double stdev);
    static double clamped_gauss(double a, double b);
    static int gaussint(int lo, int hi);
};

class Uniform {

  public:
    static bool random_bool(double p);
};

#endif
