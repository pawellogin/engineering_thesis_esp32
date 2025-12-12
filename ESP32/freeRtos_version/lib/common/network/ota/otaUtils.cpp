#include "otaUtils.h"

void clientOtaInit()
{
    uint64_t chipid = ESP.getEfuseMac(); // 64-bit unique MAC
    char otaHostname[32];
    sprintf(otaHostname, "ESP32_Client_%04X", (uint16_t)(chipid >> 32));
    ArduinoOTA.setHostname(otaHostname);

    LOG_INFO("%s", otaHostname);

    ArduinoOTA.setPassword(ota_password);

    ArduinoOTA.onStart([]()
                       { LOG_INFO("OTA Start"); });
    ArduinoOTA.onEnd([]()
                     { LOG_INFO("OTA End"); });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                          { LOG_INFO("OTA Progress: %u%%", (progress * 100) / total); });
    ArduinoOTA.onError([](ota_error_t error)
                       { LOG_ERROR("OTA Error[%u]", error); });

    ArduinoOTA.begin();
    LOG_INFO("OTA ready");
}

void clientOtaLoop()
{
    ArduinoOTA.handle();
}