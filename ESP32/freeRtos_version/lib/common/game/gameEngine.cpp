#include "gameEngine.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static constexpr TickType_t ENGINE_QUEUE_TIMEOUT = pdMS_TO_TICKS(50);

void GameEngine_Init(GameEngine *engine, QueueHandle_t queue)
{
    engine->eventQueue = queue;
    engine->state = GameEngineState::ENGINE_STATE_IDLE;
    engine->activeGame = nullptr;
}

void GameEngine_Run(GameEngine *engine)
{
    GameEngineEvent ev{};

    while (true)
    {
        // Block waiting for an event, but wake up periodically
        if (xQueueReceive(engine->eventQueue, &ev, ENGINE_QUEUE_TIMEOUT) == pdTRUE)
        {
            switch (engine->state)
            {
            /* ================= IDLE ================= */
            case GameEngineState::ENGINE_STATE_IDLE:
            {
                if (ev.type == GameEngineEventType::WEB_COMMAND &&
                    ev.web.cmd == WebGameCommandType::START_GAME &&
                    engine->activeGame)
                {
                    engine->activeGame->init(engine->activeGame);
                    engine->activeGame->start(engine->activeGame, ev.tick_time_ms);
                    engine->state = GameEngineState::ENGINE_STATE_RUNNING;
                }
                break;
            }

            /* ================= RUNNING ================= */
            case GameEngineState::ENGINE_STATE_RUNNING:
            {
                if (ev.type == GameEngineEventType::CLIENT_EVENT)
                {
                    engine->activeGame->handleClientEvent(
                        engine->activeGame,
                        &ev.client);
                }
                else if (ev.type == GameEngineEventType::WEB_COMMAND &&
                         ev.web.cmd == WebGameCommandType::CANCEL_GAME)
                {
                    engine->activeGame->cancel(engine->activeGame);
                    engine->state = GameEngineState::ENGINE_STATE_ABORTED;
                }
                break;
            }

            /* ================= ABORTED ================= */
            case GameEngineState::ENGINE_STATE_ABORTED:
            {
                engine->state = GameEngineState::ENGINE_STATE_IDLE;
                break;
            }

            default:
                break;
            }
        }

        /* ================= PERIODIC TICK ================= */
        if (engine->state == GameEngineState::ENGINE_STATE_RUNNING &&
            engine->activeGame)
        {
            uint32_t now_ms = xTaskGetTickCount() * portTICK_PERIOD_MS;
            engine->activeGame->tick(engine->activeGame, now_ms);

            if (engine->activeGame->isFinished(engine->activeGame))
            {
                engine->state = GameEngineState::ENGINE_STATE_REPORT;
            }
        }

        /* ================= REPORT ================= */
        if (engine->state == GameEngineState::ENGINE_STATE_REPORT &&
            engine->activeGame)
        {
            GameResult result{};
            engine->activeGame->getResult(engine->activeGame, &result);

            // TODO:
            // - send result to web app
            // - send GAME_END to clients

            engine->state = GameEngineState::ENGINE_STATE_IDLE;
        }
    }
}
