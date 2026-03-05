#pragma once
#include <Arduino.h>

struct LedController
{
    bool override = false;
    uint8_t pin;
    bool active = false;
    unsigned long duration = 0;
    unsigned long previousMillis = 0;
};

extern LedController builtInLed;
extern LedController buttonLed;

void gatewayLedInitAll();
void clientLedInitAll();

void ledInit(LedController &led, uint8_t pin, bool inverse = false);

void ledBlink(LedController &led, unsigned long durationMs = 200, bool inverse = false);
void ledHandle(LedController &led, bool inverse = false);

void ledTurnOn(LedController &led, bool inverse = false);
void ledTurnOff(LedController &led, bool inverse = false);

void gatewayLedTask(void *p);
void clientLedTask(void *p);
