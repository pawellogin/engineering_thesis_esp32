#include "webMessageDTO.h"

static WebMessageType typeFromString(const String &typeStr)
{
    if (typeStr == "command")
        return WebMessageType::COMMAND;
    else if (typeStr == "error")
        return WebMessageType::ERROR;
    else if (typeStr == "status")
        return WebMessageType::STATUS;
    else
    {
        LOG_ERROR("Missing typeFromString conversion");
        return static_cast<WebMessageType>(-1); // Unknown
    }
}
static WebMessageAction actionFromString(const String &actionStr)
{

    if (actionStr == "restart")
        return WebMessageAction::RESTART;
    else if (actionStr == "restart_clients")
        return WebMessageAction::RESTART_CLIENTS;
    else if (actionStr == "blink_clients_led")
        return WebMessageAction::BLINK_CLIENTS_LED;
    else if (actionStr == "blink_gateway_led")
        return WebMessageAction::BLINK_GATEWAY_LED;
    else if (actionStr == "ping")
        return WebMessageAction::PING;
    else
    {
        LOG_ERROR("Missing actionFromString conversion");
        return static_cast<WebMessageAction>(-1); // Unknown
    }
}

bool deserializeWebMessage(const uint8_t *payload, size_t length, WebMessageDTO &msg)
{
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload, length);
    if (error)
    {
        LOG_ERROR("Invalid JSON in WebSocket message");
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

    if (!doc["data"].isNull())
    {
        String temp;
        serializeJson(doc["data"], temp); // get raw string representation
        msg.data = temp;
    }
    else
    {
        LOG_INFO("Missing 'data' property in web msg json");
        msg.data = "";
    }

    msg.timestamp = doc["timestamp"] | millis();
    return true;
}

String serializeWebMessage(const WebMessageDTO &msg)
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

String actionToString(WebMessageAction action)
{
    switch (action)
    {
    case WebMessageAction::RESTART:
        return "restart";
    case WebMessageAction::RESTART_CLIENTS:
        return "restart_clients";
    case WebMessageAction::BLINK_CLIENTS_LED:
        return "blink_clients_led";
    case WebMessageAction::BLINK_GATEWAY_LED:
        return "blink_gateway_led";
    case WebMessageAction::PING:
        return "ping";
    default:
        LOG_ERROR("Missing actionToString conversion");
        return "unknown";
    }
}

String typeToString(WebMessageType type)
{
    switch (type)
    {
    case WebMessageType::COMMAND:
        return "command";
    case WebMessageType::ERROR:
        return "error";
    case WebMessageType::STATUS:
        return "status";
    default:
        LOG_ERROR("Missing typeToString conversion");
        return "unknown";
    }
}
