#include "debug.h"
#include "network/dto/messageDTO.h"
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

    MessageDTO msg;
    msg.type = MessageType::STATUS;
    msg.action = MessageAction::BLINK_LED;
    msg.data = "test debug message";
    msg.timestamp = millis();

    String json = serializeMessage(msg);
    webSocketPtr->broadcastTXT(json.c_str());
}
