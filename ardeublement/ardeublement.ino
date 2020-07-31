#include <MIDI.h>
#include "Params.hpp"
#include "Logger.hpp"
#include "Drunk.hpp"
#include "SerialConsole.hpp"

// todo: interrupts instead of delays

MIDI_CREATE_DEFAULT_INSTANCE();

#define CH_IN 2
#define CH_OUT 1
#define PIN_SEED 0

static Drunk *d;
static SerialConsole *console;

static int deviation = 4;
static int center = 60;
static bool dirty = false;


void setup()
{
    d = new Drunk(center);
    d->setStep(deviation);
    
    console = new SerialConsole();

    pinMode(LED_BUILTIN, OUTPUT);
    randomSeed(analogRead(PIN_SEED));
    MIDI.begin(CH_IN);
    MIDI.sendControlChange(midi::MidiControlChangeNumber::AllNotesOff, 0, CH_OUT);
}


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
  Logger::log("[update_params]: looking for dirty params\n");
  if (dirty == true) {
    Logger::log("[update_params]: params found dirty\n");
    d->setStep(deviation);
    dirty = false;
  }
}

void loop() {

  console->process_commands();
  update_params();
  
  digitalWrite(LED_BUILTIN, HIGH);
  int n = d->next();
  Logger::log("Next note: %d\n", n);
  arp(n, 2900, 3000, 1000);
  //play(n, 300, 50);
  digitalWrite(LED_BUILTIN, LOW);
}
