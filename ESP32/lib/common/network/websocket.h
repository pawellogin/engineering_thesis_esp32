#pragma once
#include <ArduinoJson.h>
#include <WebSocketsServer.h>
#include "network/dto/messageDTO.h"

void setupWebSocket();
void webSocketLoop();
String serializeMessage(const MessageDTO &msg);