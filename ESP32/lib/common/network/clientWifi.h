#pragma once
#include <WiFi.h>
#include <ESPmDNS.h>

bool clientWiFiConnect(const char *ssid, const char *password, unsigned long timeoutMs = 10000);
IPAddress discoverGatewayIP(const char *hostname, unsigned long timeoutMs = 3000);
IPAddress clientGetIP();
