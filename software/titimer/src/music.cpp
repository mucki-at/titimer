#include "music.h"
#include <Arduino.h>

Music::Music(uint8_t pin) :
    pin(pin),
    begin(nullptr),
    cur(nullptr),
    end(nullptr),
    nextToneTimeMs(Never),
    repeat(false)
{}

void Music::stop()
{
    noTone();
    begin = cur = end = nullptr;
    nextToneTimeMs = Never;
    repeat = false;
}

void Music::play(const Tone* tones, size_t count, bool repeat)
{
    begin = cur = tones;
    end = tones+count;
    this->repeat = repeat;
    nextToneTimeMs = millis();
}

void Music::update(unsigned long now)
{
    if (now < nextToneTimeMs) return;   // nothing to do
    if (cur < begin || cur >= end) return;      // prevent out of range access

    auto duration = (nextToneTimeMs + cur->durationMs)-now;
    tone(pin, cur->hz, duration);
    nextToneTimeMs = nextToneTimeMs + cur->durationMs + cur->pauseMs;
    ++cur;
    if (cur==end)
    {
        if (repeat)
        {
            cur = begin;
        }
        else
        {
            nextToneTimeMs = Never;
        }
    }
}

unsigned long Music::msUntilNextNote(unsigned long now)
{
    if (now > nextToneTimeMs) return 0;
    else return nextToneTimeMs-now;
}

