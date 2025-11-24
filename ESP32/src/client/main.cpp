#include <Arduino.h>
#include "network/wifi/clientWifi.h"
#include "network/UDP/clientUdp.h"
#include "config.h"
#include "debug.h"
#include "network/ota/otaUtils.h"
#include "IO/ledUtils.h"
#include "IO/buttonUtils.h"

// TODO restart command sometimes do not work
void setup()
{
  Serial.begin(115200);
  LOG_INFO("Client starting...");

  ledInit(builtInLed, BUILTIN_LED);
  ledStripInit(ledStrip);
  ledStripClear(ledStrip);

  showClientState(ClientState::STATE_SETUP);

  buttonInit(mainButton, MAIN_BUTTON);

  // TODO button led needs correct ground, get correct cables and change the machine itself
  // ledInit(buttonLed, BUTTON_LED);

  // 1. Connect to the same network as the gateway
  delay(2000);
  if (!clientWiFiConnect(sta_ssid, sta_password))
  {
    LOG_ERROR("Wi-Fi failed, aborting");
    return;
  }

  clientOtaInit();

  // 2. Discover gateway via mDNS
  showClientState(ClientState::STATE_WAITING_FOR_CONNECTION);
  gatewayIP = discoverGatewayIP(mDNS_hostname); // "esp-gateway"
  if (gatewayIP.toString() == "0.0.0.0")
  {
    LOG_ERROR("Gateway not found via mDNS, cannot start UDP");
    return;
  }
  // 3. Start UDP
  clientUdpInit();
  setGatewayIP(gatewayIP);

  showClientState(ClientState::STATE_WAITING_FOR_ACKNOWLEDGE);
}

void loop()
{
  // TODO button led has hardware isues with gnd
  // ledHandle(buttonLed);

  ledHandle(builtInLed);
  clientUdpLoop();
  clientOtaLoop();
  ledStripAnimHandle(ledStrip);

  clientUdpDiscoverPing();

  // TODO test, remove me
  if (buttonWasPressed(mainButton))
  {
    ledStripClear(ledStrip);
    // ledStripShowColor(ledStrip, RED, );
  }

  static unsigned long last = 0;

  if (millis() - last > 2000)
  {
    last = millis();
  }
}
