#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

enum class Button : byte
{
    A = 0,
    B = 1,
    X = 2,
    Count = 3,
    None = 255,
};

enum class State
{
    Idle,
    Counting,
    Pause,
    Alert
};

static constexpr byte LedPin = 0;
static constexpr byte SoundPin = 1;
static constexpr byte ButtonPin[3] = {2, 3, 4};

struct LedRange
{
    byte offset;
    byte length;
};

static constexpr LedRange ButtonRange[(int)Button::Count] = {{5, 3}, {2, 3}, {0, 2}};
static constexpr LedRange StripRange = {8, 12};
static constexpr byte LedCount = StripRange.offset + StripRange.length;

static constexpr byte MaxBrightness = 120;
static constexpr uint32_t Black = Adafruit_NeoPixel::Color(0, 0, 0);
static constexpr uint32_t Blue = Adafruit_NeoPixel::Color(0, 0, MaxBrightness);
static constexpr uint32_t Green = Adafruit_NeoPixel::Color(0, MaxBrightness, 0);
static constexpr uint32_t Yellow = Adafruit_NeoPixel::Color(MaxBrightness, MaxBrightness, 0);
static constexpr uint32_t Red = Adafruit_NeoPixel::Color(MaxBrightness, 0, 0);
static constexpr short RedHue = 0;
static constexpr short YellowHue = 10922;
static constexpr short GreenHue = 21845;

static constexpr unsigned long ChooseTimeMs = 1 * 60000;
static constexpr unsigned long ExtendTimeMs = 2 * 60000;

Adafruit_NeoPixel pixels(LedCount, LedPin, NEO_GRB + NEO_KHZ800);

inline void setButtonColor(Button button, uint32_t color)
{
    pixels.fill(color, ButtonRange[(int)button].offset, ButtonRange[(int)button].length);
}

void setProgress(unsigned long value, unsigned long maxValue, short activeHue, byte activeBrightness, short passiveHue, byte passiveBrightness)
{
    auto V = value * StripRange.length;
    auto active = V / maxValue;
    pixels.fill(Adafruit_NeoPixel::ColorHSV(activeHue, 255, activeBrightness), StripRange.offset + StripRange.length - active, active);
    if (active < 12)
    {
        auto intermediateHue = map(V % maxValue, 0, maxValue - 1, passiveHue, activeHue);
        auto intermediateBrightness = map(V % maxValue, 0, maxValue - 1, passiveBrightness, activeBrightness);
        pixels.setPixelColor(StripRange.offset + StripRange.length - active - 1, Adafruit_NeoPixel::ColorHSV(intermediateHue, 255, intermediateBrightness));
        if (active < 11)
        {
            pixels.fill(Adafruit_NeoPixel::ColorHSV(passiveHue, 255, passiveBrightness), StripRange.offset, StripRange.length - active - 1);
        }
    }
}

// check for press of a button, but do not refire until button was released and pressed again
Button checkButtonEvent()
{
    static bool wasDown[(byte)Button::Count] = {false, false, false};
    for (auto i = 0; i < (byte)Button::Count; ++i)
    {
        if (digitalRead(ButtonPin[i]) == LOW)
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

unsigned long g_timerStart = 0;
unsigned long g_timerMax = 0;
State g_state = State::Idle;

void enterState(State newState)
{
    switch (newState)
    {
    case State::Idle:
        setButtonColor(Button::A, Blue);
        setButtonColor(Button::B, Black);
        setButtonColor(Button::X, Black);
        pixels.fill(Black, StripRange.offset, StripRange.length);
        break;
    case State::Counting:
        setButtonColor(Button::A, Green);
        setButtonColor(Button::B, Black);
        setButtonColor(Button::X, Black);
        pixels.fill(Green, StripRange.offset, StripRange.length);
        g_timerStart = millis();
        g_timerMax = g_timerStart + ChooseTimeMs;
        break;
    case State::Pause:
        break;
    case State::Alert:
        pixels.fill(Red, 0, LedCount);
        break;
    }
    g_state = newState;
}

void setup()
{
    pixels.begin();
    pinMode(ButtonPin[(byte)Button::A], INPUT_PULLUP);
    pinMode(ButtonPin[(byte)Button::B], INPUT_PULLUP);
    pinMode(ButtonPin[(byte)Button::X], INPUT_PULLUP);
    pixels.clear();
    enterState(State::Idle);
}

void loop()
{
    auto b = checkButtonEvent();
    auto curTime = millis();
    switch (g_state)
    {
    case State::Idle:
        if (b == Button::A)
        {
            enterState(State::Counting);
        }
        break;

    case State::Counting:
        if (b == Button::A)
        {
            enterState(State::Counting);
        }

        if (curTime >= g_timerMax)
        {
            enterState(State::Alert);
        }
        else
        {
            auto length = g_timerMax - g_timerStart;
            auto cur = curTime - g_timerStart;
            auto sectionLength = length / 3;
            auto phase = 0;
            while (cur > sectionLength)
            {
                phase++;
                cur -= sectionLength;
            }
            if (phase == 0)
            {
                setProgress(
                    cur, sectionLength,
                    YellowHue, MaxBrightness,
                    GreenHue, MaxBrightness);
            }
            else if (phase == 1)
            {
                setProgress(
                    cur, sectionLength,
                    RedHue, MaxBrightness,
                    YellowHue, MaxBrightness);
            }
            else
            {
                setProgress(
                    cur, sectionLength,
                    RedHue, 0,
                    RedHue, MaxBrightness);
            }
        }
        break;

    case State::Pause:
        break;

    case State::Alert:
        pixels.fill(Adafruit_NeoPixel::ColorHSV(RedHue, 255, Adafruit_NeoPixel::sine8(curTime)), 0, LedCount);

        if (b == Button::A)
        {
            enterState(State::Idle);
        }
        break;
    }

    pixels.show();
}
