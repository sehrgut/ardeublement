#include <arduino.h>
#include "Logger.hpp"
#include "ComposeGaussian.hpp"
#include "Util.hpp"

#define MIN_NOTE 36
#define MAX_NOTE 96
#define MIDDLE_C 60
#define NAME "ComposeGaussian"

ComposeGaussian::~ComposeGaussian() {
  if (this->sc != NULL)
    delete this->sc;
}

void ComposeGaussian::init(Params p) {
  this->set(p);  
}

void ComposeGaussian::set(Params p) {
  if (p.center < MIN_NOTE || p.center > MAX_NOTE) {
    this->center = MIDDLE_C;
  } else {
    this->center = p.center;    
  }

  this->tonality = p.tonality;

  if (this->sc != NULL) delete this->sc;
  if (tonality > 0) this->sc = new Scale(this->center);

  this->dev = p.deviation;
}

byte ComposeGaussian::next() {  
  byte lo = max(0, this->center - (3*this->dev));
  byte hi = min(127, this->center + (3*this->dev));

  //Logger::log(NAME, "looking for int between %d and %d", lo, hi);
  byte a = (byte)Gaussian::clamped_gauss(double(lo), double(hi));

  if (this->tonality > 0 && Uniform::random_bool(this->tonality) == true)
    a = this->sc->nearest(a);
  
  //Logger::log(NAME, "found %d", a);
  return a;
}
