#pragma once
#include "gameTypes.h"
#include <cstddef>

enum class GameState
{
    GAME_STATE_IDLE = 0,
    GAME_STATE_SETUP,
    GAME_STATE_RUNNING,
    GAME_STATE_FINISHED,
    GAME_STATE_ABORTED
};

struct GameBase
{
    const char *name;

    void (*init)(GameBase *self);
    void (*start)(GameBase *self, uint32_t start_time_ms);
    void (*handleClientEvent)(GameBase *self, const ClientGameEvent *event, uint32_t now_ms);
    void (*tick)(GameBase *self, uint32_t now_ms);
    void (*cancel)(GameBase *self);

    bool (*isFinished)(GameBase *self);
    void (*getResult)(GameBase *self, GameResult *out);

    GameState state;
};

void gameResultToJson(char *out,
                      size_t outSize,
                      const GameResult *game,
                      size_t count);
