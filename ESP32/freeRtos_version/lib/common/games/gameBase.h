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
    /*Callback function that is called when the games ends. Is called both after shouldEnd ends the game or endGame function is called explicitly */
    void (*onFinish)(Game *game);

    /*This callback function checks some condition, if the condition is true it ends the game
    Preferably use this only in gateway, and then send the end game udp message to clients
    */
    bool (*shouldEnd)(struct Game *game);
};

/*Call this in main loop*/
void handleGame(Game *game);
/*Call this to start the game*/
void startGame(Game *game);
// TODO probably remove
void resetGame(Game *game);

/*Ends the game by calling this function */
void endGame(Game *game);
