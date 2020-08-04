#ifndef PERFORMMODULE_HPP
#define PERFORMMODULE_HPP

#include <arduino.h>
#include <CircularBuffer.h>
#include <MIDI.h>
#include "ComposeModule.hpp"
#include "MidiClock.hpp"
#include "Params.hpp"

#define CH_OUT 1 //todo: channel(s) set up by main
#define NUM_VOICES 3 // todo: multiple composers
#define QUEUE_LEN 4

// todo: https://github.com/FortySevenEffects/arduino_midi_library/issues/162
typedef midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> MidiInterface;

enum ClockDivisions {
  WHOLE         = 24 * 4,
  HALF          = 24 * 2,
  QUARTER       = 24,
  EIGHTH        = 24 / 2,
  SIXTEENTH     = 24/4,
  THIRTYSECOND  = 24/8,
};

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

typedef CircularBuffer<VoiceAction, QUEUE_LEN> ActionQueue;

// todo: separation between "voice" and "part"
//       - a Part gets 1 composer, 1 rhythm, and n voices
//       - a Voice gets 1 channel
//       - AbstractPerformer should only care about voice scheduling.

/*
  - class MonophonicPerformer(RhythmModule r, ComposeModule c)
  - class PolyphonicPerformer(RhythmModule r, ComposeModule c, int voices)
  - class MultitimbralPerformer(RhythmModule[] rr, ComposeModule cc[], int voices)
*/
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

    volatile Voice voices[NUM_VOICES];
    void sendVoices();

  protected:
    ComposeModule* compose;
    volatile ActionQueue actions[NUM_VOICES];

  public:
    PerformModule(MidiInterface& midi_interface);
    virtual void service() = 0;

  //Module
    void init(Params p);
    void set(Params p);

  //MidiClockWatcher
    void tick(midiclock_ticks_t ticks);
    void stop();
};

#endif
