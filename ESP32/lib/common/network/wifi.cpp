#include "wifi.h"

bool connectToHotspot(const char *ssid, const char *password, unsigned long timeoutMs)
{
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
        LOG_DEBUG("Connected. IP: %s", WiFi.localIP().toString().c_str());

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

bool createAccessPoint(const char *ssid, const char *password, uint8_t channel, uint8_t maxConn)
{
    WiFi.mode(WIFI_AP);
    bool result = WiFi.softAP(ssid, password, channel, 0, maxConn);

    if (result)
    {
        IPAddress ip = WiFi.softAPIP();
        LOG_DEBUG("Access Point created: %s", ssid);
        LOG_DEBUG("AP IP: %s", ip.toString().c_str());

        if (MDNS.begin(mDNS_hostname))
        {
            LOG_DEBUG("mDNS started for AP mode: mDNS_hostname.local");
        }
        else
        {
            LOG_DEBUG("mDNS failed to start in AP mode");
        }

        return true;
    }
    else
    {
        LOG_DEBUG("Failed to create Access Point.");
        return false;
    }
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
