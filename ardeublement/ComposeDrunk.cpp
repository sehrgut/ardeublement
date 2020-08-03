#include <math.h>
#include "Scale.hpp"
#include "ComposeDrunk.hpp"
#include "Logger.hpp"
#include "Util.hpp"

/*
okay what about this?
Coherence: tropism to previous note vs tropism to center
	coh=0.0, 100% center tropism, aka Gaussian Sample
	coh=1.0, 100% previous-note tropism, aka Drunkard's Walk
*/

#define LOGGER "ComposeDrunk"

ComposeDrunk::ComposeDrunk() {
}

ComposeDrunk::~ComposeDrunk() {
  delete this->sc;
}

void ComposeDrunk::init(Params p) {
  this->counter = 0;
  this->tonality = p.tonality;
  this->stepSize = p.deviation;
  this->setCenter(p.center);
}

void ComposeDrunk::set(Params p) {
  this->setStep(p.deviation);
  this->setCenter(p.center);
}

void ComposeDrunk::setStep(int n) {
  // check useful values in range
  this->stepSize = n;
}

void ComposeDrunk::setCenter(byte n) {
  // n guaranteed to be MIDI note number
  delete this->sc;
  this->sc = new Scale(n); // todo: scale should not require destruction
  if (counter == 0)
    this->prev = n;
  this->center = n;
}

byte ComposeDrunk::next() {
  this->counter++;
  byte lo = 21;
  byte hi = 108;
  int range = hi - lo;
  
  double centeredness = 0.5; // between 0 and 1
  //todo: make tropism weighted by a central musical parameter, probably coherence
  int tropism = round(((double) (this->center - this->prev) / range) * this->stepSize * centeredness);
  byte minNote = max(lo, this->prev - this->stepSize + tropism);
  byte maxNote = min(hi, this->prev + this->stepSize + tropism);
  //Logger::log(LOGGER, "(next) picking between %d and %d", minNote, maxNote);

  byte a = (byte)Gaussian::gauss8(minNote, maxNote);
  if (this->tonality > 0 && Uniform::random_bool(this->tonality) == true)
    a = this->sc->nearest(a);

  return (this->prev = a);
}
