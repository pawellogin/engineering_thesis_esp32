#include "testGameClient.h"
#include <Arduino.h>

// Global instance
TestGameClient espTestGameClient;

// Callbacks
void testGameClientOnStart(Game *g)
{
    TestGameClient *game = (TestGameClient *)g;
    game->score = 0;
    LOG_INFO("Esp test game started!");

    ledStripAnimStart(ledStrip, getDefaultColorForBoard(BOARD_ID));
}

void testGameClientOnUpdate(Game *g)
{
    TestGameClient *game = (TestGameClient *)g;
    // non-blocking button read
    if (buttonWasPressed(mainButton))
    {
        game->score++;

        UdpMessageDTO msg;

        msg.type = UdpMessageType::COMMAND;
        msg.action = UdpMessageAction::TEST_GAME_STATUS;
        msg.data = (String)game->score;
        msg.timestamp = millis();

        String serializedMsg = serializeUdpMessage(msg);
        clientUdpSend(serializedMsg.c_str());
    }
}

void testGameClientOnFinish(Game *g)
{
    TestGameClient *game = (TestGameClient *)g;
    LOG_INFO("Client: Game finished! Score: %d", game->score);
    ledStripClear(ledStrip);
    showClientState(ClientState::STATE_READY);
}

bool testGameClientShouldEnd(Game *g)
{
    // TODO add logic that waits for the message from gateway to end the game

    return false;
}

void testGameClientSetup()
{
    // TODO think what to do with this time, we should and the game after the gateway sends end msg
    espTestGameClient.base.state = GAME_IDLE;
    espTestGameClient.base.duration = 0;
    espTestGameClient.base.onStart = testGameClientOnStart;
    espTestGameClient.base.onUpdate = testGameClientOnUpdate;
    espTestGameClient.base.onFinish = testGameClientOnFinish;
    espTestGameClient.base.shouldEnd = testGameClientShouldEnd;
}