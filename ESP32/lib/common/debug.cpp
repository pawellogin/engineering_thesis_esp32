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

static bool ledState = false;
static unsigned long previousMillis = 0;
static unsigned long blinkDuration = 0;
static bool blinking = false;

void initBuiltInLED()
{
    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(BUILTIN_LED, LOW);
}

void blinkBuiltInLED(unsigned long durationMs)
{
    blinkDuration = durationMs;
    blinking = true;
    previousMillis = millis();
    ledState = true;
    digitalWrite(BUILTIN_LED, HIGH);
}

void handleBuiltInLED()
{
    if (blinking && millis() - previousMillis >= blinkDuration)
    {
        ledState = false;
        blinking = false;
        digitalWrite(BUILTIN_LED, LOW);
    }
}

void restartESP()
{
    esp_restart();
}