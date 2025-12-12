#pragma once
#include "config.h"

struct ClientInfo
{
    IPAddress ip;
    unsigned long lastSeen; // for timeout / ping
    bool connected;
    uint8_t boardId;
};