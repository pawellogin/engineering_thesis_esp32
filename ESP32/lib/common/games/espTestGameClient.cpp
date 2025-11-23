#include "EspTestGameClient.h"
#include <Arduino.h>

// Global instance
EspTestGameClient espTestGameClient;

// Callbacks
void clientOnStart(Game *g)
{
    EspTestGameClient *game = (EspTestGameClient *)g;
    game->score = 0;
    LOG_INFO("Esp test game started!");

    ledStripAnimStart(ledStrip, getDefaultColorForBoard(BOARD_ID));
}

void clientOnUpdate(Game *g)
{
    EspTestGameClient *game = (EspTestGameClient *)g;
    // non-blocking button read
    if (buttonWasPressed(mainButton))
    {
        game->score++;
        // TODO
        // sendScoreToGateway(game->score); // UDP non-blocking
    }
}

void clientOnFinish(Game *g)
{
    EspTestGameClient *game = (EspTestGameClient *)g;
    LOG_INFO("Client: Game finished! Score: %d", game->score);
    ledStripClear(ledStrip);
}

void setupEspTestGameClient()
{
    espTestGameClient.base.state = GAME_IDLE;
    espTestGameClient.base.duration = 5000;
    espTestGameClient.base.onStart = clientOnStart;
    espTestGameClient.base.onUpdate = clientOnUpdate;
    espTestGameClient.base.onFinish = clientOnFinish;
}
