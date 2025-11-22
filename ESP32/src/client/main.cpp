#include <Arduino.h>
#include "network/wifi/clientWifi.h"
#include "network/UDP/clientUdp.h"
#include "config.h"
#include "debug.h"
#include "network/ota/otaUtils.h"

void setup()
{
  Serial.begin(115200);
  LOG_INFO("Client starting...");

  initBuiltInLED();

  // 1. Connect to the same network as the gateway
  if (!clientWiFiConnect(sta_ssid, sta_password))
  {
    LOG_ERROR("Wi-Fi failed, aborting");
    return;
  }

  clientOtaInit();

  // 2. Discover gateway via mDNS
  gatewayIP = discoverGatewayIP(mDNS_hostname); // "esp-gateway"
  if (gatewayIP.toString() == "0.0.0.0")
  {
    LOG_ERROR("Gateway not found via mDNS, cannot start UDP");
    return;
  }

  // 3. Start UDP
  clientUdpInit();
  setGatewayIP(gatewayIP); // you’ll add this in clientUdp.cpp
}

void loop()
{
  handleBuiltInLED();
  clientUdpLoop();
  clientOtaLoop();

  clientUdpDiscoverPing();
}
