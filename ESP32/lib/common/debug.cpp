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

static bool ledState = false;
static unsigned long previousMillis = 0;
static unsigned long blinkDuration = 0;
static bool blinking = false;

void initDebugLED()
{
    pinMode(GATEWAY_LED, OUTPUT);
    digitalWrite(GATEWAY_LED, LOW);
}

void blinkLED(unsigned long durationMs)
{
    blinkDuration = durationMs;
    blinking = true;
    previousMillis = millis();
    ledState = true;
    digitalWrite(GATEWAY_LED, HIGH);
}

void handleLED()
{
    if (blinking && millis() - previousMillis >= blinkDuration)
    {
        ledState = false;
        blinking = false;
        digitalWrite(GATEWAY_LED, LOW);
    }
}