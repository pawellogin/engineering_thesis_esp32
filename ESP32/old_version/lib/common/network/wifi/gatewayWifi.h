#pragma once
#include <WiFi.h>
#include <WebSocketsServer.h>
#include "debug.h"
#include <ESPmDNS.h>

bool setupNetwork();

bool connectToHotspot(const char *ssid, const char *password, unsigned long timeoutMs = wifi_timeout);

bool createAccessPoint(const char *ssid, const char *password, uint8_t channel = 1, uint8_t maxConn = 4);

IPAddress getLocalIP();
