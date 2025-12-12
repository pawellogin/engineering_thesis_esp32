#pragma once
#include <WiFi.h>
#include <WiFiUdp.h>
#include "config.h"
#include "gatewayUdp.h"
#include "debug.h"
#include "network/dto/udpMessageDTO.h"
#include "network/objects/client.h"
#include "network/UDP/gatewayUtils.h"

void gatewayUdpInit();
void gatewayUdpLoop();
void gatewayUdpSend(IPAddress ip, const char *msg);
void gatewayUdpSendAll(const char *msg);
void proccessClientCommand(const UdpMessageDTO &msg, IPAddress clientIP);
// void gatewayPingClients();