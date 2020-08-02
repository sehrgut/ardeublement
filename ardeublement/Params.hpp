#ifndef PARAMS_HPP
#define PARAMS_HPP

// params should be self-similarity, tonality, etc.
/*
 * Range: distance from center, should there be rangelow/rangehigh?
 * Deviation: change per iteration, should be % of range
 * Center: the tonal and physical center of the piece
 * Tonality:
 * Coherence: repetition? self-similarity? no good name for this yet
 */
// things that make sense for any generator

// todo: should there be an enumerated PARAM_SET/PARAM_GET with automatic dirty management?

typedef struct {
  int     deviation;
  byte    center;
  int     bpm;
  double  tonality; // 0-1
  bool    running;
  bool    dirty;
} Params;

extern Params GLOBAL_PARAMS;

#endif
