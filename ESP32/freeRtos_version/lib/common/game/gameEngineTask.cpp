#include "gameEngineTask.h"
#include "gameEngine.h"
#include "system/systemContext.h"
#include "game/games/testGame.h"

static GameEngine gameEngine;

void GameEngine_SystemInit()
{
    if (!sys.isGateway)
        return;

    sys.gameEngineQueue = xQueueCreate(
        16,
        sizeof(GameEngineEvent));

    GameEngine_Init(&gameEngine, sys.gameEngineQueue);

    // Temporary: single game
    gameEngine.activeGame = &testGame;

    sys.gameEngine = &gameEngine;
}

void GameEngine_Task(void *arg)
{
    GameEngine_Run(static_cast<GameEngine *>(arg));
}
