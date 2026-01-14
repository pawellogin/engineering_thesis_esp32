#pragma once
#include <ArduinoJson.h>
#include <Arduino.h>
#include "core/config.h"

enum class WebMessageType
{
    UNKNOWN,
    COMMAND,
    STATUS,
    // EVENT,
    ERROR,
};

enum class WebMessageAction
{
    UNKNOWN,
    RESTART_ALL,
    RESTART_CLIENTS,
    RESTART_GATEWAY,
    BLINK_CLIENTS_LED,
    BLINK_GATEWAY_LED,
    PING,
    GET_SYSTEM_INFO,

    START_ESP_TEST_GAME,
    END_ESP_TEST_GAME,
};

/*
Message is for transfering data between Web and gateway using websocket
*/

struct WebMessageDTO
{
    WebMessageType type;
    WebMessageAction action;
    char data[WS_DATA_MAX];  // raw JSON string or text
    unsigned long timestamp; // optional, set with millis()
};

String serializeMessage(const WebMessageDTO &msg);
bool deserializeMessage(const uint8_t *payload, size_t length, WebMessageDTO &msg);

String actionToString(WebMessageAction action);
String typeToString(WebMessageType type);

void webSocketInit();
void webSocketLoop();
void wsSendMessage(const WebMessageDTO &msg);

void websocketTask(void *p);

void wsCommandTask(void *p);