#include "gatewayUtils.h"

void blinkClientLed()
{
    LOG_INFO("Blink LED command received");
    UdpMessageDTO msg;

    msg.action = UdpMessageAction::BLINK_BUILTIN_LED;
    msg.type = UdpMessageType::COMMAND;
    msg.data = "test led";
    msg.timestamp = millis();

    String serializedMsg = serializeUdpMessage(msg);

    gatewayUdpSend(serializedMsg.c_str());
}