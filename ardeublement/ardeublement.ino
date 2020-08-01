#include <MIDI.h>
#include <TimerThree.h>
#include "Params.hpp"
#include "Logger.hpp"
#include "SerialConsole.hpp"
#include "MidiClock.hpp"
#include "PerformModule.hpp"

// todo: port delay-based performers to clock-based
// todo: everything is a Module subtype, and Module.set(Params) is how all globals get around

MIDI_CREATE_DEFAULT_INSTANCE();

#define LOGGER "main"
#define CH_IN 2
#define CH_OUT 1
#define PIN_SEED A0

// REGISTER PLUGINS
#include "Drunk.hpp"
#include "ComposeGaussian.hpp"

#define NUM_COMPOSERS 2
ComposeModule *composers[] {
  new Drunk(),
  new ComposeGaussian()
};




static ComposeModule *compose;
static SerialConsole *console;
static MidiClock& mainclock = MidiClock::instance();
static PerformModule* perform = new PerformModule(MIDI);

static Params GLOBAL_PARAMS = {
  .deviation = 4,
  .center = 60,
  .bpm = 100,
  .tonality = 0.0,
  .running = false // todo: need running and should_run
};


void start_composer() {
  Logger::log(LOGGER,"(start_composer) starting composer");
  mainclock.start();
  compose->init(GLOBAL_PARAMS);
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



// todo: PerformModule abstract class
void arp(int n, int rate, int sustain, int wait) {
  int n1 = n+4 > 127 ? -1 : n+4;
  int n2 = n+7 > 127 ? -1 : n+7;
  
  MIDI.sendNoteOn(n, 127, CH_OUT);
  delay(rate);
  if (n1 >= 0) {
    MIDI.sendNoteOn(n1, 127, CH_OUT);
    delay(rate);
  }

  if (n2 >= 0) {
    MIDI.sendNoteOn(n2, 127, CH_OUT);
    delay(rate);
  }

  delay(sustain);
  
  MIDI.sendNoteOff(n, 0, CH_OUT);
  if (n1 >= 0) MIDI.sendNoteOff(n1, 0, CH_OUT);
  if (n2 >= 0) MIDI.sendNoteOff(n2, 0, CH_OUT);

  delay(wait);
}

void play(int n, int sustain, int wait) {
  MIDI.sendNoteOn(n, 127, CH_OUT);
  delay(sustain);
  MIDI.sendNoteOff(n, 0, CH_OUT);
  delay(wait);
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
    mainclock.watch(perform);
    console = new SerialConsole();
    delay(1000);
    start_composer();

    pinMode(LED_BUILTIN, OUTPUT);
    randomSeed(analogRead(PIN_SEED));
    MIDI.begin(CH_IN);
    MIDI.sendControlChange(midi::MidiControlChangeNumber::AllNotesOff, 0, CH_OUT);
    MIDI.sendControlChange(midi::MidiControlChangeNumber::AllSoundOff, 0, CH_OUT); // todo: panic should maybe send noteoff for 0-127?
}


void loop() {

  console->process_commands();
  update_params();
/*
  if (GLOBAL_PARAMS.running == true) { //todo: performer module
    digitalWrite(LED_BUILTIN, HIGH);
    int n = compose->next();
    Logger::log(LOGGER, "Next note: %d", n);
    //arp(n, 2900, 3000, 1000);
    play(n, 300, 50);
    digitalWrite(LED_BUILTIN, LOW);
  }*/
}
