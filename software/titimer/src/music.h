#include <Arduino.h>

struct Tone
{
    static constexpr short Forever = 65535;

    short hz;         // frequency of tone to play
    short durationMs; // duration of tone
    short pauseMs;    // time to pause after tone
};

class Music
{
private:
    const uint8_t pin;
    const Tone* begin;
    const Tone* cur;
    const Tone* end;
    unsigned long nextToneTimeMs;
    bool repeat;
    static constexpr unsigned long Never = -1;

public:
    Music(uint8_t pin);
    void stop();
    void play(const Tone* tones, size_t count, bool repeat = false);
    template<int N> inline void play(const Tone(&tones)[N], bool repeat = false) { play(tones, N, repeat); }

    void update(unsigned long now = millis());
    unsigned long msUntilNextNote(unsigned long now = millis());
};
