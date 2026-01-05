#include "core/tasks.h"
#include <Arduino.h>
#include "network/udp/udpManager.h"
#include "network/websocket/websocketManager.h"
#include "drivers/ledManager.h"

TaskHandle_t gatewayUdpTaskHandle = NULL;
TaskHandle_t gatewayWebsocketTaskHandle = NULL;
TaskHandle_t gatewayLedTaskHandle = NULL;
TaskHandle_t gatewayWsCommandTaskHandle = NULL;

TaskHandle_t clientUdpTaskHandle = NULL;
TaskHandle_t clientLedTaskHandle = NULL;

void startClientTasks()
{
    xTaskCreate(
        udpTask,
        "udpTask",
        2048,
        nullptr,
        2,
        &clientUdpTaskHandle);

    xTaskCreate(
        clientLedTask,
        "clientLedTask",
        2048,
        nullptr,
        2,
        &clientLedTaskHandle);
}

void startGatewayTasks()
{
    xTaskCreate(
        udpTask,
        "udpTask",
        2048,
        nullptr,
        2,
        &gatewayUdpTaskHandle);

    xTaskCreate(
        websocketTask,
        "websocketTask",
        4096,
        nullptr,
        2,
        &gatewayWebsocketTaskHandle);

    xTaskCreate(
        gatewayLedTask,
        "gatewayLedTask",
        2048,
        nullptr,
        2,
        &gatewayLedTaskHandle);

    xTaskCreate(
        wsCommandTask,
        "wsCommandTask",
        4096,
        nullptr,
        2,
        &gatewayWsCommandTaskHandle);
}