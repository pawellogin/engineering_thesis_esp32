#pragma once
#include "gameBase.h"
#include <Arduino.h>
#include "network/dto/udpMessageDTO.h"
#include "network/UDP/gatewayUdp.h"

struct TetsGameGateway
{
    Game base;
    int clientScores[max_clients];
};

extern TetsGameGateway espTestGameGateway;
void testGameGatewaySetup();
