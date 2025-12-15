#pragma once
#include <Arduino.h>

struct LedController
{
    uint8_t pin;
    bool active = false;
    unsigned long duration = 0;
    unsigned long previousMillis = 0;
};

extern LedController builtInLed;
extern LedController buttonLed;

void ledInitAll();
void ledBlink(LedController &led, unsigned long durationMs = 200);
void ledHandle(LedController &led);

void ledTask(void *p);