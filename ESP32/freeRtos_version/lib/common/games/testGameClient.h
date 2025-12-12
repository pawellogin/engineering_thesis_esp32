#pragma once
#include "gameBase.h"
#include "IO/ledUtils.h"
#include "IO/buttonUtils.h"
#include "network/dto/udpMessageDTO.h"
#include "network/UDP/clientUdp.h"

struct TestGameClient
{
    Game base;
    int score;
};

extern TestGameClient espTestGameClient;
void testGameClientSetup();
