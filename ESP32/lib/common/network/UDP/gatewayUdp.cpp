#include <WiFi.h>
#include <WiFiUdp.h>
#include "config.h"
#include "gatewayUdp.h"
#include "debug.h"
#include "network/dto/udpMessageDTO.h"

WiFiUDP udp;

char incomingPacket[255]; // buffer for incoming data
IPAddress clientIP;       // stores last active client

void gatewayUdpInit()
{
    if (udp.begin(udp_port))
    {
        LOG_INFO("UDP Listening on port %d\n", udp_port);
    }
    else
    {
        Serial.println("UDP Failed to start.");
    }
}

void handleClientCommand(const UdpMessageDTO &msg, IPAddress clientIP)
{
    switch (msg.action)
    {
    case UdpMessageAction::PING:
        // gatewayUdpSend(clientIP, R"({"type":"status","action":"pong"})");
        break;

    default:
        break;
    }
}

void gatewayUdpLoop()
{
    int packetSize = udp.parsePacket();
    if (packetSize)
    {
        int len = udp.read(incomingPacket, sizeof(incomingPacket) - 1);
        if (len > 0)
            incomingPacket[len] = '\0';

        clientIP = udp.remoteIP(); // remember who sent it
        LOG_INFO("UDP Received from %s: %s\n", clientIP.toString().c_str(), incomingPacket);

        // TODO receive data from clients and handle it
    }
}

void gatewayUdpSend(const char *msg)
{
    if (clientIP)
    {
        udp.beginPacket(clientIP, udp_port);
        udp.print(msg);
        udp.endPacket();
        LOG_INFO("UDP Sent to %s: %s\n", clientIP.toString().c_str(), msg);
    }
}
