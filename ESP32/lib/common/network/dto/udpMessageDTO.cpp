#include "udpMessageDTO.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic error "-Wswitch-enum"

static UdpMessageType typeFromString(const String &typeStr)
{
    if (typeStr == "command")
        return UdpMessageType::COMMAND;
    else if (typeStr == "error")
        return UdpMessageType::ERROR;
    else if (typeStr == "status")
        return UdpMessageType::STATUS;
    else
    {
        LOG_ERROR("Missing typeFromString conversion");
        return static_cast<UdpMessageType>(-1); // Unknown
    }
}
static UdpMessageAction actionFromString(const String &actionStr)
{

    // TODO find a way to check this for every possibility, switch wont work here i think
    if (actionStr == "restart")
        return UdpMessageAction::RESTART;
    else if (actionStr == "blink_builtin_led")
        return UdpMessageAction::BLINK_BUILTIN_LED;
    else if (actionStr == "registration_ack")
        return UdpMessageAction::REGISTRATION_ACK;
    else if (actionStr == "ping")
        return UdpMessageAction::PING;
    else if (actionStr == "esp_game_test")
        return UdpMessageAction::ESP_GAME_TEST;
    else
    {
        LOG_ERROR("Missing actionFromString conversion");
        return static_cast<UdpMessageAction>(-1); // Unknown
    }
}

static String actionToString(UdpMessageAction action)
{
    switch (action)
    {
    case UdpMessageAction::RESTART:
        return "restart";
    case UdpMessageAction::BLINK_BUILTIN_LED:
        return "blink_builtin_led";
    case UdpMessageAction::REGISTRATION_ACK:
        return "registration_ack";
    case UdpMessageAction::ESP_GAME_TEST:
        return "esp_game_test";
    case UdpMessageAction::PING:
        return "ping";
    default:
        LOG_ERROR("Missing actionToString conversion");
        return "unknown";
    }
}

static String typeToString(UdpMessageType type)
{
    switch (type)
    {
    case UdpMessageType::COMMAND:
        return "command";
    case UdpMessageType::ERROR:
        return "error";
    case UdpMessageType::STATUS:
        return "status";
    default:
        LOG_ERROR("Missing typeToString conversion");
        return "unknown";
    }
}

String serializeUdpMessage(const UdpMessageDTO &msg)
{
    JsonDocument doc;
    doc["type"] = typeToString(msg.type);
    doc["action"] = actionToString(msg.action);

    if (msg.data.length() > 0)
    {
        JsonDocument dataDoc;
        DeserializationError err = deserializeJson(dataDoc, msg.data);
        if (!err)
            doc["data"] = dataDoc;
        else
            doc["data"] = msg.data;
    }

    doc["timestamp"] = msg.timestamp;

    String json;
    serializeJson(doc, json);
    return json;
}

bool deserializeUdpMessage(const uint8_t *payload, size_t length, UdpMessageDTO &msg)
{
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload, length);
    if (error)
    {
        LOG_ERROR("Invalid JSON in udp message");
        String raw = String((const char *)payload).substring(0, length);
        LOG_ERROR("Raw JSON: %s", raw.c_str());
        return false;
    }

    if (doc["type"].is<const char *>())
    {
        msg.type = typeFromString(doc["type"].as<String>());
    }
    else
    {
        LOG_INFO("Missing 'type' property in json");
    }

    if (doc["action"].is<const char *>())
    {
        msg.action = actionFromString(doc["action"].as<String>());
    }
    else
    {
        LOG_INFO("Missing 'action' property in json");
    }

    if (doc["data"].is<const char *>())
    {
        String temp;
        serializeJson(doc["data"], temp); // get raw string representation
        msg.data = temp;
    }
    else
    {
        LOG_INFO("Missing 'data' property in UDP json");
        msg.data = "";
    }

    msg.timestamp = doc["timestamp"] | millis();
    return true;
}

#pragma GCC diagnostic pop
