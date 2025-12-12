#pragma once
#include "debug.h"
#include "network/dto/udpMessageDTO.h"
#include "network/dto/webMessageDTO.h"
#include "network/UDP/gatewayUdp.h"
#include "network/websocket.h"

// TODO move this to something like ws utils
void gatewayUtilsBlinkClientsLed();

void gatewayUtilsRestartClients();

void gatewayUtilsRegistrationAck(IPAddress ip);

void gatewayUtilsSendSystemInfo();
