#include "gameBase.h"

void handleGame(Game *game)
{
    switch (game->state)
    {
    case GAME_IDLE:
        // waiting for start
        break;

    case GAME_RUNNING:
        if (millis() - game->startMillis >= game->duration)
        {
            game->state = GAME_FINISHED;
            if (game->onFinish)
                game->onFinish(game);
        }
        else
        {
            if (game->onUpdate)
                game->onUpdate(game);
        }
        break;

    case GAME_FINISHED:
        // waiting for reset
        break;
    }
}

// Start a game
void startGame(Game *game)
{
    game->state = GAME_RUNNING;
    game->startMillis = millis();
    if (game->onStart)
        game->onStart(game);
}

// Reset a game
void resetGame(Game *game)
{
    game->state = GAME_IDLE;
    if (game->onFinish)
        game->onFinish(game); // optional cleanup
}
