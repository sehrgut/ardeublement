#include <arduino.h>
#include <TimerThree.h>
#include "MidiClock.hpp"
#include "Logger.hpp"

#define LOGGER "MidiClock"
#define PPQN 24.0

MidiClock::MidiClock() {
  this->prev_us = micros();
  this->cur_us = micros();
  
  Timer3.initialize(MIDICLOCK_RESOLUTION_MICROS); //todo: ties-up some pins, check docs
}

MidiClock::~MidiClock() {
  Timer3.detachInterrupt();
}

static MidiClock& MidiClock::instance() {
  static MidiClock _instance;

  return _instance;
}

void MidiClock::tick() { //do this every time the Timer3 interval timer is triggered
  // from https://github.com/mortonkopf/Teensy-Midi-Euclidean-Sequencer/blob/master/Teensy_Euclidean_Sequencer/Teensy_Euclidean_Sequencer.ino
  // merge back from there if external clock becomes needed

  MidiClock& that = MidiClock::instance();
  
  that.cur_us = micros();
  that.pulse_interval = (1000/(that.bpm/60.0))/PPQN;//set the midi clock using ref to bpm
  
  if ((that.cur_us - that.prev_us) > that.pulse_interval*1000) {

    // service watchers
    int i;
    for (i=0; i<that.watchers_count; i++)
      that.watchers[i]->tick(that.ticks);

    
    that.prev_us = that.cur_us;
    that.ticks++;
  
  }//end if intervl time hit
}

void MidiClock::start() {
  Logger::log(LOGGER, "(start) received start signal");
  if (this->running == false) {
    Timer3.attachInterrupt(this->tick);
    this->running == true;
  }
}

void MidiClock::stop() {
  Logger::log(LOGGER, "(stop) received stop signal");
  Timer3.detachInterrupt();
  this->running = false;

  // service watchers
  int i;
  for (i=0; i<this->watchers_count; i++)
    this->watchers[i]->stop();
}

bool MidiClock::watch(MidiClockWatcher *obj) {
  // todo: unwatch()
  if (this->watchers_count < MIDICLOCK_NUM_WATCHERS) {
    this->watchers[this->watchers_count++] = obj;
    return true;
  } else {
    return false;
  }
  
}
