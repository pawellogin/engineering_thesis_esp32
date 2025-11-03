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

// TODO move
// // ---------------- WebSocket Event ----------------
void webSocketEvent2(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_DISCONNECTED:
    LOG_DEBUG("[%u] Disconnected!", num);
    break;
  case WStype_CONNECTED:
  {
    IPAddress ip = webSocket.remoteIP(num);
    LOG_DEBUG("[%u] Connected from %s", num, ip.toString().c_str());
  }
  break;
  case WStype_TEXT:
  {
    // Safely parse JSON
    DeserializationError error = deserializeJson(doc, payload, length);
    if (error)
    {
      LOG_DEBUG("Failed to parse JSON");
      return;
    }

    String message = String((char *)payload).substring(0, length);
    LOG_DEBUG("[%u] Received message: %s", num, message.c_str());
    // Example command: register client
    // if (doc.containsKey("function") && String(doc["function"]) == "ADDCLIENT")
    // {
    //   String name = doc["name"] | "unnamed";
    //   addClient(webSocket.remoteIP(num), name);
    // }

    doc.clear();
  }
  break;
  }
}

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
    broadcastDebug();
  }

  // // UDP
  // udp.begin(udpPort);
  // LOG_DEBUG("UDP listening on port %u", udpPort);
}

// // ---------------- Loop ----------------
void loop()
{
  webSocketLoop();

  static unsigned long last = 0;

  if (millis() - last > 1000)
  {
    last = millis();
    broadcastDebug();
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
