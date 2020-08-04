#ifndef PERFORMMULTITIMBRAL_HPP
#define PERFORMMULTITIMBRAL_HPP

#include <Arduino.h>
#include "PerformModule.hpp"

template <size_t VOICE_COUNT>
class PerformMultitimbral : public PerformModule<VOICE_COUNT> {
	
	public:
		PerformMultitimbral(MidiInterface &MIDI);
	
	// PerfomModule
	void service();
};

#include "PerformMultitimbral-impl.hpp"

#endif
