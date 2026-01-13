#include "drivers/ledManager.h"
#include "core/config.h"
#include "core/debug.h"

SemaphoreHandle_t gatewayLedMutex;
SemaphoreHandle_t clientLedMutex;
LedController builtInLed;
LedController buttonLed;

void ledInit(LedController &led, uint8_t pin)
{
    led.pin = pin;
    led.active = false;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void gatewayLedInitAll()
{
    gatewayLedMutex = xSemaphoreCreateMutex();
    configASSERT(gatewayLedMutex);

    ledInit(builtInLed, BUILTIN_LED);
    // ledInit(buttonLed, BUTTON_LED_PIN);
}

void clientLedInitAll()
{
    clientLedMutex = xSemaphoreCreateMutex();
    configASSERT(clientLedMutex);

    ledInit(builtInLed, BUILTIN_LED);
    // ledInit(buttonLed, BUTTON_LED_PIN);
}

void ledBlink(LedController &led, SemaphoreHandle_t mutex, unsigned long durationMs)
{
    xSemaphoreTake(mutex, portMAX_DELAY);
    led.duration = durationMs;
    led.previousMillis = millis();
    led.active = true;
    digitalWrite(led.pin, HIGH);
    xSemaphoreGive(mutex);
}

void ledHandle(LedController &led)
{

    if (led.active && millis() - led.previousMillis >= led.duration)
    {
        led.active = false;
        digitalWrite(led.pin, LOW);
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
        xSemaphoreTake(gatewayLedMutex, portMAX_DELAY);
        ledHandle(builtInLed);
        xSemaphoreGive(gatewayLedMutex);

        vTaskDelay(10 / portTICK_PERIOD_MS); // small delay to yield CPU
    }
}

void clientLedTask(void *p)
{
    while (true)
    {
        xSemaphoreTake(clientLedMutex, portMAX_DELAY);
        ledHandle(builtInLed);
        xSemaphoreGive(clientLedMutex);

        vTaskDelay(10 / portTICK_PERIOD_MS); // small delay to yield CPU
    }
}