#include <WiFi.h>
#include <WiFiUdp.h>
#include "config.h"
#include "debug.h"
#include "network/dto/udpMessageDTO.h"

WiFiUDP udp;
char incomingPacket[255];
IPAddress gatewayIP; // discovered dynamically
bool gatewayKnown = false;

void clientUdpInit()
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

/*
Function to send message to gateway
*/
void clientUdpSend(const char *msg)
{
    if (gatewayKnown)
    {
        udp.beginPacket(gatewayIP, udp_port);
        udp.print(msg);
        udp.endPacket();
        LOG_INFO("UDP Sent to gateway (%s): %s\n", gatewayIP.toString().c_str(), msg);
    }
    else
    {
        Serial.println("UDP Gateway not known, skipping send.");
    }
}

static void processGatewayCommand(const UdpMessageDTO &msg)
{
    switch (msg.action)
    {
    case UdpMessageAction::BLINK_BUILTIN_LED:
        blinkLED(200);
        break;
    case UdpMessageAction::PING:
        // TODO
        clientUdpSend("PONG");
        break;
    default:
        LOG_ERROR("Unknown UDP command");
        break;
    }
}

void clientUdpLoop()
{
    int packetSize = udp.parsePacket();
    if (!packetSize)
        return;

    int len = udp.read(incomingPacket, sizeof(incomingPacket) - 1);
    if (len > 0)
        incomingPacket[len] = 0;

    gatewayIP = udp.remoteIP();
    gatewayKnown = true;

    UdpMessageDTO msg;
    if (deserializeUdpMessage((uint8_t *)incomingPacket, len, msg))
    {
        switch (msg.type)
        {
        case UdpMessageType::COMMAND:
            processGatewayCommand(msg);
            break;

        case UdpMessageType::STATUS:
        case UdpMessageType::ERROR:
        default:
            break;
        }
    }
}

// Example: client sends “PING” every 2 seconds
void clientUdpTestPing()
{
    static unsigned long lastSend = 0;

    if (millis() - lastSend > 10000)
    {
        clientUdpSend("PING");
        lastSend = millis();
    }
}

void setGatewayIP(IPAddress ip)
{
    gatewayIP = ip;
    gatewayKnown = true;
}
