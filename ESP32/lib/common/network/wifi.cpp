#include "wifi.h"

bool connectToHotspot(const char *ssid, const char *password, unsigned long timeoutMs)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    LOG_DEBUG("[WiFi] Connecting to hotspot: %s", ssid);
    unsigned long start = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - start < timeoutMs)
    {
        delay(500);
        LOG_DEBUG_INLINE(".");
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        LOG_DEBUG("");
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

bool setupNetwork()
{
#if NETWORK_MODE == 1
    return connectToHotspot(sta_ssid, sta_password);
#elif NETWORK_MODE == 2
    return createAccessPoint(ap_ssid, ap_password, 1, max_clients);
#endif
}

IPAddress getLocalIP()
{
    if (WiFi.getMode() & WIFI_AP)
        return WiFi.softAPIP();
    else
        return WiFi.localIP();
}
