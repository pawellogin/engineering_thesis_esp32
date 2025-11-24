#include "debug.h"
#include "websocket.h"
#include "network/dto/udpMessageDTO.h"
#include "network/UDP/gatewayUdp.h"
#include "network/UDP/gatewayUtils.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic error "-Wswitch-enum"

static WebSocketsServer *webSocket = nullptr;
static uint8_t adminClientId = 255;

static void processWebsocketCommand(const WebMessageDTO &msg)
{
    switch (msg.action)
    {
        // TODO maybe make separate action for gateway and clients restart
    case WebMessageAction::RESTART:
        gatewayUtilsRestartClients();
        restartESP();
        break;
    case WebMessageAction::RESTART_CLIENTS:
        gatewayUtilsRestartClients();
        break;
    case WebMessageAction::BLINK_CLIENTS_LED:
        LOG_INFO("BLINK_CLIENTS_LED command received");
        gatewayUtilsBlinkClientsLed();
        break;
    case WebMessageAction::BLINK_GATEWAY_LED:
        LOG_INFO("BLINK_GATEWAY_LED command received");
        ledBlink(builtInLed);
        break;
    case WebMessageAction::PING:
        break;
    case WebMessageAction::START_ESP_TEST_GAME:

        break;
    case WebMessageAction::GET_SYSTEM_INFO:
        gatewayUtilsSendSystemInfo();
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
        WebMessageDTO msg;
        if (deserializeWebMessage(payload, length, msg))
        {
            switch (msg.type)
            {
            case WebMessageType::COMMAND:
                processWebsocketCommand(msg);
                break;
            case WebMessageType::STATUS:
            case WebMessageType::ERROR:
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

// TODO make a better name
void wsSendMessage(const WebMessageDTO &msg)
{
    if (adminClientId == 255)
        return;

    String json = serializeWebMessage(msg);
    LOG_INFO("Sent to web socket : %s\n", json.c_str());
    webSocket->sendTXT(adminClientId, json.c_str());
}

#pragma GCC diagnostic pop
