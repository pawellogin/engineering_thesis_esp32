#pragma once
#include <WiFi.h>
#include <WebSocketsServer.h>
#include "debug.h"

void setupNetwork(WebSocketsServer *ws, std::function<void(uint8_t, WStype_t, uint8_t *, size_t)> wsEvent);

bool connectToHotspot(const char *ssid, const char *password, unsigned long timeoutMs = 10000);

bool createAccessPoint(const char *ssid, const char *password, uint8_t channel = 1, uint8_t maxConn = 4);

IPAddress getLocalIP();
