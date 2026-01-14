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

void gatewayLedInitAll()
{
    ledMutex = xSemaphoreCreateMutex();
    configASSERT(ledMutex);

    ledInit(builtInLed, BUILTIN_LED);
    // ledInit(buttonLed, BUTTON_LED_PIN);
}

void clientLedInitAll()
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

        vTaskDelay(10 / portTICK_PERIOD_MS); // small delay to yield CPU
    }
}

void clientLedTask(void *p)
{
    while (true)
    {
        xSemaphoreTake(ledMutex, portMAX_DELAY);
        ledHandle(builtInLed);
        xSemaphoreGive(ledMutex);

        vTaskDelay(10 / portTICK_PERIOD_MS); // small delay to yield CPU
    }
}