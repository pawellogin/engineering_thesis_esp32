#include "network.h"

bool connectToHotspot(const char *ssid, const char *password, unsigned long timeoutMs)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    LOG_DEBUG("[WiFi] Connecting to hotspot: %s", ssid);
    unsigned long start = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - start < timeoutMs)
    {
        delay(500);
        LOG_DEBUG(".");
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        LOG_DEBUG("[WiFi] Connected. IP: %s", WiFi.localIP().toString().c_str());
        return true;
    }
    else
    {
        LOG_DEBUG("[WiFi] Connection failed.");
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
        LOG_DEBUG("[WiFi] Access Point created: %s", ssid);
        LOG_DEBUG("[WiFi] AP IP: %s", ip.toString().c_str());
        return true;
    }
    else
    {
        LOG_DEBUG("[WiFi] Failed to create Access Point.");
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
        LOG_DEBUG("Network setup failed, skipping WebSocket start.");
        return;
    }

    if (ws == nullptr)
    {
        LOG_DEBUG("WebSocket pointer is null!");
        return;
    }

    ws->begin();
    ws->onEvent(wsEvent);
    LOG_DEBUG("WebSocket server started on port %u", udp_port + 1);

    setWebSocket(ws);
}

IPAddress getLocalIP()
{
    if (WiFi.getMode() & WIFI_AP)
        return WiFi.softAPIP();
    else
        return WiFi.localIP();
}
