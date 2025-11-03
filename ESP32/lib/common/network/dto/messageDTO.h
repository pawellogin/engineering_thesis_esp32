#pragma once
#include <ArduinoJson.h>
#include <Arduino.h>

enum class MessageType
{
    COMMAND,
    STATUS,
    // EVENT,
    ERROR
};

enum class MessageAction
{
    RESTART,
    BLINK_LED,
};

struct MessageDTO
{
    MessageType type;
    MessageAction action;
    String data;             // raw JSON string or text
    unsigned long timestamp; // optional, set with millis()
};