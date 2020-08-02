#ifndef MIDICLOCK_HPP
#define MIDICLOCK_HPP

#define MIDICLOCK_NUM_WATCHERS 16
#define MIDICLOCK_RESOLUTION_MICROS 1000

typedef unsigned int midiclock_ticks_t;

class MidiClockWatcher {
  public:
    virtual void tick(midiclock_ticks_t ticks) = 0;
    virtual void stop() = 0;
};

class MidiClock {
  private:
    unsigned long prev_us;
    unsigned long cur_us;
    midiclock_ticks_t ticks;
    bool running;
    
    MidiClockWatcher *watchers[MIDICLOCK_NUM_WATCHERS];
    int watchers_count;

    MidiClock();
    ~MidiClock();
    
  public:
    int bpm = 120;
    volatile float pulse_interval;

    void start();
    void stop();

    static MidiClock& instance();
    static void tick();
    bool watch(MidiClockWatcher *obj);
};

#endif
