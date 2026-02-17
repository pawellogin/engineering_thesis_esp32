#pragma once
#include "gameBase.h"
#include "gameTypes.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

enum class GameEngineState
{
    ENGINE_STATE_IDLE = 0,
    ENGINE_STATE_SETUP,
    ENGINE_STATE_RUNNING,
    ENGINE_STATE_REPORT,
    ENGINE_STATE_ABORTED
};

struct GameEngine
{
    QueueHandle_t eventQueue;
    GameEngineState state;
    GameBase *activeGame;
};

void GameEngine_Init(GameEngine *engine, QueueHandle_t queue);
void GameEngine_Run(GameEngine *engine);
