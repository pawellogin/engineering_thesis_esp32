#pragma once
#include <ArduinoJson.h>
#include <Arduino.h>
#include "debug.h"

enum class UdpMessageType
{
    COMMAND,
    STATUS,
    // EVENT,
    ERROR,
};

enum class UdpMessageAction
{
    RESTART,
    BLINK_BUILTIN_LED,
    REGISTRATION_ACK,
    PING,

    ESP_GAME_TEST,
};

/*
UdpMessage is for transfering data between client and gateway
*/
struct UdpMessageDTO
{
    UdpMessageType type;
    UdpMessageAction action;
    String data;             // raw JSON string or text
    unsigned long timestamp; // optional, set with millis()
};

String serializeUdpMessage(const UdpMessageDTO &msg);
bool deserializeUdpMessage(const uint8_t *payload, size_t length, UdpMessageDTO &msg);

// String actionToString(UdpMessageAction action);
// String typeToString(UdpMessageType type);

// UdpMessageType typeFromString(const String &typeStr);
// UdpMessageAction actionFromString(const String &actionStr);
