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

static constexpr int Black = Adafruit_NeoPixel::Color(0, 0, 0);
static constexpr int Blue = Adafruit_NeoPixel::Color(0, 0, 120);
static constexpr int Green = Adafruit_NeoPixel::Color(0, 120, 0);
static constexpr int Yellow = Adafruit_NeoPixel::Color(120, 120, 0);
static constexpr int Red = Adafruit_NeoPixel::Color(120, 0, 0);

Adafruit_NeoPixel pixels(LedStripBase + LedStripLength, LedPin, NEO_GRB + NEO_KHZ800);

void setup()
{
    pixels.begin();
    pinMode(ButtonAPin, INPUT_PULLUP);
    pinMode(ButtonBPin, INPUT_PULLUP);
    pinMode(ButtonXPin, INPUT_PULLUP);
    pixels.clear();
    pixels.setPixelColor(ButtonALed, Blue);
    pixels.setPixelColor(ButtonBLed, Blue);
    pixels.setPixelColor(ButtonXLed, Black);
}

enum class Button
{
    None = -1,
    A = 0,
    B = 1,
    X = 2
};

// check for press of a button, but do not refire until button was released and pressed again
Button checkButtonEvent()
{
    static bool wasDown[3] = {false, false, false};
    static constexpr byte pins[3] = {ButtonAPin, ButtonBPin, ButtonXPin};

    for (auto i = 0; i < 3; ++i)
    {
        if (digitalRead(pins[i]) == LOW)
        {
            if (!wasDown[i])
            {
                wasDown[i] = true;
                return Button(i);
            }
        }
        else
        {
            if (wasDown[i])
            {
                wasDown[i] = false;
            }
        }
    }
    return Button::None;
}

void loop()
{
    auto b = checkButtonEvent();
    if (b == Button::A)
    {
        tone(SoundPin, 220, 100);
    }
    else if (b == Button::B)
    {
        tone(SoundPin, 440, 100);
    }

    auto t = millis() / 10;
    auto phase = Adafruit_NeoPixel::sine8(t);
    pixels.fill(phase, LedStripBase, LedStripLength);
    pixels.show();
}
