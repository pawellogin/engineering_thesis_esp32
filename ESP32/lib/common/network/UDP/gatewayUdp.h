#pragma once

void gatewayUdpInit();
void gatewayUdpLoop();
void gatewayUdpSend(IPAddress ip, const char *msg);
void gatewayUdpSendAll(const char *msg);
void gatewayPingClients();