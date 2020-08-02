#ifndef PERFORMMODULE_HPP
#define PERFORMMODULE_HPP

#include <arduino.h>
#include <CircularBuffer.h>
#include "ComposeModule.hpp"
#include "MidiClock.hpp"
#include "Params.hpp"

#define CH_OUT 1 //todo: channel(s) set up by main

// todo: separation of concerns between service() low-priority and tick() high-priority code.
//       note calculation should be done in service() and made available to tick()

// todo: https://github.com/FortySevenEffects/arduino_midi_library/issues/162
typedef midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> MidiInterface;

typedef enum {
  Rest = 0,
  Note = 1
} VoiceMode;

typedef struct {
  VoiceMode mode;
  byte      note;
  byte      vel;
  int       dur;  
} VoiceAction;

typedef struct {
  byte        chan;
  VoiceAction action;
  bool        playing;
  midiclock_ticks_t expires;
} Voice;

class PerformModule : public MidiClockWatcher {
  private:
    midiclock_ticks_t ticks;
    MidiInterface& MIDI;
    ComposeModule* compose;

    CircularBuffer<VoiceAction, 4> actions;
    volatile Voice voice = { 0 };

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

#endif
