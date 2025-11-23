#include "network/UDP/clientUdp.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic error "-Wswitch-enum"

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
        LOG_INFO("UDP Failed to start.");
    }
}

/*
Function to send message to gateway
*/
void clientUdpSend(const char *msg)
{
    if (!gatewayKnown)
    {
        LOG_INFO("UDP Gateway not known, skipping send.");
        showClientState(ClientState::STATE_COMMUNICATION_ERROR);
        return;
    }

    // Check Wi-Fi connection
    if (WiFi.status() != WL_CONNECTED)
    {
        LOG_ERROR("Wi-Fi not connected, cannot send UDP");
        showClientState(ClientState::STATE_COMMUNICATION_ERROR);
        return;
    }

    udp.beginPacket(gatewayIP, udp_port);
    udp.print(msg);
    int result = udp.endPacket(); // 1 = success, 0 = failure

    if (result == 1)
    {
        LOG_INFO("UDP Sent to gateway (%s): %s\n", gatewayIP.toString().c_str(), msg);
    }
    else
    {
        LOG_ERROR("UDP Failed to send to gateway (%s): %s, code: %d\n", gatewayIP.toString().c_str(), msg, result);
        showClientState(ClientState::STATE_COMMUNICATION_ERROR);
        gatewayKnown = false; // optionally mark gateway as unknown
    }
}

static void processGatewayCommand(const UdpMessageDTO &msg)
{
    switch (msg.action)
    {
    case UdpMessageAction::RESTART:
        restartESP();
        break;
    case UdpMessageAction::BLINK_BUILTIN_LED:
        ledBlink(builtInLed);
        break;
    case UdpMessageAction::REGISTRATION_ACK:
        // TODO add blinking the main led and led strip here, also make led strip correct color depending on the number
        clientUtilsRegistrationAck();
        break;
    case UdpMessageAction::ESP_GAME_TEST:
        // TODO
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

void clientUdpDiscoverPing(unsigned int interval)
{
    static unsigned int lastSend = 0;

    if (millis() - lastSend > 10000)
    {
        clientUtilsPingGateway();
        lastSend = millis();
    }
}

void setGatewayIP(IPAddress ip)
{
    gatewayIP = ip;
    gatewayKnown = true;
}

#pragma GCC diagnostic pop