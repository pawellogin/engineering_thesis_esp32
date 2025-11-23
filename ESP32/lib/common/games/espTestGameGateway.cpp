#include "espTestGameGateway.h"

EspTestGameGateway espTestGameGateway;

void gatewayOnStart(Game *g)
{
    Serial.println("Gateway: Game started!");
    // reset scores
    for (int i = 0; i < max_clients; i++)
    {
        espTestGameGateway.clientScores[i] = 0;
    }

    UdpMessageDTO msg;

    msg.action = UdpMessageAction::ESP_GAME_TEST;
    msg.type = UdpMessageType::COMMAND;
    msg.data = "game start";
    msg.timestamp = millis();

    String serializedMsg = serializeUdpMessage(msg);

    gatewayUdpSendAll(serializedMsg.c_str());
}

void gatewayOnUpdate(Game *g)
{
    // TODO
    // processClientScores(); // non-blocking: read UDP packets

    // sendScoresToWebApp(); // non-blocking WebSocket update
}

void gatewayOnFinish(Game *g)
{
    LOG_INFO("Gateway: Game finished!");
    // sendFinalScoresToWebApp();
}

void setupEspTestGameGateway()
{
    espTestGameGateway.base.state = GAME_IDLE;
    espTestGameGateway.base.duration = 5000; // 5 seconds
    espTestGameGateway.base.onStart = gatewayOnStart;
    espTestGameGateway.base.onUpdate = gatewayOnUpdate;
    espTestGameGateway.base.onFinish = gatewayOnFinish;
}
