#include "core/tasks.h"
#include <Arduino.h>
#include "network/udp/udpManager.h"
#include "network/websocket/websocketManager.h"
#include "drivers/ledManager.h"

TaskHandle_t udpTaskHandle = NULL;
TaskHandle_t websocketTaskHandle = NULL;
TaskHandle_t ledTaskHandle = NULL;
TaskHandle_t wsCommandTaskHandle = NULL;

void startTasks()
{
    xTaskCreate(
        udpTask,
        "udpTask",
        2048,
        nullptr,
        2,
        &udpTaskHandle);

    xTaskCreate(
        websocketTask,
        "websocketTask",
        4096,
        nullptr,
        2,
        &websocketTaskHandle);

    xTaskCreate(
        ledTask,
        "ledTask",
        2048,
        nullptr,
        2,
        &ledTaskHandle);

    xTaskCreate(
        wsCommandTask,
        "wsCommandTask",
        4096,
        nullptr,
        2,
        &wsCommandTaskHandle);
}