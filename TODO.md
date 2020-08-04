## TO DO

## Priority Triage
- Better logging. Needs to be compile-out, multiple loggers, efficient output, separate loglevel per logger, compile-out by loglevel
	- Logger::asyncLog to be called from isr to run in loop to avoid disrupting isr
- Efficient print lib
- Finish nailing down global musicality parameters
  - Range, Deviation, Tonality, Coherence?

### Scale/Temperament
- Replace array rotate in Scale with offset
- Support Scala tunings
  - qv http://www.music.chrblee.net/tuningaid/

### MIDI scheduler
- priority queue of messages based on their intended tick time?
- abstract "pitch+duration" and "articulation" structs instead, from which messages are derived?
  - getting there, but how to abstract it? What's useful to have on the abstract Performer, vs. in a specific concrete performer implementation?

### MIDI clock resilience
- The highest bpm that I can reach without dropping notes and locking up the serial is about 160 (on a Leonardo).
- This is with resolution of 1000us. I hate to take away resolution further than that.
- polyphonic composition doesn't seem to play all notes at once

## To Triage
- Doxygen

### RNG
- Tune RNG scaling for uniformity, qv https://lemire.me/blog/2017/02/28/how-many-floating-point-numbers-are-in-the-interval-01/
- 8-bit fastrand? https://github.com/edrosten/8bit_rng

## Core Featureset
- EEPROM storage of parameters
- Rotary encoder and switch hardware interface
  - Optional potentiometer interface module
- ToneRowComposer
- Euclidean rhythms
- Markov (reads transition tables from SD)
- Separate composer classes for poly and mono
- Multitimbral
  - Performers can use this for multiple voice parts, or more precise articulation

## Expanded Featureset
- SD card support, for modes with large amounts of static data
- MIDI clock in/out
- SP-MIDI to support a variety of output devices
  - https://www.midi.org/specifications-old/item/scalable-polyphony-midi-sp-midi
- Goldberg: a module that accepts a melody and improvises variations (from SD card)
- Parsons: Generates melodies for Parsons codes (from SD card)