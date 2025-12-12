#pragma once
#include <Arduino.h>
#include "config.h"

struct ButtonController
{
    uint8_t pin;
    bool currentState;
    bool lastState;
    unsigned long lastDebounceTime;
    unsigned long debounceDelay;
};

extern ButtonController mainButton;

void buttonInit(ButtonController &btn, uint8_t pin, unsigned long debounceMs = 50);

bool buttonWasPressed(ButtonController &btn);
