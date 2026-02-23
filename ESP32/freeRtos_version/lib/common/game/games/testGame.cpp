#include "testGame.h"
#include <cstring>
#include "core/debug.h"
#include "network/udp/udpManager.h"
#include "system/clients/clientsManager.h"
#include "network/udp/udpCommands.h"
#include "network/udp/udpManager.h"
#include "drivers/ledManager.h"
#include <cstdarg>

static constexpr uint32_t TEST_GAME_DURATION_MS = 10 * 1000;

struct TestGameState
{
    uint32_t start_time_ms;
    uint32_t end_time_ms;

    uint8_t click_count[GAME_MAX_CLIENTS];
    bool active[GAME_MAX_CLIENTS];
    bool led_should_be_on[GAME_MAX_CLIENTS];
};

static TestGameState state;

/* ---------- Game callbacks ---------- */

static void testGame_init(GameBase *self)
{
    std::memset(&state, 0, sizeof(state));
    self->state = GameState::GAME_STATE_IDLE;
}

static void testGame_start(GameBase *self, uint32_t start_time_ms)
{
    state.start_time_ms = start_time_ms;
    state.end_time_ms = start_time_ms + TEST_GAME_DURATION_MS;

    self->state = GameState::GAME_STATE_RUNNING;
}

static void testGame_handleClientEvent(GameBase *self, const ClientGameEvent *event)
{
    if (self->state != GameState::GAME_STATE_RUNNING)
        return;

    if (event->type != ClientEventType::BUTTON_PRESS)
        return;

    uint8_t boardID = event->board_id;
    uint8_t arrayID = boardID - first_client_board_id;
    LOG_DEBUG("testGame client event id = %d", event->board_id);
    if (boardID >= GAME_MAX_CLIENTS)
        return;

    state.click_count[arrayID]++;
    state.active[arrayID] = true;
    state.led_should_be_on[arrayID] = true;
}

static char out[UDP_DATA_MAX];

static void testGame_tick(GameBase *self, uint32_t now_ms)
{
    if (self->state != GameState::GAME_STATE_RUNNING)
        return;

    UdpMessageDTO msg;
    buildUdpMessage(msg);
    msg.action = UdpMessageAction::BLINK_BUILTIN_LED;
    msg.type = UdpMessageType::COMMAND;
    msg.data[0] = '\0';

    // char out[UDP_DATA_MAX];
    size_t len;

    bool ok = serializeUdpMessage(msg, out, sizeof(out), len);

    for (uint8_t i = 0; i < GAME_MAX_CLIENTS; ++i)
    {
        if (state.led_should_be_on[i])
        {
            ledBlink(builtInLed);
            state.led_should_be_on[i] = false;
        }
    }

    if (now_ms >= state.end_time_ms)
    {

        // LOG_DEBUG(" now ms: %d", now_ms);
        // LOG_DEBUG("end time ms: %d", state.end_time_ms);
        self->state = GameState::GAME_STATE_FINISHED;
        // LOG_DEBUG("test game tick");
        return;
    }
}

static void testGame_cancel(GameBase *self)
{
    self->state = GameState::GAME_STATE_ABORTED;
}

static bool testGame_isFinished(GameBase *self)
{
    return (self->state == GameState::GAME_STATE_FINISHED ||
            self->state == GameState::GAME_STATE_ABORTED);
}

static void testGame_getResult(GameBase *self, GameResult *out)
{

    for (uint8_t i = 0; i < GAME_MAX_CLIENTS; ++i)
    {
        // LOG_DEBUG("board id: %d, active: %d, click count: %d", i + first_client_board_id, state.active[i], state.click_count[i]);
        out->results[i].board_id = i + first_client_board_id;
        out->results[i].responded = state.active[i];
        out->results[i].score = state.click_count[i];
        out->results[i].valid = true;
        LOG_DEBUG("board id: %d, active: %d, click count: %d, valid: %d", out->results[i].board_id, out->results[i].responded, out->results[i].score, out->results[i].valid);
    }
}

inline bool append(char *out, size_t outSize, size_t &offset, const char *fmt, ...)
{
    if (offset >= outSize)
        return false;

    va_list args;
    va_start(args, fmt);
    int written = vsnprintf(out + offset, outSize - offset, fmt, args);
    va_end(args);

    if (written < 0 || (size_t)written >= outSize - offset)
    {
        out[outSize - 1] = '\0';
        return false;
    }

    offset += written;
    return true;
}

void gameResultToJson(char *out,
                      size_t outSize,
                      const GameResult *game,
                      size_t count)
{
    if (outSize == 0)
    {
        LOG_ERROR("gameResultToJson e1");
        return;
    }

    size_t offset = 0;

    if (!append(out, outSize, offset, "["))
    {
        LOG_ERROR("gameResultToJson e2");
        return;
    }

    bool first = true;

    for (size_t i = 0; i < count; i++)
    {
        const GameClientResult &r = game->results[i];

        // optional filter (skip invalid entries)
        if (!r.valid)
        {
            LOG_ERROR("gameResultToJson e3");
            continue;
        }

        if (!append(out, outSize, offset,
                    "%s{\"valid\":%s,\"board_id\":%u,\"score\":%u,\"responded\":%s}",
                    first ? "" : ",",
                    r.valid ? "true" : "false",
                    r.board_id,
                    r.score,
                    r.responded ? "true" : "false"))
        {
            LOG_ERROR("gameResultToJson e4");
            return;
        }

        first = false;
    }

    append(out, outSize, offset, "]");
}

/* ---------- Game instance ---------- */

GameBase testGame =
    {
        .name = "TestButtonCount",
        .init = testGame_init,
        .start = testGame_start,
        .handleClientEvent = testGame_handleClientEvent,
        .tick = testGame_tick,
        .cancel = testGame_cancel,
        .isFinished = testGame_isFinished,
        .getResult = testGame_getResult,
        .state = GameState::GAME_STATE_IDLE};
