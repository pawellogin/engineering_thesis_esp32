#pragma once
#include "debug.h"
#include "network/dto/udpMessageDTO.h"
#include "network/UDP/gatewayUdp.h"

void gatewayUtilsBlinkClientsLed();

void gatewayUtilsRegistrationAck(IPAddress ip);
