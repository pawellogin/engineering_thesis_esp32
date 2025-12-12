#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include "network/objects/clientInfo.h"

struct ClientsListDTO
{
    // TODO remove count
    uint8_t count;
    ClientInfo *items;
};

String serializeClientsDTO(const ClientsListDTO &dto);
