#include "testGameGateway.h"

TetsGameGateway espTestGameGateway;

void testGameGatewayOnStart(Game *g)
{
    LOG_INFO("Gateway: Game started!");
    // reset scores
    for (int i = 0; i < max_clients; i++)
    {
        espTestGameGateway.clientScores[i] = 0;
    }

    UdpMessageDTO msg;

    msg.action = UdpMessageAction::TEST_GAME_START;
    msg.type = UdpMessageType::COMMAND;
    msg.data = "game start";
    msg.timestamp = millis();

    String serializedMsg = serializeUdpMessage(msg);

    gatewayUdpSendAll(serializedMsg.c_str());
}

void testGameGatewayOnUpdate(Game *g)
{
    // TODO
    // processClientScores(); // non-blocking: read UDP packets

    // sendScoresToWebApp(); // non-blocking WebSocket update
}

void testGameGatewayOnFinish(Game *g)
{
    LOG_INFO("Gateway: Game finished!");

    // TODO for creating messageDTO i there should be a helper function
    UdpMessageDTO msg;

    msg.action = UdpMessageAction::TEST_GAME_END;
    msg.type = UdpMessageType::COMMAND;
    msg.data = "game end";
    msg.timestamp = millis();

    String serializedMsg = serializeUdpMessage(msg);

    gatewayUdpSendAll(serializedMsg.c_str());
}

void testGameGatewaySetup()
{
    espTestGameGateway.base.state = GAME_IDLE;
    espTestGameGateway.base.duration = 5000; // 5 seconds
    espTestGameGateway.base.onStart = testGameGatewayOnStart;
    espTestGameGateway.base.onUpdate = testGameGatewayOnUpdate;
    espTestGameGateway.base.onFinish = testGameGatewayOnFinish;
}
