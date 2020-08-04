#ifndef PERFORMPOLYPHONIC_HPP
#define PERFORMPOLYPHONIC_HPP

#include <Arduino.h>
#include "PerformModule.hpp"

class PerformPolyphonic : public PerformModule {
	
	public:
		PerformPolyphonic(MidiInterface &midi_interface);
	
	// PerfomModule
	void service();
};

#endif
