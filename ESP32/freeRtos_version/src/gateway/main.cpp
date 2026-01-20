#include <Arduino.h>
#include "core/debug.h"
#include "core/config.h"
#include "network/wifi/wifiManager.h"
#include "network/udp/udpManager.h"
#include "network/websocket/websocketManager.h"
#include "core/tasks.h"
#include "drivers/ledManager.h"
#include "system/systemContext.h"
#include "system/clients/clientsManager.h"

void setup()
{
  Serial.begin(115200);
  LOG_INFO("ESP32 gateway starting...");
  systemContextInit();

  initClients();

  wifiInit();

  gatewayLedInitAll();
  startGatewayTasks();
}

void loop()
{
}
