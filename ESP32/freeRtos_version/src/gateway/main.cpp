#include <Arduino.h>
#include "core/debug.h"
#include "core/config.h"
#include "network/wifi/wifiManager.h"
#include "network/udp/udpManager.h"
#include "network/websocket/websocketManager.h"
#include "core/tasks.h"
#include "drivers/ledManager.h"

void setup()
{
  Serial.begin(115200);
  LOG_INFO("ESP32 gateway starting...");

  if (connectToHotspot(sta_ssid, sta_password, 100000))
  {
    udpInit();
    webSocketInit();
  }
  else
  {
    LOG_ERROR("Connect to hotspot failed");
    while (true)
      ;
  }

  gatewayLedInitAll();

  startGatewayTasks();
}

void loop()
{

  // static unsigned long last = 0;

  // if (millis() - last > 2000)
  // {
  //   last = millis();

  // }
}

// TOOD
// make wifi logic, connect to hotspot in setup
// make ws logic, make ws task and queue