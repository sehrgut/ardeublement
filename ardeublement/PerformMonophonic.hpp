#ifndef PERFORMMONOPHONIC_HPP
#define PERFORMMONOPHONIC_HPP

#include <Arduino.h>
#include "PerformModule.hpp"

class PerformMonophonic : public PerformModule {
	
	public:
		PerformMonophonic(MidiInterface &midi_interface);
	
	// PerfomModule
	void service();
};

/*

class PerformModule : public MidiClockWatcher {
  private:
    midiclock_ticks_t ticks;
    MidiInterface& MIDI;
    ComposeModule* compose;

    volatile ActionQueue actions[NUM_VOICES];
    volatile Voice voices[NUM_VOICES];

    void PerformModule::sendVoice();

  public:
    PerformModule(MidiInterface& midi_interface);
    void service();

  //Module
    void init(Params p);
    void set(Params p);

  //MidiClockWatcher
    void tick(midiclock_ticks_t ticks);
    void stop();
};
*/

#endif
