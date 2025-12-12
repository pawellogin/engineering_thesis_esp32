#include "buttonUtils.h"

// Global instance
ButtonController mainButton;

void buttonInit(ButtonController &btn, uint8_t pin, unsigned long debounceMs)
{
    btn.pin = pin;
    btn.currentState = false;
    btn.lastState = false;
    btn.lastDebounceTime = 0;
    btn.debounceDelay = debounceMs;
    pinMode(pin, INPUT_PULLUP);
}

bool buttonWasPressed(ButtonController &btn)
{
    bool reading = !digitalRead(btn.pin); // pressed = LOW
    unsigned long now = millis();

    if (reading != btn.lastState)
    {
        btn.lastDebounceTime = now;
    }

    if ((now - btn.lastDebounceTime) > btn.debounceDelay)
    {
        if (reading != btn.currentState)
        {
            btn.currentState = reading;
            if (btn.currentState)
            { // button pressed
                btn.lastState = reading;
                return true;
            }
        }
    }

    btn.lastState = reading;
    return false;
}
