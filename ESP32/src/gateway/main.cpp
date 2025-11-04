#include <WiFi.h>
#include <WiFiUdp.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include "debug.h"
#include "config.h"
#include "utils.h"
#include "network/wifi.h"
#include "network/websocket.h"

// ---------------- Networking ----------------
WiFiUDP udp;
WebSocketsServer webSocket(udp_port + 1);

// ---------------- JSON ----------------
JsonDocument doc;

// TODO move
// // ---------------- Client Structure ----------------
// struct Client
// {
//   IPAddress ip;
//   bool online;
//   String name;
// };

// Client clients[max_clients];

// ---------------- Setup ----------------
void setup()
{
  Serial.begin(115200);
  delay(2000);
  LOG_INFO("ESP32 Starting...");

  // WIFI and websocket setup
  if (setupNetwork())
  {
    setupWebSocket();
  }

  initDebugLED();

  // // UDP
  // udp.begin(udpPort);
  // LOG_DEBUG("UDP listening on port %u", udpPort);
}

// // ---------------- Loop ----------------
void loop()
{
  webSocketLoop();
  handleLED();

  static unsigned long last = 0;

  if (millis() - last > 1000)
  {
    last = millis();
    // blinkLED();
    // broadcastDebug();
  }

  // // Example: check UDP packets
  // int packetSize = udp.parsePacket();
  // if (packetSize)
  // {
  //   char packetBuffer[255];
  //   int len = udp.read(packetBuffer, 255);
  //   if (len > 0)
  //     packetBuffer[len] = 0;
  //   LOG_DEBUG("UDP Received: %s", packetBuffer);
  // }
}
