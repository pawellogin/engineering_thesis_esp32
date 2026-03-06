#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

struct LedController
{
    bool override = false;
    uint8_t pin;
    bool active = false;
    unsigned long duration = 0;
    unsigned long previousMillis = 0;
};

struct LedStripController
{
    Adafruit_NeoPixel *strip;

    bool active = false;
    bool override = false;

    uint32_t color = 0;

    unsigned long duration = 0;
    unsigned long previousMillis = 0;
};

struct ColorRGB
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

enum class ColorEnum : uint8_t
{
    PINK = 0,
    ORANGE,
    CYAN,
    YELLOW,
    RED,
    GREEN,
    BLUE,
    WHITE,
    PURPLE,
    TEAL,

    COLOR_COUNT
};

extern LedStripController ledStrip;

extern LedController builtInLed;
extern LedController buttonLed;

void gatewayLedInitAll();
void clientLedInitAll();

void ledInit(LedController &led, uint8_t pin, bool inverse = false);

void ledBlink(LedController &led, unsigned long durationMs = 200, bool inverse = false);
void ledHandle(LedController &led, bool inverse = false);

void ledTurnOn(LedController &led, bool inverse = false);
void ledTurnOff(LedController &led, bool inverse = false);

void ledStripInit();

void ledStripBlink(ColorEnum color, unsigned long durationMs = 200);
void ledStripTurnOn(ColorEnum color);
void ledStripTurnOff();

void ledStripHandle();

void ledStripTask(void *p);

void gatewayLedTask(void *p);
void clientLedTask(void *p);

const ColorRGB &getColor(ColorEnum c);

constexpr ColorRGB COLOR_PALETTE[(size_t)ColorEnum::COLOR_COUNT] =
    {
        {255, 71, 249},  // PINK
        {121, 25, 0},    // ORANGE
        {0, 255, 255},   // CYAN
        {255, 255, 0},   // YELLOW
        {255, 0, 0},     // RED
        {0, 255, 0},     // GREEN
        {0, 0, 255},     // BLUE
        {255, 255, 255}, // WHITE
        {101, 0, 127},   // PURPLE
        {0, 255, 128}    // TEAL
};
