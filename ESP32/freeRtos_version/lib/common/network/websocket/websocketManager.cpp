#include "core/config.h"
#include "core/debug.h"
#include "network/websocket/websocketManager.h"
#include "drivers/ledManager.h"
#include "network/udp/udpCommands.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic error "-Wswitch-enum"

QueueHandle_t wsCommandQueue;

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
        return WebMessageType::UNKNOWN; // Unknown
    }
}
static WebMessageAction actionFromString(const String &actionStr)
{
    // TODO think of a better system or check, this approach is error prone
    if (actionStr == "restart_all")
        return WebMessageAction::RESTART_ALL;
    else if (actionStr == "restart_clients")
        return WebMessageAction::RESTART_CLIENTS;
    else if (actionStr == "restart_gateway")
        return WebMessageAction::RESTART_GATEWAY;
    else if (actionStr == "blink_clients_led")
        return WebMessageAction::BLINK_CLIENTS_LED;
    else if (actionStr == "blink_gateway_led")
        return WebMessageAction::BLINK_GATEWAY_LED;
    else if (actionStr == "ping")
        return WebMessageAction::PING;
    else if (actionStr == "get_system_info")
        return WebMessageAction::GET_SYSTEM_INFO;
    else if (actionStr == "start_esp_test_game")
        return WebMessageAction::START_ESP_TEST_GAME;
    else
    {
        LOG_ERROR("Missing web actionFromString conversion");
        return WebMessageAction::UNKNOWN; // Unknown
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
        serializeJson(doc["data"], temp);

        strncpy(msg.data, temp.c_str(), WS_DATA_MAX - 1);
        msg.data[WS_DATA_MAX - 1] = '\0';

        if (temp.length() >= WS_DATA_MAX)
        {
            LOG_INFO("WebSocket data truncated to %d bytes", WS_DATA_MAX - 1);
        }
    }
    else
    {
        LOG_INFO("Missing 'data' property in web msg json");
        msg.data[0] = '\0';
    }

    msg.timestamp = doc["timestamp"] | millis();

    String fullMsg;
    serializeJson(doc, fullMsg);
    LOG_INFO("WS received: %s", fullMsg.c_str());

    return true;
}

String serializeWebMessage(const WebMessageDTO &msg)
{
    JsonDocument doc;
    doc["type"] = typeToString(msg.type);
    doc["action"] = actionToString(msg.action);

    if (msg.data[0] != '\0')
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
    case WebMessageAction::RESTART_ALL:
        return "restart_all";
    case WebMessageAction::RESTART_CLIENTS:
        return "restart_clients";
    case WebMessageAction::RESTART_GATEWAY:
        return "restart_gateway";
    case WebMessageAction::BLINK_CLIENTS_LED:
        return "blink_clients_led";
    case WebMessageAction::BLINK_GATEWAY_LED:
        return "blink_gateway_led";
    case WebMessageAction::GET_SYSTEM_INFO:
        return "get_system_info";
    case WebMessageAction::START_ESP_TEST_GAME:
        return "start_esp_test_game";
    case WebMessageAction::END_ESP_TEST_GAME:
        return "end_esp_test_game";
    case WebMessageAction::PING:
        return "ping";
    case WebMessageAction::UNKNOWN:
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
    case WebMessageType::UNKNOWN:
    default:
        LOG_ERROR("Missing typeToString conversion");
        return "unknown";
    }
}

static WebSocketsServer *webSocket = nullptr;
static uint8_t adminClientId = 255;

static void processWebsocketCommand(const WebMessageDTO &msg)
{
    switch (msg.action)
    {
        // TODO maybe make separate action for gateway and clients restart
    case WebMessageAction::RESTART_ALL:
        // gatewayUtilsRestartClients();
        restartESP();
        break;
    case WebMessageAction::RESTART_CLIENTS:
        // gatewayUtilsRestartClients();
        break;
    case WebMessageAction::RESTART_GATEWAY:
        restartESP();
        break;
    case WebMessageAction::BLINK_CLIENTS_LED:
        // LOG_INFO("BLINK_CLIENTS_LED command received");
        udpBlinkAllClientsBuiltInLed();
        break;
    case WebMessageAction::BLINK_GATEWAY_LED:
        // LOG_INFO("BLINK_GATEWAY_LED command received");
        ledBlink(builtInLed, gatewayLedMutex);
        break;
    case WebMessageAction::PING:
        break;
    case WebMessageAction::START_ESP_TEST_GAME:
        // startGame(&espTestGameGateway.base);
        break;
    case WebMessageAction::END_ESP_TEST_GAME:
        // endGame(&espTestGameGateway.base);
        break;
    case WebMessageAction::GET_SYSTEM_INFO:
        // gatewayUtilsSendSystemInfo();
        break;
    case WebMessageAction::UNKNOWN:
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
        WebMessageDTO msg;
        if (deserializeWebMessage(payload, length, msg))
        {
            switch (msg.type)
            {
            case WebMessageType::COMMAND:
                xQueueSend(wsCommandQueue, &msg, 0);
                break;
            case WebMessageType::STATUS:
            case WebMessageType::ERROR:
            case WebMessageType::UNKNOWN:
            default:
                LOG_DEBUG("Received non-command message: %s", actionToString(msg.action).c_str());
                break;
            }
        }
        break;
    }

    case WStype_ERROR:
    case WStype_BIN:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    case WStype_PING:
    case WStype_PONG:
    default:
        break;
    }
}

void webSocketInit()
{

    wsCommandQueue = xQueueCreate(16, sizeof(WebMessageDTO));
    configASSERT(wsCommandQueue);

    if (webSocket != nullptr)
        delete webSocket;

    webSocket = new WebSocketsServer(ws_port);
    webSocket->begin();
    webSocket->onEvent(webSocketEvent);
    // setDebugWebSocket(webSocket);
    LOG_INFO("WebSocket server started on port %d", ws_port);
}

/*
 * For code that executes in loop search webSocketEvent
 */
void webSocketLoop()
{
    if (webSocket)
        webSocket->loop();
}

// TODO make a better name
void wsSendMessage(const WebMessageDTO &msg)
{
    if (adminClientId == 255)
        return;

    String json = serializeWebMessage(msg);
    LOG_INFO("Sent to web socket : %s\n", json.c_str());
    webSocket->sendTXT(adminClientId, json.c_str());
}

void websocketTask(void *p)
{
    while (true)
    {
        webSocketLoop();
        vTaskDelay(10 / portTICK_PERIOD_MS); // small delay to yield CPU
    }
}

void wsCommandTask(void *p)
{
    WebMessageDTO msg;

    while (true)
    {
        if (wsCommandQueue == NULL)
        {
        }
        else if (xQueueReceive(wsCommandQueue, &msg, portMAX_DELAY) == pdTRUE)
        {
            // TODO add fucntion to procces the type, and then inside it the action
            processWebsocketCommand(msg);
        }
    }
}

#pragma GCC diagnostic pop
