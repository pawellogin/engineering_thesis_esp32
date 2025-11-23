#include "gatewayUtils.h"

void gatewayUtilsBlinkClientsLed()
{
    LOG_INFO("Blink LED command received");
    UdpMessageDTO msg;

    msg.action = UdpMessageAction::BLINK_BUILTIN_LED;
    msg.type = UdpMessageType::COMMAND;
    msg.data = "";
    msg.timestamp = millis();

    String serializedMsg = serializeUdpMessage(msg);

    gatewayUdpSendAll(serializedMsg.c_str());
}

void gatewayUtilsRestartClients()
{
    LOG_INFO("restart clients command received");
    UdpMessageDTO msg;

    msg.action = UdpMessageAction::RESTART;
    msg.type = UdpMessageType::COMMAND;
    msg.data = "";
    msg.timestamp = millis();

    String serializedMsg = serializeUdpMessage(msg);

    gatewayUdpSendAll(serializedMsg.c_str());
}

void gatewayUtilsRegistrationAck(IPAddress ip)
{
    LOG_INFO("Registration ack command received");
    UdpMessageDTO msg;

    msg.action = UdpMessageAction::REGISTRATION_ACK;
    msg.type = UdpMessageType::COMMAND;
    msg.data = "";
    msg.timestamp = millis();

    String serializedMsg = serializeUdpMessage(msg);

    gatewayUdpSend(ip, serializedMsg.c_str());
}