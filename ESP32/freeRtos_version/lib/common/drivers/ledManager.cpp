#include "drivers/ledManager.h"
#include "core/config.h"
#include "core/debug.h"

SemaphoreHandle_t ledMutex;
LedController builtInLed;
LedController buttonLed;

void ledInit(LedController &led, uint8_t pin)
{
    led.pin = pin;
    led.active = false;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void ledInitAll()
{
    ledMutex = xSemaphoreCreateMutex();
    configASSERT(ledMutex);

    ledInit(builtInLed, BUILTIN_LED);
    // ledInit(buttonLed, BUTTON_LED_PIN);
}

void ledBlink(LedController &led, unsigned long durationMs)
{
    xSemaphoreTake(ledMutex, portMAX_DELAY);
    led.duration = durationMs;
    led.previousMillis = millis();
    led.active = true;
    digitalWrite(led.pin, HIGH);
    xSemaphoreGive(ledMutex);
}

void ledHandle(LedController &led)
{
    if (led.active && millis() - led.previousMillis >= led.duration)
    {
        led.active = false;
        digitalWrite(led.pin, LOW);
    }
}

void ledTask(void *p)
{
    while (true)
    {
        xSemaphoreTake(ledMutex, portMAX_DELAY);
        ledHandle(builtInLed);
        xSemaphoreGive(ledMutex);

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}