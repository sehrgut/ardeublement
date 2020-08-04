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
#include "PerformModule.hpp"
#include "PerformMonophonic.hpp"
#include "PerformPolyphonic.hpp"

#define NUM_PERFORMERS 1
PerformModule *performers[] {
	new PerformPolyphonic(MIDI)
};


// Instantiate other modules
static ComposeModule *compose;
static PerformModule* perform;
static SerialConsole console;
static MidiClock& mainclock = MidiClock::instance();

/*  int     deviation; // todo: make double 0-1 % of range
  double  tonality; // 0-1
  double  coherence; // 0-1
  byte    center;
  byte    range; // 0-127, centered on center

*/

static Params GLOBAL_PARAMS = {
  .deviation = 4,
  .tonality = 0.75,
  .coherence = 0.5,
  .center = 60,
  .range = 87,
  .bpm = 100,
  .running = false // todo: need running and should_run
};


void start_composer() {
  Logger::log(LOGGER,"(start_composer) starting composer");
  perform->init(GLOBAL_PARAMS);
  perform->service();
  mainclock.start();
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
    perform = performers[0];
    mainclock.watch(perform);
    delay(1000);
    start_composer();

    pinMode(LED_BUILTIN, OUTPUT);
    
    // Seed the RNGs. Even the libc ones, in case any lib ever uses them. We sha'n't.
    srand(analogRead(PIN_SEED));
    srandom(analogRead(PIN_SEED));
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
/* In case there's first-loop stuff that needs to be done after setup() */
  static bool first_run = true;
  if (first_run) {
    // todo: BUG, something weird with SerialConsole, looks related to:
    // https://github.com/arduino/Arduino/issues/5797, so probably need to burn a new
    // bootloader. Regardless, without this, SerialConsole::process_commands gets
    // stuck on boot.
	Logger::log(LOGGER, "kickstart");
    first_run = false;
  }
  Logger::log(LOGGER, "loop");
  update_params();
  digitalWrite(LED_BUILTIN, HIGH);
  perform->service();
  digitalWrite(LED_BUILTIN, LOW);
  console.process_commands();
}
