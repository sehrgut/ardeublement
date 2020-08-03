#include <math.h>
#include "Scale.hpp"
#include "ComposeRand.hpp"
#include "Logger.hpp"
#include "Util.hpp"

/*
okay what about this?
Coherence: tropism to previous note vs tropism to center
	coh=0.0, 100% center tropism, aka Gaussian Sample
	coh=1.0, 100% previous-note tropism, aka Drunkard's Walk

There are two kinds of coherence here.
1. Distribution. Gaussian is more coherent, uniform is less
2. Tropism. Do I count drunkard's walk as more or less coherent?

*/

#define LOGGER "ComposeRand"

ComposeRand::ComposeRand() {
  this->setCenter(this->center);
  this->compute_derived_parameters();
}

ComposeRand::~ComposeRand() {
  delete this->sc;
}

void ComposeRand::init(Params p) {
  // don't care about init vs update here
  this->set(p);
}

void ComposeRand::set(Params p) {
  // set primary parameters
  this->tonality = p.tonality;
  this->coherence = p.coherence;
  this->setCenter(p.center);
  this->range = p.range;
  // todo: this will be a proportion later
  this->deviation = (float)p.deviation / this->range * 2; 

  this->compute_derived_parameters();
}

void ComposeRand::compute_derived_parameters() {

  // Gamut: the notes available for composition, defined by Range and Center
  int gamut_lo = max(0, this->center - (this->range / 2));
  int gamut_hi = min(127, gamut_lo + this->range);
  
  // Expand gamut down if it rolls off the top.
  if (gamut_hi == 127)
  	gamut_lo = gamut_hi - this->range;

  this->gamut[0] = gamut_lo;
  this->gamut[1] = gamut_hi;
  
  // Deviation Width: deviation should increase as coherence decreases.
  // This is empirically-determined, but mathematically "elegant-ish",
  // mapping (0,1)=>(3,1), giving a deviation 3x as wide at coherence=0 than
  // at coherence=1.
  // todo: probably #define COHERENCE_DEVIATION_K somewhere?

  this->deviation_width = this->deviation * this->range * (3-(2*this->coherence));
}

void ComposeRand::setCenter(byte n) {
  // n guaranteed to be MIDI note number
  delete this->sc;
  this->sc = new Scale(n); // todo: scale should not require destruction
  this->center = n;
}

byte ComposeRand::next() {
  // Compute attractor for this note, between center and previous note.
  // A drunkard's walk is given by coherence=1, the attractor being the previous note.
  // A gaussian sample is given by coherence=0, the attractor being the center.
  int delta = this->prev - this->center;
  int attractor = this->center + (this->coherence * delta); 
  
  // Compute deviation interval for this note.
  // int instead of byte to allow clampable overflow
  int lo = max(this->gamut[0], attractor - (this->deviation_width / 2));
  int hi = min(this->gamut[1], lo + this->deviation_width);
  if (hi == this->gamut[1])
  	lo = this->gamut[1] - this->deviation_width;
  
  //todo: BUG sometimes it won't start without logline going
  //Logger::log(LOGGER, "(next) picking between %d and %d", lo, hi);

  byte a = (byte)Gaussian::gauss8((byte)lo, (byte)hi);
  if (this->tonality > 0 && Uniform::random_bool(this->tonality) == true)
    a = this->sc->nearest(a);

  return (this->prev = a);
}
