#ifndef IPERFORMER_HPP
#define IPERFORMER_HPP

#include "MidiClock.hpp"

struct IPerformer : public MidiClockWatcher {
	virtual void service() = 0;
	
	// IModule
	virtual void init(Params p) = 0;
	virtual void set(Params p) = 0;
};


#endif
