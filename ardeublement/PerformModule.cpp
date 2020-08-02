#include <arduino.h>
#include <MIDI.h>
#include "MidiClock.hpp"
#include "PerformModule.hpp"
#include "ComposeGaussian.hpp"
#include "ComposeDrunk.hpp"
#include "Util.hpp"
#include "Logger.hpp"

#define LOGGER "PerformModule"

union LongBytes {
  long longval;
  byte bytes[sizeof(long)];
};


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
  
  int i;
  for (i=0; i<NUM_VOICES; i++)
	  this->voices[i].chan = CH_OUT;
}

// todo: expiry scheduling is less good than ticks%duration, because voices can drift off-count
// modulo ticks will always be on agreed-upon counts.
// however, modulo ticks stands in the way of some humanize possibilities.
// maybe duration and over/under hold? that would allow following notes to receive the
// remainder of their nominal duration.
void PerformModule::sendVoice() {
// todo: actiontype NONE for init, stream dead, etc

	// does this term re-sync at all?
	int jitter = this->ticks % THIRTYSECOND;

	int i;
	for (i=0; i<NUM_VOICES; i++) {
	  Voice *v = &this->voices[i];
	  ActionQueue *a = &this->actions[i];
	  
	  if (v->playing) {
//	  	if (this->ticks % v->action.dur == 0) {
		if (v->expires == this->ticks) {
		  //Logger::log(LOGGER, "expires: %d, ticks: %d", expires, this->ticks);
		  if (v->action.mode == VoiceMode::Note) {
			this->MIDI.sendNoteOff(v->action.note, 0, v->chan);
		  }
		  v->playing = false;
		}
	  }

	  if (!v->playing) {
		if (!a->isEmpty()) {
    	  //Logger::log(LOGGER, "(sendVoice) sending voice %d", i);
		  v->action = a->pop();

		  if (v->action.mode == VoiceMode::Note) {
			this->MIDI.sendNoteOn(v->action.note, v->action.vel, v->chan);
		  }

		  v->expires = this->ticks + v->action.dur + jitter;
		  v->playing = true;
		}
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
    int i;
    for (i=0; i<NUM_VOICES; i++) {
    	//Logger::log(LOGGER, "(service) servicing voice %d", i);
    	ActionQueue *a = &this->actions[i];
    	
    	if (!a->isFull()) {
			union LongBytes r;
			r.longval = FastRand::random();

			bool rest  = (r.bytes[0] < 32);
	
			VoiceAction next = {
			  mode: (rest ? VoiceMode::Rest : VoiceMode::Note),
			  note: (rest ? 0 : this->compose->next()),
			  vel:  r.bytes[2] / 2 + 128,
			  dur:  (r.bytes[1] > 127 ? QUARTER : r.bytes[1] > 63 ? HALF : r.bytes[1] > 31 ? WHOLE : EIGHTH),
			};

			a->unshift(next);
		}
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
  int i;
  for (i=0; i<NUM_VOICES; i++) {
  	Voice *v = &this->voices[i];
    this->MIDI.sendNoteOff(v->action.note, 0, v->chan);
  }
}








// todo: polyphonic perform mode
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
