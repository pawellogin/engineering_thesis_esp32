#include "drivers/ledManager.h"
#include "core/config.h"
#include "core/debug.h"

SemaphoreHandle_t ledMutex;

LedController builtInLed;
LedController buttonLed;

void ledInit(LedController &led, uint8_t pin, bool inverse)
{
    led.pin = pin;
    led.active = false;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, inverse ? HIGH : LOW);
}

void gatewayLedInitAll()
{
    ledMutex = xSemaphoreCreateMutex();
    configASSERT(ledMutex);

    ledInit(builtInLed, BUILTIN_LED);
    // ledInit(buttonLed, BUTTON_LED);
}

void clientLedInitAll()
{
    ledMutex = xSemaphoreCreateMutex();
    configASSERT(ledMutex);

    ledInit(builtInLed, BUILTIN_LED);
    ledInit(buttonLed, BUTTON_LED, true);
}

void ledBlink(LedController &led, unsigned long durationMs, bool inverse)
{
    xSemaphoreTake(ledMutex, portMAX_DELAY);
    led.duration = durationMs;
    led.previousMillis = millis();
    led.active = true;
    digitalWrite(led.pin, inverse ? LOW : HIGH);
    xSemaphoreGive(ledMutex);
}

void ledHandle(LedController &led, bool inverse)
{

    if (led.active && millis() - led.previousMillis >= led.duration && led.override == false)
    {
        led.active = false;
        digitalWrite(led.pin, inverse ? HIGH : LOW);
        // LOG_INFO("led turn off");
    }
    else
    {
        // LOG_INFO("active %d, milis %d, prevoous %d, duration %d", led.active, millis(), led.previousMillis, led.duration);
    }
}

void gatewayLedTask(void *p)
{
    while (true)
    {
        xSemaphoreTake(ledMutex, portMAX_DELAY);
        ledHandle(builtInLed);
        xSemaphoreGive(ledMutex);

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void clientLedTask(void *p)
{
    while (true)
    {
        xSemaphoreTake(ledMutex, portMAX_DELAY);
        ledHandle(builtInLed);
        ledHandle(buttonLed, true);
        xSemaphoreGive(ledMutex);

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void ledTurnOn(LedController &led, bool inverse)
{
    xSemaphoreTake(ledMutex, portMAX_DELAY);
    led.previousMillis = millis();
    led.override = true;
    led.active = true;
    digitalWrite(led.pin, inverse ? LOW : HIGH);
    xSemaphoreGive(ledMutex);
}

void ledTurnOff(LedController &led, bool inverse)
{
    xSemaphoreTake(ledMutex, portMAX_DELAY);
    led.previousMillis = millis();
    led.override = false;
    led.active = false;
    digitalWrite(led.pin, inverse ? HIGH : LOW);
    xSemaphoreGive(ledMutex);
}