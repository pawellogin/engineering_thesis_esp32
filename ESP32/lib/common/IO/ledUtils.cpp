// ledUtils.cpp
#include "ledUtils.h"

Adafruit_NeoPixel WS2812B(LED_STRIP_PIXELS, 18, NEO_GRB + NEO_KHZ800);

LedStripController ledStrip;

ColorEnum getDefaultColorForBoard(uint8_t boardId)
{
    switch (boardId)
    {
    case 2:
        return RED;
    case 3:
        return GREEN;
    case 4:
        return BLUE;
    case 5:
        return YELLOW;
    case 6:
        return ORANGE;
    case 7:
        return CYAN;
    case 8:
        return PURPLE;
    case 9:
        return PINK;
    case 10:
        return TEAL;
    default:
        return WHITE;
    }
}

void ledStripInit(LedStripController &ledCtrl)
{
    ledCtrl.strip = &WS2812B;
    ledCtrl.strip->begin();
    ledCtrl.strip->clear();
    ledCtrl.strip->show();

    ledCtrl.palette[PINK] = {255, 71, 249};
    ledCtrl.palette[ORANGE] = {121, 25, 0};
    ledCtrl.palette[CYAN] = {0, 255, 255};
    ledCtrl.palette[YELLOW] = {255, 255, 0};
    ledCtrl.palette[RED] = {255, 0, 0};
    ledCtrl.palette[GREEN] = {0, 255, 0};
    ledCtrl.palette[BLUE] = {0, 0, 255};
    ledCtrl.palette[WHITE] = {255, 255, 255};
    ledCtrl.palette[PURPLE] = {101, 0, 127};
    ledCtrl.palette[TEAL] = {0, 255, 128};

    ledCtrl.currentColor = getDefaultColorForBoard(BOARD_ID);
    ledCtrl.animIndex = 0;
    ledCtrl.animActive = false;
    ledCtrl.previousMillis = 0;
    ledCtrl.interval = 100;
}

void ledStripShowColor(LedStripController &ledCtrl, ColorEnum color, uint8_t brightness)
{
    RGB c = ledCtrl.palette[color];
    ledCtrl.strip->setBrightness(brightness);

    for (uint16_t i = 0; i < ledCtrl.strip->numPixels(); i++)
        ledCtrl.strip->setPixelColor(i, ledCtrl.strip->Color(c.r, c.g, c.b));

    ledCtrl.strip->show();
    ledCtrl.currentColor = color;
}

void ledStripClear(LedStripController &ledCtrl)
{
    ledCtrl.strip->clear();
    ledCtrl.strip->show();
    ledCtrl.animActive = false;
}

void ledStripAnimStart(LedStripController &ledCtrl, ColorEnum color, unsigned long intervalMs)
{
    ledCtrl.animActive = true;
    ledCtrl.currentColor = color;
    ledCtrl.interval = intervalMs;
    ledCtrl.previousMillis = millis();
    ledCtrl.animIndex = 0;
}

void ledStripAnimHandle(LedStripController &ledCtrl)
{
    if (!ledCtrl.animActive)
        return;

    unsigned long now = millis();
    if (now - ledCtrl.previousMillis >= ledCtrl.interval)
    {
        ledCtrl.previousMillis = now;

        ledCtrl.strip->clear();
        RGB c = ledCtrl.palette[ledCtrl.currentColor];
        if (ledCtrl.animIndex < ledCtrl.strip->numPixels())
            ledCtrl.strip->setPixelColor(ledCtrl.animIndex, ledCtrl.strip->Color(c.r, c.g, c.b));

        ledCtrl.strip->show();

        ledCtrl.animIndex++;
        if (ledCtrl.animIndex >= ledCtrl.strip->numPixels())
            ledCtrl.animIndex = 0; // loop animation
    }
}

void showClientState(ClientState state)
{
    switch (state)
    {
    case ClientState::STATE_SETUP:
        ledStripClear(ledStrip);
        ledStripShowColor(ledStrip, getDefaultColorForBoard(BOARD_ID));
        break;
    case ClientState::STATE_WAITING_FOR_CONNECTION:
        ledStripAnimStart(ledStrip, getDefaultColorForBoard(BOARD_ID), 50);
        break;
    case ClientState::STATE_WAITING_FOR_ACKNOWLEDGE:
        ledStripAnimStart(ledStrip, getDefaultColorForBoard(BOARD_ID), 10);
        break;
    case ClientState::STATE_CONNECTED_ACK:
        ledStripClear(ledStrip);
        ledStripShowColor(ledStrip, getDefaultColorForBoard(BOARD_ID));
        break;
    case ClientState::STATE_READY:
        ledStripAnimStart(ledStrip, getDefaultColorForBoard(BOARD_ID), 1000);
        break;
    case ClientState::STATE_COMMUNICATION_ERROR:
        ledStripAnimStart(ledStrip, RED, 200);
        break;
    }
}

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
