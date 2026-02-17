#pragma once
#include <Arduino.h>

enum class ButtonEvent : uint8_t
{
    BUTTON_EVENT_NONE = 0,
    BUTTON_EVENT_CLICK,
    BUTTON_EVENT_LONG_PRESS
};

struct ButtonController
{
    uint8_t pin;

    bool activeLow = true;

    bool rawState = false;
    bool stableState = false;

    unsigned long lastChangeMs = 0;
    unsigned long pressStartMs = 0;

    bool longSent = false;

    ButtonEvent pendingEvent = ButtonEvent::BUTTON_EVENT_NONE;
};

extern ButtonController clientButton;

void buttonInit(ButtonController &btn, uint8_t pin, bool activeLow = true);
void buttonHandle(ButtonController &btn);
ButtonEvent buttonPopEvent(ButtonController &btn);

void buttonTask(void *p);
void clientLogicTask(void *p);
