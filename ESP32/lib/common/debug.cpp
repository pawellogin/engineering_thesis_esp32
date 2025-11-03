#include "debug.h"

void heartbeatDebug()
{
    static bool state = false;
    state = !state;
    Serial.print("Heartbeat: ");
    Serial.println(state ? "ON" : "OFF");
}

static WebSocketsServer *webSocketPtr = nullptr;

void setWebSocket(WebSocketsServer *ws)
{
    webSocketPtr = ws;
}

void broadcastDebug()
{
    if (!webSocketPtr)
        return; // safety check

    JsonDocument doc;
    doc["heartbeat"] = millis();

    // Example client data
    // You may want to pass actual client info here
    doc["example"] = "debug message";

    char buffer[256];
    size_t n = serializeJson(doc, buffer);
    webSocketPtr->broadcastTXT(buffer, n);
}
