#include "clientUtils.h"
#include "network/dto/udpMessageDTO.h"
#include "network/UDP/clientUdp.h"

void pingGateway()
{
    UdpMessageDTO msg;

    msg.action = UdpMessageAction::BLINK_BUILTIN_LED;
    msg.type = UdpMessageType::COMMAND;
    msg.data = "test led";
    msg.timestamp = millis();

    String serializedMsg = serializeUdpMessage(msg);
    clientUdpSend(serializedMsg.c_str());
}