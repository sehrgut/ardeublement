#include <arduino.h>
#include "Util.hpp"

#define LOGGER "PerformMultitimbral"

template <size_t VOICE_COUNT>
PerformMultitimbral<VOICE_COUNT>::PerformMultitimbral(MidiInterface &MIDI)
	: PerformModule<VOICE_COUNT>(MIDI)
{ }

// doesn't know about ticks, just about notes and how long they are
template <size_t VOICE_COUNT>
void PerformMultitimbral<VOICE_COUNT>::service() {

#define RLEN 8

	static byte rcur[VOICE_COUNT] = {0};
	static byte rhythm[VOICE_COUNT][RLEN] = {0};

    int i;
    for (i=0; i<VOICE_COUNT; i++) {
    	//Logger::log(LOGGER, "(service) servicing voice %d", i);
    	ActionQueue *a = &this->actions[i];
    	
    	if (!a->isFull()) {
			union LongBytes r;
			r.longval = FastRand::random();

// todo: rhythms now should be their own module.
// todo: permute rhythm based on coherence?
			bool rest  = (r.bytes[0] < 32);

			if (rhythm[i][rcur[i]] == 0) {
				rhythm[i][rcur[i]] = (
					r.bytes[1] > 127 ? QUARTER
					: r.bytes[1] > 63 ? HALF
					: r.bytes[1] > 31 ? WHOLE
					: EIGHTH);
			} // else do something else with rbytes1?
	
			VoiceAction next = {
			  mode: (rest ? VoiceMode::Rest : VoiceMode::Note),
			  note: (rest ? 0 : this->compose->next()),
			  vel:  r.bytes[2] / 2 + 128,
			  dur:  rhythm[i][rcur[i]],
			};

			a->unshift(next);
			
			// need to bench this. Is it really faster than % with the avr compiler
			// optimizations? see https://stackoverflow.com/a/13773421/2255888
			rcur[i] = rcur[i] + 1 == RLEN ? 0 : rcur[i] + 1;
		}
	}
}