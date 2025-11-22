#pragma once
#include <WiFi.h>
#include "config.h"
#include <ESPmDNS.h>

bool clientWiFiConnect(const char *ssid, const char *password, unsigned long timeoutMs = wifi_timeout);
IPAddress discoverGatewayIP(const char *hostname, unsigned long timeoutMs = wifi_timeout);
IPAddress clientGetIP();
