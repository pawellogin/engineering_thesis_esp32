#pragma once
#include <ArduinoJson.h>
#include <WebSocketsServer.h>
#include "network/dto/webMessageDTO.h"
#include "debug.h"
#include "IO/ledUtils.h"
#include "games/testGameGateway.h"

void setupWebSocket();
void webSocketLoop();
void wsSendMessage(const WebMessageDTO &msg);