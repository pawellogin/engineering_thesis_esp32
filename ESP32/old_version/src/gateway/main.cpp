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
#include "IO/ledUtils.h"
#include "games/gameBase.h"
#include "games/testGameGateway.h"

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

  testGameGatewaySetup();
  ledInit(builtInLed, BUILTIN_LED);
  ledInit(buttonLed, BUTTON_LED);
}

void loop()
{
  webSocketLoop();
  ledHandle(builtInLed);
  ledHandle(buttonLed);
  gatewayUdpLoop();

  handleGame(&espTestGameGateway.base);

  static unsigned long last = 0;

  if (millis() - last > 2000)
  {
    // for testing
    last = millis();
    // ledBlink();
    // broadcastDebug();
  }
}
