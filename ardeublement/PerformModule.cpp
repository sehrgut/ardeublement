#include <arduino.h>
#include <MIDI.h>
#include "MidiClock.hpp"
#include "PerformModule.hpp"
#include "ComposeGaussian.hpp"
#include "Util.hpp"
#include "Logger.hpp"

#define LOGGER "PerformModule"

union LongBytes {
  long longval;
  byte bytes[sizeof(long)];
};

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
char* divNames[6] ={"1/1","1/2","1/4","1/8","1/16","1/32"};
int divRatio[6] = {96,48,24,12,6,3};//note multipliers
char* actionModeNames[2] = {"Rest","Note"};

// todo: set channels outside?

PerformModule::PerformModule(MidiInterface& midi_interface)
  : MIDI(midi_interface)
{
  this->compose = new ComposeGaussian();
  this->ticks = 0;
  this->voice.chan = CH_OUT;
}

void PerformModule::sendVoice() {
// todo: actiontype NONE for init, stream dead, etc

  if (this->voice.playing) {
    //bool action_expires = (this->voice.action.dur == 0 || this->ticks % this->voice.action.dur == 0);
    bool action_expires = (this->voice.expires == this->ticks);
    if (action_expires) {
      //Logger::log(LOGGER, "expires: %d, ticks: %d", expires, this->ticks);
      if (this->voice.action.mode == VoiceMode::Note) {
        this->MIDI.sendNoteOff(this->voice.action.note, 0, this->voice.chan);
      }
      this->voice.playing = false;
    }
  }

  if (!this->voice.playing) {
    if (!this->actions.isEmpty()) {
      this->voice.action = this->actions.pop();

      if (this->voice.action.mode == VoiceMode::Note) {
        this->MIDI.sendNoteOn(this->voice.action.note, this->voice.action.vel, this->voice.chan);
      }

      this->voice.expires = this->ticks + this->voice.action.dur;
      this->voice.playing = true;
    }
  }
  
}

// doesn't know about ticks, just about notes and how long they are
void PerformModule::service() {
  /*
  Logger::log(LOGGER, "(service) servicing performance");

  Logger::log(LOGGER,"(service) this->voice.action.mode: %s", actionModeNames[this->voice.action.mode] );
  Logger::log(LOGGER,"(service) this->voice.action.note: %d", this->voice.action.note);
  Logger::log(LOGGER,"(service) this->voice.action.vel: %d", this->voice.action.vel);
  Logger::log(LOGGER,"(service) this->voice.action.dur: %d", this->voice.action.dur);
  Logger::log(LOGGER,"(service) this->actions.itemCount: %d", this->actions.itemCount());
*/
  while (!this->actions.isFull()) {
    union LongBytes r;
    r.longval = FastRand::random();
    
    VoiceAction next = {
      mode: VoiceMode::Note,
      note: this->compose->next(),
      vel:  r.bytes[0],
      dur:  r.bytes[1] > 127 ? HALF : QUARTER,
    };

    this->actions.unshift(next);
  }
}

void PerformModule::init(Params p) {
  this->compose->set(p);
}

void PerformModule::set(Params p) {
  this->compose->set(p);
}

void PerformModule::tick(midiclock_ticks_t ticks) {
  this->ticks = ticks;

  this->sendVoice();
}

void PerformModule::stop() {
  this->MIDI.sendNoteOff(this->voice.action.note, 0, this->voice.chan);
}







/*
  if (GLOBAL_PARAMS.running == true) { //todo: performer module
    digitalWrite(LED_BUILTIN, HIGH);
    int n = compose->next();
    Logger::log(LOGGER, "Next note: %d", n);
    //arp(n, 2900, 3000, 1000);
    play(n, 300, 50);
    digitalWrite(LED_BUILTIN, LOW);
  }*/


//void arp(int n, int rate, int sustain, int wait) {
//  int n1 = n+4 > 127 ? -1 : n+4;
//  int n2 = n+7 > 127 ? -1 : n+7;
//  
//  MIDI.sendNoteOn(n, 127, CH_OUT);
//  delay(rate);
//  if (n1 >= 0) {
//    MIDI.sendNoteOn(n1, 127, CH_OUT);
//    delay(rate);
//  }
//
//  if (n2 >= 0) {
//    MIDI.sendNoteOn(n2, 127, CH_OUT);
//    delay(rate);
//  }
//
//  delay(sustain);
//  
//  MIDI.sendNoteOff(n, 0, CH_OUT);
//  if (n1 >= 0) MIDI.sendNoteOff(n1, 0, CH_OUT);
//  if (n2 >= 0) MIDI.sendNoteOff(n2, 0, CH_OUT);
//
//  delay(wait);
//}
//
//void play(int n, int sustain, int wait) {
//  MIDI.sendNoteOn(n, 127, CH_OUT);
//  delay(sustain);
//  MIDI.sendNoteOff(n, 0, CH_OUT);
//  delay(wait);
//}
