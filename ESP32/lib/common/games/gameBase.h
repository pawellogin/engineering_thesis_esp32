#pragma once
#include <Arduino.h>
#include "debug.h"

enum GameState
{
    GAME_IDLE,
    GAME_RUNNING,
    GAME_FINISHED
};

struct Game
{
    GameState state;
    unsigned long startMillis;
    unsigned long duration;

    void (*onStart)(Game *game);
    void (*onUpdate)(Game *game);
    void (*onFinish)(Game *game);
};

void handleGame(Game *game);
void startGame(Game *game);
void resetGame(Game *game);
