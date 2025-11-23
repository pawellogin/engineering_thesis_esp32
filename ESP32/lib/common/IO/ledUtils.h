#pragma once
#include <Arduino.h>
#include "config.h"

#include <Adafruit_NeoPixel.h>
#include <stdint.h>

struct RGB
{
    uint8_t r, g, b;
};

enum ColorEnum
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
    TEAL
};

struct LedStripController
{
    Adafruit_NeoPixel *strip;
    RGB palette[9];
    ColorEnum currentColor;
    unsigned long previousMillis;
    unsigned long interval; // for animations
    uint16_t animIndex;
    bool animActive;
};

extern LedStripController ledStrip;

ColorEnum getDefaultColorForBoard(uint8_t boardId);
void ledStripInit(LedStripController &ledCtrl);
void ledStripShowColor(LedStripController &ledCtrl, ColorEnum color, uint8_t brightness = 100);
void ledStripClear(LedStripController &ledCtrl);
void ledStripAnimStart(LedStripController &ledCtrl, ColorEnum color, unsigned long intervalMs);
void ledStripAnimHandle(LedStripController &ledCtrl);

struct LedController
{
    uint8_t pin;
    bool active = false;
    unsigned long duration = 0;
    unsigned long previousMillis = 0;
};

extern LedController builtInLed;
extern LedController buttonLed;

void ledInit(LedController &led, uint8_t pin);
void ledBlink(LedController &led, unsigned long durationMs = 200);
void ledHandle(LedController &led);
