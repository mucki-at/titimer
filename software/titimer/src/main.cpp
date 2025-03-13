#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

static constexpr byte LedPin = 0;
static constexpr byte SoundPin = 1;
static constexpr byte ButtonAPin = 2;
static constexpr byte ButtonBPin = 3;
static constexpr byte ButtonXPin = 4;

static constexpr byte ButtonALed = 0;
static constexpr byte ButtonBLed = 1;
static constexpr byte ButtonXLed = 2;
static constexpr byte LedStripBase = 3;
static constexpr byte LedStripLength = 10;

Adafruit_NeoPixel pixels(LedStripBase + LedStripLength, LedPin, NEO_GRB + NEO_KHZ800);

void setup()
{
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
#endif

    pixels.begin();
    pinMode(SoundPin, OUTPUT);
    pinMode(ButtonAPin, INPUT_PULLUP);
    pinMode(ButtonBPin, INPUT_PULLUP);
    pinMode(ButtonXPin, INPUT_PULLUP);
    pixels.clear();
}

void loop()
{
    if (digitalRead(ButtonAPin) == LOW)
    {
        pixels.setPixelColor(ButtonALed, 0, 128, 0);
        digitalWrite(SoundPin, HIGH);
    }
    else
    {
        pixels.setPixelColor(ButtonALed, 0, 0, 128);
        digitalWrite(SoundPin, LOW);
    }

    if (digitalRead(ButtonBPin) == LOW)
    {
        pixels.setPixelColor(ButtonBLed, 128, 128, 0);
    }
    else
    {
        pixels.setPixelColor(ButtonBLed, 0, 0, 128);
    }

    if (digitalRead(ButtonXPin) == LOW)
    {
        pixels.setPixelColor(ButtonXLed, 128, 0, 0);
    }
    else
    {
        pixels.setPixelColor(ButtonXLed, 0, 0, 128);
    }

    int t = millis() / 4;
    int phase = pixels.sine8(t);
    pixels.fill(phase, LedStripBase, LedStripLength);

    while (!pixels.canShow())
    {
        delayMicroseconds(100);
    }
    pixels.show();
}