#include "gameBase.h"

void handleGame(Game *game)
{
    unsigned long now = millis();

    switch (game->state)
    {
    case GAME_IDLE:
        // waiting for start
        break;

    case GAME_RUNNING:
        // LOG_INFO("GAME_RUNNING: startMillis=%lu, now=%lu, duration=%lu",
        //          game->startMillis, now, game->duration);
        if (now - game->startMillis >= game->duration)
        {
            // LOG_INFO("GAME_RUNNING -> GAME_FINISHED");
            game->state = GAME_FINISHED;
            if (game->onFinish)
                game->onFinish(game);
        }
        else
        {
            // LOG_INFO("GAME_RUNNING -> still running");

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
    LOG_INFO("game start");
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
