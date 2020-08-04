#ifndef PERFORMMODULE_HPP
#define PERFORMMODULE_HPP

#include <arduino.h>
#include <CircularBuffer.h>
#include <MIDI.h>
#include "ComposeModule.hpp"
#include "MidiClock.hpp"
#include "Params.hpp"
#include "IPerformer.hpp"

#define CH_OUT 1 //todo: channel(s) set up by main

/* todo: is this useful to template?
 * Since, it's mostly dependent on how much work is being done and the potential lagtime
 */
#define QUEUE_LEN 4

// todo: https://github.com/FortySevenEffects/arduino_midi_library/issues/162
typedef midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> MidiInterface;

/*
// todo: if these would be useful, put them in another file and extern them here

char* clock_divisions_names[6] ={"1/1","1/2","1/4","1/8","1/16","1/32"};
int divRatio[6] = {96,48,24,12,6,3};//note multipliers
char* actionModeNames[2] = {"Rest","Note"};

*/

enum ClockDivisions {
  WHOLE         = 24 * 4,
  HALF          = 24 * 2,
  QUARTER       = 24,
  EIGHTH        = 24 / 2,
  SIXTEENTH     = 24 / 4,
  THIRTYSECOND  = 24 / 8,
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

typedef struct {
  byte        chan;
  VoiceAction action;
  bool        playing;
  midiclock_ticks_t expires;
} Voice;


typedef CircularBuffer<VoiceAction, QUEUE_LEN> ActionQueue;

template <size_t VOICE_COUNT>
class PerformModule : public IPerformer {
  private:
    midiclock_ticks_t	ticks;
    MidiInterface&		MIDI;
    volatile Voice		voices[VOICE_COUNT];

    void sendVoices();

  protected:
    ComposeModule* compose;
    volatile ActionQueue actions[VOICE_COUNT];

  public:
    PerformModule(MidiInterface& MIDI);
    virtual void service() = 0;

  //Module
    void init(Params p);
    void set(Params p);

  //MidiClockWatcher
    void tick(midiclock_ticks_t ticks);
    void stop();
};

#include "PerformModule-impl.hpp"

#endif
