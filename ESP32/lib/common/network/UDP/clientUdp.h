#pragma once
#include <WiFi.h>
#include <WiFiUdp.h>
#include "config.h"
#include "debug.h"
#include "network/dto/udpMessageDTO.h"
#include "network/UDP/clientUtils.h"

extern IPAddress gatewayIP;

void clientUdpInit();
void clientUdpLoop();
void clientUdpDiscoverPing(unsigned int interval = 10000);

void clientUdpSend(const char *msg);

void setGatewayIP(IPAddress ip);