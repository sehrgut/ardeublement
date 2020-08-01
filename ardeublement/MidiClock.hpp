#ifndef MIDICLOCK_HPP
#define MIDICLOCK_HPP

#define MIDICLOCK_NUM_WATCHERS 16
#define MIDICLOCK_INTERRUPT_MICROS 100

class MidiClockWatcher {
  public:
    virtual void tick(unsigned long ticks) = 0;
    virtual void stop() = 0;
};

class MidiClock {
  private:
    unsigned long prev_us;
    unsigned long cur_us;
    unsigned long ticks;
    bool running;
    
    MidiClockWatcher *watchers[MIDICLOCK_NUM_WATCHERS];
    int watchers_count;

    MidiClock();
    ~MidiClock();
    
  public:
    int bpm = 120;
    volatile float midInterval;

    void start();
    void stop();

    static MidiClock& instance();
    static void tick();
    bool watch(MidiClockWatcher *obj);
};

#endif
