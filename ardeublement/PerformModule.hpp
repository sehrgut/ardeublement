#ifndef PERFORMMODULE_HPP
#define PERFORMMODULE_HPP

#include <arduino.h>
#include "MidiClock.hpp"

// todo: https://github.com/FortySevenEffects/arduino_midi_library/issues/162
typedef midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> MidiInterface;

class PerformModule : public MidiClockWatcher {
  private:
    unsigned long prevticks;
    unsigned long curticks;
    MidiInterface& MIDI;

    void PerformModule::sendVoice1();

  public:
    PerformModule(MidiInterface& midi_interface);
    void service();

  //MidiClockWatcher
    void tick(unsigned long ticks);
    void stop();
};

#endif
