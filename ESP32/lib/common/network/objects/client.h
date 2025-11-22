#pragma once

#include "config.h"
#include <Arduino.h>
#include "debug.h"

struct ClientInfo
{
    IPAddress ip;
    unsigned long lastSeen; // for timeout / ping
    bool connected;
};

extern ClientInfo clients[];

enum class ClientRegisterResult
{
    ALREADY_REGISTERED,
    NEW_REGISTERED,
    NO_SPACE
};

ClientRegisterResult registerClient(IPAddress ip);
