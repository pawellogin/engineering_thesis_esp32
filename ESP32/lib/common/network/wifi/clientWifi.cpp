#include "clientWifi.h"
#include "debug.h"
#include "network/wifi/staticIpConfig.h"

bool clientWiFiConnect(const char *ssid, const char *password, unsigned long timeoutMs)
{
    WiFi.config(getStaticIP(), getGateway(), getSubnet());
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    LOG_INFO("[WiFi] Connecting to: %s", ssid);

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < timeoutMs)
    {
        delay(500);
        LOG_DEBUG_INLINE(".");
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        LOG_INFO("\n[WiFi] Connected. IP: %s", WiFi.localIP().toString().c_str());
        return true;
    }
    else
    {
        LOG_ERROR("[WiFi] Connection failed.");
        WiFi.disconnect(true);
        return false;
    }
}

IPAddress discoverGatewayIP(const char *hostname, unsigned long timeoutMs)
{
    if (!MDNS.begin("esp-client")) // optional: client registers itself
        LOG_INFO("[mDNS] Client hostname registration failed");

    LOG_INFO("[mDNS] Looking for gateway: %s.local", hostname);

    IPAddress gatewayIP;
    unsigned long start = millis();

    while (gatewayIP.toString() == "0.0.0.0" && millis() - start < timeoutMs)
    {
        gatewayIP = MDNS.queryHost(hostname);
        delay(500);
    }

    if (gatewayIP.toString() != "0.0.0.0")
    {
        LOG_INFO("Gateway found: %s -> %s",
                 hostname, gatewayIP.toString().c_str());
    }
    else
    {
        LOG_ERROR("Failed to resolve gateway: %s.local", hostname);
    }

    return gatewayIP;
}
