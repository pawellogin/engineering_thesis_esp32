#include "debug.h"
#include "websocket.h"

static WebSocketsServer *webSocket = nullptr;
static uint8_t adminClientId = 255;

#pragma region utils
static MessageType typeFromString(const String &typeStr)
{
    if (typeStr == "command")
        return MessageType::COMMAND;
    else if (typeStr == "error")
        return MessageType::ERROR;
    else if (typeStr == "status")
        return MessageType::STATUS;
    else
        return static_cast<MessageType>(-1); // Unknown
}

static String typeToString(MessageType type)
{
    switch (type)
    {
    case MessageType::COMMAND:
        return "command";
    case MessageType::ERROR:
        return "error";
    case MessageType::STATUS:
        return "status";
    default:
        return "unknown";
    }
}

static MessageAction actionFromString(const String &actionStr)
{
    if (actionStr == "restart")
        return MessageAction::RESTART;
    else if (actionStr == "blink_led")
        return MessageAction::BLINK_LED;
    else
        return static_cast<MessageAction>(-1); // Unknown
}

static String actionToString(MessageAction action)
{
    switch (action)
    {
    case MessageAction::RESTART:
        return "restart";
    case MessageAction::BLINK_LED:
        return "blink_led";
    default:
        return "unknown";
    }
}
#pragma endregion

bool deserializeMessage(const uint8_t *payload, size_t length, MessageDTO &msg)
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

    if (doc["action"].is<const char *>())
    {
        msg.action = actionFromString(doc["action"].as<String>());
    }

    if (!doc["data"].isNull())
    {
        String temp;
        serializeJson(doc["data"], temp); // get raw string representation
        msg.data = temp;
    }
    else
    {
        msg.data = "";
    }

    msg.timestamp = doc["timestamp"] | millis();
    return true;
}

String serializeMessage(const MessageDTO &msg)
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

static void processCommand(const MessageDTO &msg)
{
    switch (msg.action)
    {
    case MessageAction::RESTART:
        LOG_INFO("Restart requested");
        // TODO restartSystem();
        break;

    case MessageAction::BLINK_LED:
        LOG_INFO("Blink LED command received");
        // TODO blinkLed();
        break;

    default:
        LOG_ERROR("Unknown command action");
        break;
    }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_CONNECTED:
        adminClientId = num;
        LOG_INFO("Admin connected: %d", num);
        break;

    case WStype_DISCONNECTED:
        LOG_INFO("Admin disconnected");
        adminClientId = 255;
        break;

    case WStype_TEXT:
    {
        MessageDTO msg;
        if (deserializeMessage(payload, length, msg))
        {
            if (msg.type == MessageType::COMMAND)
                processCommand(msg);
            else
                LOG_DEBUG("Received non-command message: %s", actionToString(msg.action).c_str());
        }
        break;
    }

    default:
        break;
    }
}

void setupWebSocket()
{
    if (webSocket != nullptr)
        delete webSocket;

    webSocket = new WebSocketsServer(ws_port);
    webSocket->begin();
    webSocket->onEvent(webSocketEvent);
    setDebugWebSocket(webSocket);
    LOG_INFO("WebSocket server started on port %d", ws_port);
}

void webSocketLoop()
{
    if (webSocket)
        webSocket->loop();
}

void sendMessage(const MessageDTO &msg)
{
    if (adminClientId == 255)
        return;

    String json = serializeMessage(msg);
    webSocket->sendTXT(adminClientId, json.c_str());
}
