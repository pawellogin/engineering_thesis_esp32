#include "network/wifi/wifiManager.h"
#include "core/debug.h"
#include "core/config.h"
#include "network/udp/udpManager.h"
#include <ESPmDNS.h>
#include "network/websocket/websocketManager.h"
#include "system/systemContext.h"

IPAddress getStaticIP()
{
    return IPAddress(192, 168, 137, 100 + BOARD_ID);
}

IPAddress getHotspot()
{
    return IPAddress(192, 168, 137, 1);
}

IPAddress getSubnet()
{
    return IPAddress(255, 255, 255, 0);
}

bool connectToHotspot(const char *ssid, const char *password, unsigned long timeoutMs)
{

    WiFi.config(getStaticIP(), getHotspot(), getSubnet());
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    LOG_DEBUG("Connecting to hotspot: %s", ssid);
    unsigned long start = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - start < timeoutMs)
    {
        delay(500);
        LOG_DEBUG_INLINE(".");
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        LOG_DEBUG("\nConnected. IP: %s", WiFi.localIP().toString().c_str());

        if (MDNS.begin(mDNS_hostname))
        {
            LOG_DEBUG("mDNS started: %s.local", mDNS_hostname);
        }
        else
        {
            LOG_DEBUG("mDNS failed to start");
        }

        return true;
    }
    else
    {
        LOG_DEBUG("Connection failed.");
        WiFi.disconnect(true);
        return false;
    }
}

static void wifiEventHandler(WiFiEvent_t event)
{
    switch (event)
    {
    case SYSTEM_EVENT_STA_GOT_IP:
        xEventGroupSetBits(sys.systemEvents, SYS_WIFI_READY);
        udpInit();
        webSocketInit();
        break;

    case SYSTEM_EVENT_STA_DISCONNECTED:
        xEventGroupClearBits(
            sys.systemEvents,
            SYS_WIFI_READY | SYS_UDP_READY | SYS_WS_READY);
        break;

    default:
        break;
    }
}

void wifiInit()
{
    WiFi.onEvent(wifiEventHandler);
    WiFi.mode(WIFI_STA);
    WiFi.config(getStaticIP(), getHotspot(), getSubnet());
}

void wifiTask(void *p)
{
    while (true)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            LOG_INFO("WiFi connecting...");
            WiFi.begin(sta_ssid, sta_password);
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
