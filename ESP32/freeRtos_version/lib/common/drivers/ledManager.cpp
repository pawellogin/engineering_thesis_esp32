#include "drivers/ledManager.h"
#include "core/config.h"
#include "core/debug.h"

static SemaphoreHandle_t ledMutex;
static SemaphoreHandle_t ledStripMutex;

static Adafruit_NeoPixel stripInstance(LED_STRIP_PIXELS, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);

LedController builtInLed;
LedController buttonLed;

LedStripController ledStrip;

static uint32_t paletteToNeoPixel(ColorEnum color)
{
    const ColorRGB &c = COLOR_PALETTE[(uint8_t)color];
    return ledStrip.strip->Color(c.r, c.g, c.b);
}

static void stripApplyColor(uint32_t color)
{
    for (int i = 0; i < ledStrip.strip->numPixels(); i++)
        ledStrip.strip->setPixelColor(i, color);

    ledStrip.strip->show();
}

void ledStripInit()
{
    ledStripMutex = xSemaphoreCreateMutex();
    configASSERT(ledStripMutex);

    ledStrip.strip = &stripInstance;

    stripInstance.begin();
    stripInstance.clear();
    stripInstance.show();
}

void ledStripTurnOn(ColorEnum color)
{
    xSemaphoreTake(ledStripMutex, portMAX_DELAY);

    ledStrip.override = true;
    ledStrip.active = true;
    ledStrip.color = paletteToNeoPixel(color);

    stripApplyColor(paletteToNeoPixel(color));

    xSemaphoreGive(ledStripMutex);
}

void ledStripTurnOff()
{
    xSemaphoreTake(ledStripMutex, portMAX_DELAY);

    ledStrip.override = false;
    ledStrip.active = false;

    stripApplyColor(0);

    xSemaphoreGive(ledStripMutex);
}

void ledStripBlink(ColorEnum color, unsigned long durationMs)
{
    xSemaphoreTake(ledStripMutex, portMAX_DELAY);

    ledStrip.color = paletteToNeoPixel(color);
    ledStrip.duration = durationMs;
    ledStrip.previousMillis = millis();
    ledStrip.active = true;

    stripApplyColor(paletteToNeoPixel(color));

    xSemaphoreGive(ledStripMutex);
}

void ledStripHandle()
{
    if (ledStrip.active &&
        ledStrip.override == false &&
        millis() - ledStrip.previousMillis >= ledStrip.duration)
    {
        ledStrip.active = false;
        stripApplyColor(0);
    }
}

void ledStripTask(void *p)
{
    while (true)
    {
        xSemaphoreTake(ledStripMutex, portMAX_DELAY);
        ledStripHandle();
        xSemaphoreGive(ledStripMutex);

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

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