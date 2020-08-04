#include <arduino.h>
#include "MidiClock.hpp"
#include "ComposeGaussian.hpp"
#include "ComposeDrunk.hpp"
#include "ComposeRand.hpp"
#include "Util.hpp"
#include "Logger.hpp"



// todo: set channels outside?

template <size_t VOICE_COUNT>
PerformModule<VOICE_COUNT>::PerformModule(MidiInterface& MIDI)
  : MIDI(MIDI)
{
  this->compose = new ComposeRand();
  this->ticks = 0;
  
  
  int i;
  for (i=0; i<VOICE_COUNT; i++)
	  this->voices[i].chan = CH_OUT;
}

// todo: expiry scheduling is less good than ticks%duration, because voices can drift off-count
// modulo ticks will always be on agreed-upon counts.
// however, modulo ticks stands in the way of some humanize possibilities.
// maybe duration and over/under hold? that would allow following notes to receive the
// remainder of their nominal duration.
template <size_t VOICE_COUNT>
void PerformModule<VOICE_COUNT>::sendVoices() {
// todo: actiontype NONE for init, stream dead, etc

	// does this term re-sync at all?
	int jitter = this->ticks % THIRTYSECOND;

	int i;
	for (i=0; i<VOICE_COUNT; i++) {
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
template <size_t VOICE_COUNT>
void PerformModule<VOICE_COUNT>::init(Params p) {
  this->compose->init(p);
}

template <size_t VOICE_COUNT>
void PerformModule<VOICE_COUNT>::set(Params p) {
  this->compose->set(p);
}

template <size_t VOICE_COUNT>
void PerformModule<VOICE_COUNT>::tick(midiclock_ticks_t ticks) {
  this->ticks = ticks;
  this->sendVoices();
}

template <size_t VOICE_COUNT>
void PerformModule<VOICE_COUNT>::stop() {
  int i;
  for (i=0; i<VOICE_COUNT; i++) {
  	Voice *v = &this->voices[i];
    this->MIDI.sendNoteOff(v->action.note, 0, v->chan);
  }
}







