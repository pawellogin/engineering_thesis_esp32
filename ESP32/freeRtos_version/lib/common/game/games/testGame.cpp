#include "testGame.h"
#include <cstring>
#include "core/debug.h"
#include "network/udp/udpManager.h"
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

    uint8_t id = event->board_id;
    LOG_DEBUG("testGame client event id = %d", event->board_id);
    if (id >= GAME_MAX_CLIENTS)
        return;

    state.click_count[id]++;
    state.active[id] = true;
    state.led_should_be_on[id] = true;
}

static void testGame_tick(GameBase *self, uint32_t now_ms)
{
    if (self->state != GameState::GAME_STATE_RUNNING)
        return;

    UdpMessageDTO msg;
    buildUdpMessage(msg);
    msg.action = UdpMessageAction::BLINK_BUILTIN_LED;
    msg.type = UdpMessageType::COMMAND;
    msg.data[0] = '\0';

    char out[UDP_DATA_MAX];
    size_t len;

    bool ok = serializeUdpMessage(msg, out, sizeof(out), len);

    for (uint8_t i = 0; i < GAME_MAX_CLIENTS; ++i)
    {
        if (state.led_should_be_on[i])
        {
            udpSend(i, out, false);
            state.led_should_be_on[i] = false; // consume intent
        }
    }

    if (now_ms >= state.end_time_ms)
    {
        self->state = GameState::GAME_STATE_FINISHED;
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
    out->client_count = GAME_MAX_CLIENTS;

    for (uint8_t i = 0; i < GAME_MAX_CLIENTS; ++i)
    {
        out->results[i].board_id = i;
        out->results[i].responded = state.active[i];
        out->results[i].reaction_time_ms = state.click_count[i];
    }
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
