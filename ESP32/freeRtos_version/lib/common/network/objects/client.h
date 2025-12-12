#pragma once

#include "config.h"
#include <Arduino.h>
#include "debug.h"
#include "network/dto/clientListDTO.h"
#include "clientInfo.h"

// TODO move to different dir
extern ClientInfo clients[];

enum class ClientRegisterResult
{
    ALREADY_REGISTERED,
    NEW_REGISTERED,
    NO_SPACE
};

ClientRegisterResult registerClient(IPAddress ip, uint8_t boardId);

ClientsListDTO getClientsListDTO();
