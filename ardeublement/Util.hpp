#ifndef UTIL_HPP
#define UTIL_HPP

// todo: private constructors

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
