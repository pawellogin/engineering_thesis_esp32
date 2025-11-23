#pragma once
#include "gameBase.h"
#include <Arduino.h>
#include "network/dto/udpMessageDTO.h"
#include "network/UDP/gatewayUdp.h"

struct EspTestGameGateway
{
    Game base;
    int clientScores[max_clients];
};

extern EspTestGameGateway espTestGameGateway;
void setupEspTestGameGateway();
