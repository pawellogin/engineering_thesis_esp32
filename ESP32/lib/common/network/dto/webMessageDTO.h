#pragma once
#include <ArduinoJson.h>
#include <Arduino.h>
#include "debug.h"

enum class WebMessageType
{
    COMMAND,
    STATUS,
    // EVENT,
    ERROR,
};

enum class WebMessageAction
{
    RESTART,
    RESTART_CLIENTS,
    BLINK_CLIENTS_LED,
    BLINK_GATEWAY_LED,
    PING,

    START_ESP_TEST_GAME,
};

/*
Message is for transfering data between Web and gateway using websocket
*/
struct WebMessageDTO
{
    WebMessageType type;
    WebMessageAction action;
    String data;             // raw JSON string or text
    unsigned long timestamp; // optional, set with millis()
};

String serializeWebMessage(const WebMessageDTO &msg);
bool deserializeWebMessage(const uint8_t *payload, size_t length, WebMessageDTO &msg);

String actionToString(WebMessageAction action);
String typeToString(WebMessageType type);

// WebMessageType typeFromString(const String &typeStr);
// WebMessageAction actionFromString(const String &actionStr);
