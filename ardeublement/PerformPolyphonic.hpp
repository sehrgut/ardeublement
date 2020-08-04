#ifndef PERFORMPOLYPHONIC_HPP
#define PERFORMPOLYPHONIC_HPP

#include "PerformModule.hpp"

template <size_t VOICE_COUNT>
class PerformPolyphonic : public PerformModule<VOICE_COUNT> {
	
	public:
		PerformPolyphonic(MidiInterface &MIDI);
	
	// PerfomModule
	void service();
};

#include "PerformPolyphonic-impl.hpp"

#endif
