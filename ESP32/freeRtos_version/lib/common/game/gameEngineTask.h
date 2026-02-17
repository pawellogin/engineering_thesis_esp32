#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

void GameEngine_SystemInit();    // creates engine + queue
void GameEngine_Task(void *arg); // task entry
