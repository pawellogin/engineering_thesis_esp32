#pragma once
#include <cstdint>
#include <cstdbool>

constexpr uint8_t GAME_MAX_CLIENTS = 9;
constexpr uint8_t GAME_MAX_RESULTS = 9;



enum class GameEngineEventType : uint8_t
{
    NONE = 0,
    WEB_COMMAND,
    CLIENT_EVENT,
    TICK
};

enum class WebGameCommandType : uint8_t
{
    START_GAME = 0,
    CANCEL_GAME
};

enum class ClientEventType : uint8_t
{
    BUTTON_PRESS = 0,
    DISCONNECTED
};



struct WebGameCommand
{
    WebGameCommandType cmd;
    uint32_t game_id;
};

struct ClientGameEvent
{
    uint8_t board_id;
    ClientEventType type;
    uint32_t gateway_time_ms;
    uint32_t client_time_ms;
};

struct GameEngineEvent
{
    GameEngineEventType type;
    union
    {
        WebGameCommand web;
        ClientGameEvent client;
        uint32_t tick_time_ms;
    };
};

struct GameClientResult
{
    bool valid;
    uint8_t board_id;
    uint32_t reaction_time_ms;
    bool responded;
};

struct GameResult
{
    uint32_t game_id;
    uint8_t client_count;
    GameClientResult results[GAME_MAX_RESULTS];
};
