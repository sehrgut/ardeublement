## TO DO

- EEPROM storage of parameters
- Rotary encoder and switch hardware interface
  - Optional potentiometer interface module
- SD card support, for modes with large amounts of static data
- MIDI clock in/out
- ToneRowComposer
- Euclidean rhythms
- loglevels
- Separate composer classes for poly and mono
- compile-out logging
- Logger::asyncLog to be called from isr to run in loop to avoid disrupting isr
- Efficient print lib

### MIDI scheduler
- priority queue of messages based on their intended tick time?
- abstract "pitch+duration" and "articulation" structs instead, from which messages are derived?
  - getting there, but how to abstract it? What's useful to have on the abstract Performer, vs. in a specific concrete performer implementation?

### MIDI clock resilience
- The highest bpm that I can reach without dropping notes and locking up the serial is about 160 (on a Leonardo).
- This is with resolution of 1000us. I hate to take away resolution further than that.
