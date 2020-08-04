#include <arduino.h>
#include "Util.hpp"
#include "PerformPolyphonic.hpp"

#define LOGGER "PerformPolyphonic"


PerformPolyphonic::PerformPolyphonic(MidiInterface &midi_interface)
	: PerformModule(midi_interface) { }

// doesn't know about ticks, just about notes and how long they are
void PerformPolyphonic::service() {

#define RLEN 8

	static byte rcur[NUM_VOICES] = {0};
	static byte rhythm[RLEN] = {0};

    int i;
    for (i=0; i<NUM_VOICES; i++) {
    	//Logger::log(LOGGER, "(service) servicing voice %d", i);
    	ActionQueue *a = &this->actions[i];
    	
    	if (!a->isFull()) {
			union LongBytes r;
			r.longval = FastRand::random();

// todo: rhythms now should be their own module.
// todo: permute rhythm based on coherence?
			bool rest  = (r.bytes[0] < 32);

			if (rhythm[rcur[i]] == 0) {
				rhythm[rcur[i]] = (
					r.bytes[1] > 127 ? QUARTER
					: r.bytes[1] > 63 ? HALF
					: r.bytes[1] > 31 ? WHOLE
					: EIGHTH);
			} // else do something else with rbytes1?
	
			VoiceAction next = {
			  mode: (rest ? VoiceMode::Rest : VoiceMode::Note),
			  note: (rest ? 0 : this->compose->next()),
			  vel:  r.bytes[2] / 2 + 128,
			  dur:  rhythm[rcur[i]],
			};

			a->unshift(next);
			
			// need to bench this. Is it really faster than % with the avr compiler
			// optimizations? see https://stackoverflow.com/a/13773421/2255888
			rcur[i] = rcur[i] + 1 == RLEN ? 0 : rcur[i] + 1;
		}
	}
}