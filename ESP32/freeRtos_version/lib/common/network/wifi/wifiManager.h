#pragma once
#include <WiFi.h>

#ifndef BOARD_ID
#error "BOARD_ID must be defined in platformio.ini using -DBOARD_ID=X"
#endif

IPAddress getStaticIP();
IPAddress getHotspot();
IPAddress getSubnet();

bool connectToHotspot(const char *ssid, const char *password, unsigned long timeoutMs);
