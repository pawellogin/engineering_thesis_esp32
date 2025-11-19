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

ClientInfo clients[max_clients];

bool registerClient(IPAddress ip);
