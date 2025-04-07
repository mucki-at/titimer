#include "input.h"

Input::Input(uint8_t pinA, uint8_t pinB, uint8_t pinX) :
    pinA(pinA),
    pinB(pinB),
    pinX(pinX),
    state(Error),
    xDownStart(0)
{}

Input::Event Input::update(unsigned long now)
{
    switch (state)
    {
    case Idle:
        if (digitalRead(pinA) == LOW)
        {
            state = ADown;
        }
        else if (digitalRead(pinB) == LOW)
        {
            state = BDown;
        }
        else if (digitalRead(pinX) == LOW)
        {
            state = XDown;
            xDownStart = now;
        }
        break;

    case ADown:
        if (digitalRead(pinA) == HIGH)
        {
            state = Idle;
            return A;
        }
        else if (digitalRead(pinB) == LOW)
        {
            state = Error;
        }
        else if (digitalRead(pinX) == LOW)
        {
            state = Error;
        }
        break;

    case BDown:
        if (digitalRead(pinA) == LOW)
        {
            state = Error;
        }
        else if (digitalRead(pinB) == HIGH)
        {
            state = Idle;
            return B;
        }
        else if (digitalRead(pinX) == LOW)
        {
            state = Error;
        }
        break;

    case XDown:
        if (digitalRead(pinA) == LOW)
        {
            state = Error;
        }
        else if (digitalRead(pinB) == LOW)
        {
            state = Error;
        }
        else if (digitalRead(pinX) == HIGH)
        {
            state = Idle;
            return X;
        }
        else if (digitalRead(pinX) == LOW)
        {
            if (now - xDownStart > 1000)
            {
                state = Error;
                return Reset;
            }
        }
        break;

    case Error:
        if ((digitalRead(pinA) == HIGH) &&
            (digitalRead(pinB) == HIGH)&&
            (digitalRead(pinX) == HIGH))
        {
            state = Idle;
        }
        break;
    }
    return Nothing;
}