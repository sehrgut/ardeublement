#include "CDF.hpp"

void cdf(double *in, double *out, int n) {
  int i;
  int sum = 0;

  for (i=0; i<n; i++)
    sum += in[i];

  for (i=0; i<n; i++)
    out[i] = in[i]/sum;
}

int pick(double *f, double p, int n) {
  int i;
  int sum = 0;
  for (i=0; i<n; i++) {
    sum += f[i];
    if (sum >= p) return n;
  }
}
