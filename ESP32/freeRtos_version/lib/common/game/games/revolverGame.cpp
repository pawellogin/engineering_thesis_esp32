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
static constexpr uint8_t REVOLVER_MAX_PLAYERS = 2;

static uint8_t BASE_TIME = 3000;
static uint8_t RANDOM_TIME = 2001;

struct RevolverGameState
{
    uint32_t start_time_ms;
    uint32_t end_time_ms;

    bool led_was_triggered;
    uint32_t start_trigger_time;
    uint32_t client_click_time[REVOLVER_MAX_PLAYERS];
};

static RevolverGameState state;

/* ---------- Game callbacks ---------- */

static void revolverGame_init(GameBase *self)
{
    std::memset(&state, 0, sizeof(state));
    self->state = GameState::GAME_STATE_IDLE;
}

static void revolverGame_start(GameBase *self, uint32_t start_time_ms)
{
    state.start_time_ms = start_time_ms;
    state.end_time_ms = start_time_ms + TEST_GAME_DURATION_MS;
    state.led_was_triggered = false;

    uint32_t random_delay = BASE_TIME + (esp_random() % RANDOM_TIME);

    state.start_trigger_time = start_time_ms + random_delay;

    self->state = GameState::GAME_STATE_RUNNING;
}

static void revolverGame_handleClientEvent(GameBase *self, const ClientGameEvent *event, uint32_t now_ms)
{
    if (self->state != GameState::GAME_STATE_RUNNING)
        return;

    if (event->type != ClientEventType::BUTTON_PRESS)
        return;

    uint8_t boardID = event->board_id;
    uint8_t arrayID = boardID - first_client_board_id;
    LOG_DEBUG("revolver client event id = %d", event->board_id);
    if (boardID >= GAME_MAX_CLIENTS + first_client_board_id)
        return;

    if (now_ms >= state.start_trigger_time && state.client_click_time[arrayID] == 0)
    {
        LOG_DEBUG("now_ms %d, state.start_trigger_time %d", now_ms, state.start_trigger_time);
        state.client_click_time[arrayID] = now_ms;
    }
}

static char out[UDP_DATA_MAX];

static void revolverGame_tick(GameBase *self, uint32_t now_ms)
{
    // TODO turn clients led on after random time

    if (self->state != GameState::GAME_STATE_RUNNING)
        return;

    if (state.client_click_time[0] != 0 && state.client_click_time[1] != 0)
    {
        self->state = GameState::GAME_STATE_FINISHED;
        return;
    }

    UdpMessageDTO msg;
    buildUdpMessage(msg);
    msg.action = UdpMessageAction::BLINK_BUILTIN_LED;
    msg.type = UdpMessageType::COMMAND;
    msg.data[0] = '\0';

    size_t len;

    bool ok = serializeUdpMessage(msg, out, sizeof(out), len);

    // check random LED activation
    for (uint8_t i = 0; i < REVOLVER_MAX_PLAYERS; ++i)
    {
        if (now_ms >= state.start_trigger_time && state.led_was_triggered == false)
        {
            udpSend(clients[i].ip, out, len, true);
            state.led_was_triggered = true;
        }
    }

    // for (uint8_t i = 0; i < GAME_MAX_CLIENTS; ++i)
    // {
    //     if (state.led_should_be_on[i])
    //     {
    //         // udpSend(clients[i].ip, out, len, true);
    //         state.led_should_be_on[i] = false;
    //     }
    // }

    if (now_ms >= state.end_time_ms)
    {

        // LOG_DEBUG(" now ms: %d", now_ms);
        // LOG_DEBUG("end time ms: %d", state.end_time_ms);
        self->state = GameState::GAME_STATE_FINISHED;
        // LOG_DEBUG("test game tick");
        return;
    }
}

static void revolverGame_cancel(GameBase *self)
{
    self->state = GameState::GAME_STATE_ABORTED;
}

static bool revolverGame_isFinished(GameBase *self)
{
    return (self->state == GameState::GAME_STATE_FINISHED ||
            self->state == GameState::GAME_STATE_ABORTED);
}

static void revolverGame_getResult(GameBase *self, GameResult *out)
{

    for (uint8_t i = 0; i < REVOLVER_MAX_PLAYERS; ++i)
    {
        // LOG_DEBUG("board id: %d, active: %d, click count: %d", i + first_client_board_id, state.active[i], state.click_count[i]);
        out->results[i].board_id = i + first_client_board_id;
        out->results[i].responded = state.client_click_time[i] != 0 ? true : false;
        out->results[i].score = state.client_click_time[i] == 0 ? 0 : (state.client_click_time[i] - state.start_time_ms);
        out->results[i].valid = true;
        LOG_DEBUG("board id: %d, responded: %d, score: %d, valid: %d", out->results[i].board_id, out->results[i].responded, out->results[i].score, out->results[i].valid);
    }
}

/* ---------- Game instance ---------- */

GameBase revolverGame =
    {
        .name = "Revolver game",
        .init = revolverGame_init,
        .start = revolverGame_start,
        .handleClientEvent = revolverGame_handleClientEvent,
        .tick = revolverGame_tick,
        .cancel = revolverGame_cancel,
        .isFinished = revolverGame_isFinished,
        .getResult = revolverGame_getResult,
        .state = GameState::GAME_STATE_IDLE};
