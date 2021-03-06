# ardeublement

ardeublement is:

1. a generative music sequencer
2. an algorithmic composition engine
3. an Arduino-based furniture music installation

## Theory of Operation

1. Arduino provides a limited set of parameters to the currently-loaded composer and performer modules.
2. The composer module generates pitches as requested by the performer module.
3. The performer module sends MIDI messages representing the performance.

## History

Erik Satie coined the term "musique d'ameublement", or "furniture music" for his minimalist compositions meant to remain in the background. This idea became a source of inspiration for both modern ambient music, as well as the intentionally-manipulative "muzak".

## Goals

- **Modular.** A lot of composition and performance modes would be fun to have, so easily compiling a limited subset to fit on any particular board would be useful. Especially modules like `SerialConsole` that have a lot of character data that won't be used in a running installation.
- **High-Level.** The parameters settable from hardware should be high-level, musical parameters, like degree of tonality, structural self-correlation, pitch range, and tempo. Modules should handle interpretation of these parameters so that to control an Ardeublement installation, a non-technical user simply needs to adjust these large musical ideas without knowing the details of the mode they've selected.
- **Polyphonic.** While the first target is good monophonic lines, polyphony and harmonization are eventual goals.
- **Multitimbral.** Composition and performance modes should be able to generate related instrumental parts on separate MIDI channels. An end user can then assign these channels to different voices. Performer modules may document the general voice categories for each of their channels, but an end user should be able to find pleasing voices on any multitimbral synth simply by exploration.

## Interface
Ardeublement provides a number of parameters which can be interpreted however the module desires:

- `deviation`: a full-range signed int, representing the perceived excursion per step
- `center`: a MIDI note number, representing the key, tonal center, or starting pitch

## Module System
Composition and performance modes should be pluggable. I studied the module systems used in Ornament and Crime, and Equation Composer. o_C's module system seemed more than I needed for this, so I went with a system similar to Equation Composer. To register a module, you need to include its header, add an instance of it to the registry, and update the registry count.

I would like to get away from having all modules loaded in memory, but that's a later engineering project.

## MIDI Clock Resolution
The clock resolution of other projects I've seen is 100us, which provides minimal jitter, but can have diminishing returns. For instance, maintaining a clock of that resolution means that ticks can occur with very little jitter, but with diminishing returns. If so much work is being spent on maintaining a high-resolution clock, less work is available for the underlying performance, resulting in missed notes.

If a low-jitter clock is really needed, it should probably come from outside. Implementing an external MIDI sync mode should be simpler than attempting to make a single Arduino handle both generative music and high-resolution clock maintenance.

Thus, setting the resolution to 1ms.

## Acknowledgements
- MidiClock is heavily based on the implementation by [mortonkopf's Teensy-Midi-Euclidean-Sequencer][teensy_seq]
- FastRand implements Marsaglia's xorshift with constants suggested by [John Metcalf][xorshift_z80].

[teensy_seq]: https://github.com/mortonkopf/Teensy-Midi-Euclidean-Sequencer
[xorshift_z80]: http://www.retroprogramming.com/2017/07/xorshift-pseudorandom-numbers-in-z80.html

