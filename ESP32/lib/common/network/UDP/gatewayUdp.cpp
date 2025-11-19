#include <WiFi.h>
#include <WiFiUdp.h>
#include "config.h"
#include "gatewayUdp.h"
#include "debug.h"
#include "network/dto/udpMessageDTO.h"
#include "network/objects/client.h"

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

void proccessClientCommand(const UdpMessageDTO &msg, IPAddress clientIP)
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

        IPAddress ip = udp.remoteIP();

        // register client automatically
        registerClient(ip);

        LOG_INFO("UDP Received from %s: %s\n", ip.toString().c_str(), incomingPacket);

        // parse JSON message
        UdpMessageDTO msg;
        if (deserializeUdpMessage((uint8_t *)incomingPacket, len, msg)) // implement this using ArduinoJson
        {
            switch (msg.type)
            {
            case UdpMessageType::COMMAND:
                proccessClientCommand(msg, ip);
                break;
            case UdpMessageType::STATUS:
            case UdpMessageType::ERROR:
            default:
                break;
            }
        }
    }
}

void gatewayUdpSend(IPAddress ip, const char *msg)
{
    udp.beginPacket(ip, udp_port);
    udp.print(msg);
    udp.endPacket();
    LOG_INFO("UDP Sent to %s: %s\n", ip.toString().c_str(), msg);
}

void gatewayUdpSendAll(const char *msg)
{
    for (size_t i = 0; i < max_clients; ++i)
    {
        if (clients[i].connected)
        {
            gatewayUdpSend(clients[i].ip, msg);
        }
    }
}

void gatewayPingClients()
{
    unsigned long now = millis();
    for (size_t i = 0; i < max_clients; ++i)
    {
        if (clients[i].connected)
        {
            if (now - clients[i].lastSeen > gateway_clients_ping_check_timeout)
            {
                LOG_INFO("Client timed out: %s\n", clients[i].ip.toString().c_str());
                clients[i].connected = false;
                continue;
            }
            UdpMessageDTO msg;

            msg.action = UdpMessageAction::PING;
            msg.type = UdpMessageType::COMMAND;
            msg.data = "PING";
            msg.timestamp = millis();

            String serializedMsg = serializeUdpMessage(msg);

            gatewayUdpSend(clients[i].ip, serializedMsg.c_str());
        }
    }
}
