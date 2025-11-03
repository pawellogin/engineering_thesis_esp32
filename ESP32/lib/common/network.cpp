#include "network.h"

bool connectToHotspot(const char *ssid, const char *password, unsigned long timeoutMs)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    DEBUG_PRINTF("[WiFi] Connecting to hotspot: %s\n", ssid);
    unsigned long start = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - start < timeoutMs)
    {
        delay(500);
        DEBUG_PRINT(".");
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        DEBUG_PRINTF("\n[WiFi] Connected. IP: %s\n", WiFi.localIP().toString().c_str());
        return true;
    }
    else
    {
        DEBUG_PRINTF("\n[WiFi] Connection failed.");
        WiFi.disconnect(true);
        return false;
    }
}

bool createAccessPoint(const char *ssid, const char *password, uint8_t channel, uint8_t maxConn)
{
    WiFi.mode(WIFI_AP);
    bool result = WiFi.softAP(ssid, password, channel, 0, maxConn);

    if (result)
    {
        IPAddress ip = WiFi.softAPIP();
        DEBUG_PRINTF("[WiFi] Access Point created: %s\n", ssid);
        DEBUG_PRINTF("[WiFi] AP IP: %s\n", ip.toString().c_str());
        return true;
    }
    else
    {
        DEBUG_PRINTF("[WiFi] Failed to create Access Point.");
    }
    return false;
}

void setupNetwork(WebSocketsServer *ws, std::function<void(uint8_t, WStype_t, uint8_t *, size_t)> wsEvent)
{
#if NETWORK_MODE == 1
    bool ok = connectToHotspot(sta_ssid, sta_password);
#elif NETWORK_MODE == 2
    bool ok = createAccessPoint(ap_ssid, ap_password, 1, max_clients);
#endif

    if (!ok)
    {
        DEBUG_PRINT("Network setup failed, skipping WebSocket start.");
        return;
    }

    if (ws == nullptr)
    {
        DEBUG_PRINT("WebSocket pointer is null!");
        return;
    }

    ws->begin();
    ws->onEvent(wsEvent);
    DEBUG_PRINTF("WebSocket server started on port %u", udp_port + 1);

    setWebSocket(ws);
}

IPAddress getLocalIP()
{
    if (WiFi.getMode() & WIFI_AP)
        return WiFi.softAPIP();
    else
        return WiFi.localIP();
}
