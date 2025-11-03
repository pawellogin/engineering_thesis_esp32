#pragma once
#include <WiFi.h>
#include <WebSocketsServer.h>
#include "debug.h"

bool setupNetwork();

bool connectToHotspot(const char *ssid, const char *password, unsigned long timeoutMs = 10000);

bool createAccessPoint(const char *ssid, const char *password, uint8_t channel = 1, uint8_t maxConn = 4);

IPAddress getLocalIP();
