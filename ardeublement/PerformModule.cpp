#include <arduino.h>
#include "MidiClock.hpp"
#include "PerformModule.hpp"
#include "ComposeGaussian.hpp"
#include "ComposeDrunk.hpp"
#include "ComposeRand.hpp"
#include "Util.hpp"
#include "Logger.hpp"


//------------------------------for midi and beat timing-------------------------------//
char* divNames[6] ={"1/1","1/2","1/4","1/8","1/16","1/32"};
int divRatio[6] = {96,48,24,12,6,3};//note multipliers
char* actionModeNames[2] = {"Rest","Note"};

// todo: set channels outside?

PerformModule::PerformModule(MidiInterface& midi_interface)
  : MIDI(midi_interface)
{
  this->compose = new ComposeRand();
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
void PerformModule::sendVoices() {
// todo: actiontype NONE for init, stream dead, etc

	// does this term re-sync at all?
	int jitter = this->ticks % THIRTYSECOND;

	int i;
	for (i=0; i<NUM_VOICES; i++) {
	  Voice *v = &this->voices[i];
	  ActionQueue *a = &this->actions[i];

// todo: BUG, seems to play a low note first before any voices have been serviced.	  
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


// todo: store reference to params for later use?
void PerformModule::init(Params p) {
  this->compose->init(p);
}

void PerformModule::set(Params p) {
  this->compose->set(p);
}

void PerformModule::tick(midiclock_ticks_t ticks) {
  this->ticks = ticks;
  this->sendVoices();
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
