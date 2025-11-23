#pragma once
#include "debug.h"
#include "network/dto/udpMessageDTO.h"
#include "network/UDP/gatewayUdp.h"

void gatewayUtilsBlinkClientsLed();

void gatewayUtilsRestartClients();

void gatewayUtilsRegistrationAck(IPAddress ip);
