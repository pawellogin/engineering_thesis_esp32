#include "debug.h"
#include "network/dto/webMessageDTO.h"
#include "network/websocket.h"

void heartbeatDebug()
{
    static bool state = false;
    state = !state;
    Serial.print("Heartbeat: ");
    Serial.println(state ? "ON" : "OFF");
}

static WebSocketsServer *webSocketPtr = nullptr;

void setDebugWebSocket(WebSocketsServer *ws)
{
    webSocketPtr = ws;
}

void broadcastDebug()
{
    if (!webSocketPtr)
        return; // safety check

    WebMessageDTO msg;
    msg.type = WebMessageType::STATUS;
    msg.action = WebMessageAction::BLINK_CLIENTS_LED;
    msg.data = "test debug message";
    msg.timestamp = millis();

    String json = serializeWebMessage(msg);
    webSocketPtr->broadcastTXT(json.c_str());
}

void restartESP()
{
    ledStripClear(ledStrip);
    delay(1000);

    esp_restart();
}