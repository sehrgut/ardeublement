#include <math.h>
#include "Scale.hpp"
#include "Drunk.hpp"
#include "Logger.hpp"
#include "Util.hpp"

// todo: "gravity" towards start note, less probability of higher steps as higher on scale
// todo: normal dists

#define NAME "Drunk"

Drunk::Drunk() {
}

Drunk::~Drunk() {
  delete this->sc;
}

double gravity(int n, int pivot) {
  int delta = pivot - n;
  if (delta > 0) {
    return delta / (128 - pivot);
  } else {
    return delta / pivot;
  }
}

int Drunk::nextDiatonic() {
  this->counter++;
  /*
  double pos = (double)(this->prev) / 128;
  int lo = 0-(ceil(pos * 8));
  int hi = lo + 7;
  Logger::log("[Drunk::nextDiatonic] picking random(%d, %d)\n", lo, hi);
  int delta = gaussint(lo, hi);
  int steps = abs(delta);
  */

  double g = gravity(this->prev, 60);
  int delta = round (Gaussian::gaussint(-4,4) + (g * 4));
  int steps = abs(delta);
  
  Logger::log(NAME, "(nextDiatonic) delta=%d (%d)", delta, steps);

  int i;
  int next = this->prev;
  
  if (delta > 0) {
    for (i=0; i<steps; i++)
      next = sc->up(next);
  } else if (delta < 0) {
    for (i=0; i<steps; i++)
      next = sc->down(next);
  }

  return this->prev = next;
}

void Drunk::init(Params p) {
  this->counter = 0;
  this->stepSize = p.deviation;
  this->setCenter(p.center);
}

void Drunk::set(Params p) {
  this->setStep(p.deviation);
  this->setCenter(p.center);
}

void Drunk::setStep(int n) {
  // check useful values in range
  this->stepSize = n;
}

void Drunk::setCenter(byte n) {
  // n guaranteed to be MIDI note number
  delete this->sc;
  this->sc = new Scale(n); // todo: scale should not require destruction
  if (counter == 0)
    this->prev = n;
}

byte Drunk::next() {
  this->counter++;
  byte minNote = max(0, this->prev - this->stepSize);
  byte maxNote = min(127, this->prev + this->stepSize);
  Logger::log(NAME, "(next) picking between %d and %d", minNote, maxNote);
  return this->prev = (byte)Gaussian::gaussint(minNote, maxNote);
}
