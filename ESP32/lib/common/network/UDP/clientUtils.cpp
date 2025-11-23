#include "clientUtils.h"
#include "network/dto/udpMessageDTO.h"
#include "network/UDP/clientUdp.h"

void clientUtilsPingGateway()
{
    UdpMessageDTO msg;

    msg.type = UdpMessageType::COMMAND;
    msg.action = UdpMessageAction::PING;
    msg.data = "PING";
    msg.timestamp = millis();

    String serializedMsg = serializeUdpMessage(msg);
    clientUdpSend(serializedMsg.c_str());
}

// TODO add blinking the main led and led strip here, also make led strip correct color depending on the number

void clientUtilsRegistrationAck()
{
    ledBlink(builtInLed, 1000);
    showClientState(ClientState::STATE_READY);

    // TODO add better way to show connection, preferably show special animation on the led strip
}
