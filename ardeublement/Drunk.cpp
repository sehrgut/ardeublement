#include <math.h>
#include "Scale.hpp"
#include "Drunk.hpp"
#include "Logger.hpp"
#include "util.h"

// todo: "gravity" towards start note, less probability of higher steps as higher on scale
// todo: normal dists

Drunk::Drunk(int startNote) {
      this->prev = startNote;
      this->sc = new Scale(startNote);
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
  /*
  double pos = (double)(this->prev) / 128;
  int lo = 0-(ceil(pos * 8));
  int hi = lo + 7;
  Logger::log("[Drunk::nextDiatonic] picking random(%d, %d)\n", lo, hi);
  int delta = gaussint(lo, hi);
  int steps = abs(delta);
  */

  double g = gravity(this->prev, 60);
  int delta = round (gaussint(-4,4) + (g * 4));
  int steps = abs(delta);
  
  Logger::log("[Drunk::nextDiatonic] delta=%d (%d)\n", delta, steps);

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

void Drunk::setStep(int n) {
  this->stepSize = n;
}

int Drunk::next() {
  int minNote = max(0, this->prev - this->stepSize);
  int maxNote = min(127, this->prev + this->stepSize);
  Logger::log("[Drunk::next] picking between %d and %d\n", minNote, maxNote);
  return this->prev = gaussint(minNote, maxNote);
}
