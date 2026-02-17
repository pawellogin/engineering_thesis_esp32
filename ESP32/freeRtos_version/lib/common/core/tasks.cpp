#include "core/tasks.h"
#include <Arduino.h>
#include "network/udp/udpManager.h"
#include "network/websocket/websocketManager.h"
#include "drivers/ledManager.h"
#include "system/clients/clientsManager.h"
#include "network/wifi/wifiManager.h"
#include "game/gameEngineTask.h"
#include "system/systemContext.h"
#include "drivers/buttonManager.h"

TaskHandle_t gatewayUdpTaskHandle = NULL;
TaskHandle_t gatewayWebsocketTaskHandle = NULL;
TaskHandle_t gatewayLedTaskHandle = NULL;
TaskHandle_t clientPollTaskHandle = NULL;
TaskHandle_t clientWatchdogTaskHandle = NULL;
TaskHandle_t gatewayWsCommandTaskHandle = NULL;
TaskHandle_t gatewayWifiTaskHandle = NULL;
TaskHandle_t GameEngine_TaskHandle = NULL;
TaskHandle_t udpCommandTaskHandleGateway = NULL;

TaskHandle_t clientUdpTaskHandle = NULL;
TaskHandle_t clientLedTaskHandle = NULL;
TaskHandle_t clientWifiTaskHandle = NULL;
TaskHandle_t udpCommandTaskHandleClient = NULL;
TaskHandle_t updClientSendDiscoverPingTaskHandle = NULL;

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
        udpCommandTask,
        "udpCommandTaskClient",
        4096,
        nullptr,
        2,
        &udpCommandTaskHandleClient);

    xTaskCreate(
        clientLedTask,
        "clientLedTask",
        2048,
        nullptr,
        2,
        &clientLedTaskHandle);

    xTaskCreate(
        wifiTask,
        "wifiTask",
        4096,
        nullptr,
        5,
        &clientWifiTaskHandle);

    xTaskCreate(
        buttonTask,
        "buttonTask",
        2048,
        nullptr,
        2,
        nullptr);

    xTaskCreate(
        clientLogicTask,
        "clientLogicTask",
        2048,
        nullptr,
        2,
        nullptr);
}

void startGatewayTasks()
{

    xTaskCreate(
        udpCommandTask,
        "udpCommandTaskGateway",
        4096,
        nullptr,
        2,
        &udpCommandTaskHandleGateway);

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
        clientWatchdogTask,
        "clientWatchdogTask",
        2048,
        nullptr,
        2,
        &clientWatchdogTaskHandle);

    xTaskCreate(
        clientPollTask,
        "clientPollTask",
        2048,
        nullptr,
        2,
        &clientPollTaskHandle);

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

    xTaskCreate(
        wifiTask,
        "wifiTask",
        4096,
        nullptr,
        5,
        &gatewayWifiTaskHandle);

    xTaskCreate(
        GameEngine_Task,
        "GameEngine",
        8192,
        sys.gameEngine,
        2,
        &GameEngine_TaskHandle);
}