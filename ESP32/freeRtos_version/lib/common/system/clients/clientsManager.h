#pragma once
#include <Arduino.h>
#include "core/config.h"
#include "core/debug.h"
#include <WiFi.h>

enum class ClientState
{
    CLIENT_OFFLINE,
    CLIENT_ONLINE
};

struct ClientInfo
{
    uint8_t boardId;
    IPAddress ip;
    uint32_t lastSeen;
    ClientState state;
};

extern ClientInfo clients[max_clients];

void initClients();
void onClientPacket(int boardID);
bool checkBoardID(int id);
bool checkClientBoardID(int id);

void clientPollTask(void *p);
void clientWatchdogTask(void *p);
