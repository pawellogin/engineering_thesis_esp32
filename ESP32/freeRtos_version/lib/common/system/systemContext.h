#pragma once
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/event_groups.h>

struct SystemContext
{
    EventGroupHandle_t systemEvents;

    QueueHandle_t udpCommandQueue;
    QueueHandle_t wsCommandQueue;
};

extern SystemContext sys;

void systemContextInit();

// Event bits
#define SYS_WIFI_READY (1 << 0)
#define SYS_UDP_READY (1 << 1)
#define SYS_WS_READY (1 << 2)
