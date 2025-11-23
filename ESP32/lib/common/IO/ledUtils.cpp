// ledUtils.cpp
#include "ledUtils.h"

LedController builtInLed;
LedController buttonLed;

void ledInit(LedController &led, uint8_t pin)
{
    led.pin = pin;
    led.active = false;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void ledBlink(LedController &led, unsigned long durationMs)
{
    led.duration = durationMs;
    led.previousMillis = millis();
    led.active = true;
    digitalWrite(led.pin, HIGH);
}

void ledHandle(LedController &led)
{
    if (led.active && millis() - led.previousMillis >= led.duration)
    {
        led.active = false;
        digitalWrite(led.pin, LOW);
    }
}
