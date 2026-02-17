#include "drivers/buttonManager.h"
#include "core/debug.h"
#include "network/udp/udpCommands.h"

static constexpr unsigned long BUTTON_DEBOUNCE_MS = 30;
static constexpr unsigned long BUTTON_LONG_PRESS_MS = 800;

SemaphoreHandle_t buttonMutex;

ButtonController clientButton;

void buttonInit(ButtonController &btn, uint8_t pin, bool activeLow)
{
    btn.pin = pin;
    btn.activeLow = activeLow;

    pinMode(pin, activeLow ? INPUT_PULLUP : INPUT);

    btn.rawState = false;
    btn.stableState = false;
    btn.lastChangeMs = 0;
    btn.pressStartMs = 0;
    btn.longSent = false;
    btn.pendingEvent = ButtonEvent::BUTTON_EVENT_NONE;
}

void buttonHandle(ButtonController &btn)
{
    unsigned long now = millis();

    bool read = digitalRead(btn.pin);
    bool pressed = btn.activeLow ? !read : read;

    // raw edge
    if (pressed != btn.rawState)
    {
        btn.rawState = pressed;
        btn.lastChangeMs = now;
    }

    // debounce
    if (now - btn.lastChangeMs < BUTTON_DEBOUNCE_MS)
        return;

    // stable edge
    if (btn.stableState != btn.rawState)
    {
        btn.stableState = btn.rawState;

        // RELEASE = CLICK
        if (!btn.stableState)
        {
            btn.pendingEvent = ButtonEvent::BUTTON_EVENT_CLICK;
            LOG_DEBUG("button click");
        }
    }
}

ButtonEvent buttonPopEvent(ButtonController &btn)
{
    ButtonEvent ev = btn.pendingEvent;
    btn.pendingEvent = ButtonEvent::BUTTON_EVENT_NONE;
    return ev;
}

void buttonTask(void *p)
{
    buttonMutex = xSemaphoreCreateMutex();
    configASSERT(buttonMutex);

    buttonInit(clientButton, MAIN_BUTTON_PIN);

    while (true)
    {
        xSemaphoreTake(buttonMutex, portMAX_DELAY);
        buttonHandle(clientButton);
        xSemaphoreGive(buttonMutex);

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void clientLogicTask(void *p)
{
    while (true)
    {
        ButtonEvent ev;

        xSemaphoreTake(buttonMutex, portMAX_DELAY);
        ev = buttonPopEvent(clientButton);
        xSemaphoreGive(buttonMutex);

        if (ev == ButtonEvent::BUTTON_EVENT_CLICK)
        {
            udpSendButtonEvent(gateway_board_id);
        }

        vTaskDelay(5 / portTICK_PERIOD_MS);
    }
}