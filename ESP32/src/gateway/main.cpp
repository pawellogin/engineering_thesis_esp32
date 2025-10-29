#include <WiFi.h>
#include <WiFiUdp.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include "debug.h"
#include "config.h"
#include "utils.h"

// ---------------- Networking ----------------
WiFiUDP udp;
WebSocketsServer webSocket(udpPort + 1);

// ---------------- JSON ----------------
StaticJsonDocument<256> doc;

// ---------------- Heartbeat ----------------
unsigned long lastHeartbeat = 0;
const unsigned long heartbeatInterval = 5000;

// // ---------------- Client Structure ----------------
// struct Client
// {
//   IPAddress ip;
//   bool online;
//   String name;
// };

// Client clients[maxClients];

// // ---------------- WebSocket Event ----------------
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_DISCONNECTED:
    DEBUG_PRINTF("[%u] Disconnected!", num);
    break;
  case WStype_CONNECTED:
  {
    IPAddress ip = webSocket.remoteIP(num);
    DEBUG_PRINTF("[%u] Connected from %s", num, ip.toString().c_str());
  }
  break;
  case WStype_TEXT:
  {
    // Safely parse JSON
    DeserializationError error = deserializeJson(doc, payload, length);
    if (error)
    {
      DEBUG_PRINT("Failed to parse JSON");
      return;
    }

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
  DEBUG_PRINT("ESP32 Starting...");

  // WiFi AP
  WiFi.softAP(ssid, password, 1, 0, maxClients);
  IPAddress IP = WiFi.softAPIP();
  DEBUG_PRINTF("AP IP: %s", IP.toString().c_str());

  // // UDP
  // udp.begin(udpPort);
  // DEBUG_PRINTF("UDP listening on port %u", udpPort);

  // WebSocket
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  DEBUG_PRINTF("WebSocket server started on port %u", udpPort + 1);
  setWebSocket(&webSocket);
}

// // ---------------- Loop ----------------
void loop()
{
  webSocket.loop();

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
  //   DEBUG_PRINTF("UDP Received: %s", packetBuffer);
  // }
}
