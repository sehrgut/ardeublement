#ifndef PERFORMMONOPHONIC_HPP
#define PERFORMMONOPHONIC_HPP

#include <Arduino.h>
#include "PerformModule.hpp"

class PerformMonophonic : public PerformModule<1> {
	
	public:
		PerformMonophonic(MidiInterface &MIDI);
	
	// IPerformer
	void service();
};

#endif
