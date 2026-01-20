#include "udpCommands.h"
#include "udpManager.h"
#include "core/debug.h"
#include "core/config.h"

void udpBlinkAllClientsBuiltInLed()
{
    UdpMessageDTO msg;
    buildUdpMessage(msg);
    msg.action = UdpMessageAction::BLINK_BUILTIN_LED;
    msg.type = UdpMessageType::COMMAND;
    msg.data[0] = '\0';

    char out[UDP_DATA_MAX];
    size_t len;

    bool ok = serializeUdpMessage(msg, out, sizeof(out), len);

    if (!ok)
    {
        LOG_ERROR("serialization error in udpBlinkAllClientsBuiltInLed");
    }
    else
    {
        udpSendAllClients(out);
    }
}

void statusResponse()
{
    UdpMessageDTO msg;
    buildUdpMessage(msg);
    msg.action = UdpMessageAction::STATUS_RESPONSE;
    msg.type = UdpMessageType::COMMAND;
    msg.data[0] = '\0';

    char out[UDP_DATA_MAX];
    size_t len;

    bool ok = serializeUdpMessage(msg, out, sizeof(out), len);

    if (!ok)
    {
        LOG_ERROR("serialization error in statusResponse");
    }
    else
    {
        udpSend(gatewayIp, out, false);
    }
}
