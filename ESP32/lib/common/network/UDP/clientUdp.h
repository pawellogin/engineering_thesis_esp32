#pragma once

extern IPAddress gatewayIP;

void clientUdpInit();
void clientUdpLoop();
void clientUdpTestPing();

void clientUdpSend(const char *msg);

void setGatewayIP(IPAddress ip);