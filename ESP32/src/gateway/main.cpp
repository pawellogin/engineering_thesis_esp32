#include <WiFi.h>
#include <WiFiUdp.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include "debug.h"
#include "config.h"
#include "network/wifi/gatewayWifi.h"
#include "network/websocket.h"
#include "network/UDP/gatewayUdp.h"
#include "network/dto/udpMessageDTO.h"

JsonDocument doc;

void setup()
{
  Serial.begin(115200);
  delay(2000);
  LOG_INFO("ESP32 Starting...");

  // WIFI and websocket setup
  if (setupNetwork())
  {
    setupWebSocket();
    gatewayUdpInit();
  }

  initBuiltInLED();
}

void loop()
{
  webSocketLoop();
  handleBuiltInLED();
  gatewayUdpLoop();

  static unsigned long last = 0;

  if (millis() - last > 2000)
  {

    last = millis();
    // blinkBuiltInLED();
    // broadcastDebug();
  }
}
