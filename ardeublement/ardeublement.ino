#include <MIDI.h>
#include <TimerThree.h>
#include "Params.hpp"
#include "Logger.hpp"
#include "SerialConsole.hpp"
#include "MidiClock.hpp"
#include "Util.hpp"

// todo: everything is a Module subtype, and Module.set(Params) is how all globals get around
// todo: rethink main architecture of composer/startcomposer/etc

MIDI_CREATE_DEFAULT_INSTANCE();

#define LOGGER "main"
#define CH_IN 2
#define CH_OUT 1
#define PIN_SEED A4

// REGISTER PLUGINS
#include "ComposeDrunk.hpp"
#include "ComposeGaussian.hpp"
#include "PerformModule.hpp"

#define NUM_COMPOSERS 2
ComposeModule *composers[] {
  new ComposeDrunk(),
  new ComposeGaussian()
};

#define NUM_PERFORMERS 1
PerformModule *performers[] {
	new PerformModule(MIDI)
};


// Instantiate other modules
static ComposeModule *compose;
static PerformModule* perform;
static SerialConsole console;
static MidiClock& mainclock = MidiClock::instance();

static Params GLOBAL_PARAMS = {
  .deviation = 4,
  .center = 60,
  .bpm = 100,
  .tonality = 0.75,
  .running = false // todo: need running and should_run
};


void start_composer() {
  Logger::log(LOGGER,"(start_composer) starting composer");
  mainclock.start();
  compose->init(GLOBAL_PARAMS);
  perform->init(GLOBAL_PARAMS);
  GLOBAL_PARAMS.running = true;
}

void stop_composer() {
  Logger::log(LOGGER,"(stop_composer) stopping composer");
  mainclock.stop();
  GLOBAL_PARAMS.running = false;
}

void restart_composer() {
  stop_composer();
  start_composer();
}



void update_params() {  
  if (GLOBAL_PARAMS.dirty == true) {
    Logger::log(LOGGER, "(update_params) params found dirty");

    mainclock.bpm = GLOBAL_PARAMS.bpm;
    
    if (GLOBAL_PARAMS.running == false) {
      stop_composer();
    } else /* running==true */ {
      start_composer();
    }
    
    GLOBAL_PARAMS.dirty = false;
  }
}


void setup() {
    compose = composers[1];
    perform = performers[0];
    mainclock.watch(perform);
    delay(1000);
    start_composer();

    pinMode(LED_BUILTIN, OUTPUT);
    
    randomSeed(analogRead(PIN_SEED)); // seeding in case a library ever uses it. We won't.
    FastRand::srand(analogRead(PIN_SEED));
    FastRand::srandom(analogRead(PIN_SEED));
    
    MIDI.begin(CH_IN);
    
    // MIDI Panic in case of hung notes on reset
    // todo: how to MIDI panic with multitimbral performers? some kind of channel registry?
    MIDI.sendControlChange(midi::MidiControlChangeNumber::AllNotesOff, 0, CH_OUT);
    MIDI.sendControlChange(midi::MidiControlChangeNumber::AllSoundOff, 0, CH_OUT);
    
    // for those destinations that don't respect the CCs
    byte i;
    for (i=0; i<128; i++)
    	MIDI.sendNoteOff(i, 0, CH_OUT);
}


void loop() {
  //Logger::log(LOGGER, "loop");
  digitalWrite(LED_BUILTIN, HIGH);
  console.process_commands();
  update_params();
  digitalWrite(LED_BUILTIN, LOW);
  perform->service();
}
