#ifndef PERFORMMULTITIMBRAL_HPP
#define PERFORMMULTITIMBRAL_HPP

#include <Arduino.h>
#include "PerformModule.hpp"

class PerformMultitimbral : public PerformModule {
	
	public:
		PerformMultitimbral(MidiInterface &midi_interface);
	
	// PerfomModule
	void service();
};

#endif
