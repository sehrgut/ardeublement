#include <arduino.h>
#include <MIDI.h>
#include "MidiClock.hpp"
#include "PerformModule.hpp"

// todo: pass in global params

enum ClockDivisions {
  WHOLE         = 24 * 4,
  HALF          = 24 * 2,
  QUARTER       = 24,
  EIGHTH        = 24 / 2,
  SIXTEENTH     = 24/4,
  THIRTYSECOND  = 24/8,
};

//------------------------------for midi and beat timing-------------------------------//
String divNames[6] ={"1/1","1/2","1/4","1/8","1/16","1/32"};
int divRatio[6] = {96,48,24,12,6,3};//note multipliers

// todo: track active nots to turn them off again
// todo: set channels outside?

PerformModule::PerformModule(MidiInterface& midi_interface)
  : MIDI(midi_interface)
{
  this->prevticks = 0;
  this->curticks = 0;
}

void PerformModule::sendVoice1() {
   if (this->curticks%QUARTER == 0) {
       this->MIDI.sendNoteOff(60,0,1);
       this->MIDI.sendNoteOn(60,125,1);
  }   
}

void PerformModule::service() {
  this->sendVoice1();
}

void PerformModule::tick(unsigned long ticks) {
  this->prevticks = this->curticks;
  this->curticks = ticks;

  this->service();
}

void PerformModule::stop() {
  this->MIDI.sendNoteOff(60,0,1);
}
