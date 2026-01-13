#include <Arduino.h>
#include "core/debug.h"
#include "core/config.h"
#include "network/wifi/wifiManager.h"
#include "network/udp/udpManager.h"
#include "network/websocket/websocketManager.h"
#include "core/tasks.h"
#include "drivers/ledManager.h"

void setup()
{
    Serial.begin(115200);
    LOG_INFO("ESP32 client starting...");

    if (connectToHotspot(sta_ssid, sta_password, wifi_timeout))
    {
        udpInit();
    }
    else
    {
        LOG_ERROR("Connect to hotspot failed");
        while (true)
            ;
    }

    clientLedInitAll();

    startClientTasks();
}

void loop()
{
}