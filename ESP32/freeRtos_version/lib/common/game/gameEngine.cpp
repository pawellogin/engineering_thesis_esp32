#include "gameEngine.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "core/debug.h"
#include "game/games/testGame.h"
#include "game/games/revolverGame.h"
#include "network/websocket/websocketManager.h"

static constexpr TickType_t ENGINE_QUEUE_TIMEOUT = pdMS_TO_TICKS(50);

void GameEngine_Init(GameEngine *engine, QueueHandle_t queue)
{
    engine->eventQueue = queue;
    engine->state = GameEngineState::ENGINE_STATE_IDLE;
    engine->activeGame = nullptr;
}

void GameEngine_Run(GameEngine *engine)
{
    GameEngineEvent gameEvent{};

    while (true)
    {
        // Block waiting for an event, but wake up periodically
        if (xQueueReceive(engine->eventQueue, &gameEvent, ENGINE_QUEUE_TIMEOUT) == pdTRUE)
        {
            // LOG_DEBUG("game engine queue recive ");
            switch (engine->state)
            {
            /* ================= IDLE ================= */
            case GameEngineState::ENGINE_STATE_IDLE:
            {
                if (gameEvent.type == GameEngineEventType::WEB_COMMAND && gameEvent.web.cmd == WebGameCommandType::START_GAME)
                {
                    // Select game
                    switch (gameEvent.web.game)
                    {
                    case GameType::TEST:
                        engine->activeGame = &testGame;
                        break;

                    case GameType::REVOLVER:
                        engine->activeGame = &revolverGame;
                        break;

                    default:
                        engine->activeGame = nullptr;
                        break;
                    }

                    if (engine->activeGame)
                    {
                        engine->activeGame->init(engine->activeGame);

                        uint32_t now_ms =
                            xTaskGetTickCount() * portTICK_PERIOD_MS;

                        engine->activeGame->start(
                            engine->activeGame,
                            now_ms);

                        engine->state =
                            GameEngineState::ENGINE_STATE_RUNNING;
                    }
                }

                break;
            }

            /* ================= RUNNING ================= */
            case GameEngineState::ENGINE_STATE_RUNNING:
            {
                LOG_DEBUG("ENGINE_STATE_RUNNING");
                if (gameEvent.type == GameEngineEventType::CLIENT_EVENT)
                {
                    uint32_t now_ms = xTaskGetTickCount() * portTICK_PERIOD_MS;

                    engine->activeGame->handleClientEvent(
                        engine->activeGame,
                        &gameEvent.client, now_ms);
                }
                else if (gameEvent.type == GameEngineEventType::WEB_COMMAND &&
                         gameEvent.web.cmd == WebGameCommandType::CANCEL_GAME)
                {
                    engine->activeGame->cancel(engine->activeGame);
                    engine->state = GameEngineState::ENGINE_STATE_ABORTED;
                }
                break;
            }

            /* ================= ABORTED ================= */
            case GameEngineState::ENGINE_STATE_ABORTED:
            {
                LOG_DEBUG("ENGINE_STATE_ABORTED");
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
            // LOG_DEBUG("PERIODIC TICK");
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
            LOG_DEBUG("ENGINE_STATE_REPORT");
            GameResult result{};
            engine->activeGame->getResult(engine->activeGame, &result);

            WebMessageDTO msg;
            msg.action = WebMessageAction::UNKNOWN;
            msg.type = WebMessageType::STATUS;
            msg.timestamp = millis();

            gameResultToJson(msg.data, WS_DATA_MAX, &result, GAME_MAX_CLIENTS);

            wsSendMessage(msg);
            // TODO:
            // - send result to web app
            // - send GAME_END to clients ( not in every game )

            engine->state = GameEngineState::ENGINE_STATE_IDLE;
        }
    }
}
