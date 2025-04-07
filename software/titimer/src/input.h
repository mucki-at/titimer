#include <Arduino.h>

class Input
{
public:
    enum Event
    {
        Nothing,
        A,
        B,
        X,
        Reset
    };

private:
    enum State
    {
        Idle,
        ADown,
        BDown,
        XDown,
        Error
    };

    uint8_t pinA, pinB, pinX;
    State state;
    unsigned long xDownStart;

public:
    Input(uint8_t pinA, uint8_t pinB, uint8_t pinX);
    Event update(unsigned long now = millis());
};