#pragma once
#include <ArduinoJson.h>
#include <WebSocketsServer.h>
#include "network/dto/webMessageDTO.h"
#include "debug.h"
#include "IO/ledUtils.h"

void setupWebSocket();
void webSocketLoop();