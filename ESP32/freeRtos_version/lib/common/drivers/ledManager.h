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

extern SemaphoreHandle_t gatewayLedMutex;
extern SemaphoreHandle_t clientLedMutex;

void gatewayLedInitAll();
void clientLedInitAll();

void ledBlink(LedController &led, SemaphoreHandle_t mutex, unsigned long durationMs = 200);
void ledHandle(LedController &led);

void gatewayLedTask(void *p);
void clientLedTask(void *p);
