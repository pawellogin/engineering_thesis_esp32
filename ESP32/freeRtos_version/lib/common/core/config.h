#pragma once
#include <Arduino.h>
#include <IPAddress.h>

// =====================
// General Config
// =====================
inline constexpr bool DEBUG_ENABLED = true;
inline constexpr int NETWORK_MODE = 1; // 1=STA, 2=AP

// =====================
// IO pins
// =====================
inline constexpr int BUILTIN_LED = 2;
inline constexpr int BUTTON_LED = 25;
inline constexpr int MAIN_BUTTON = 5;
inline constexpr int LED_STRIP_PIXELS = 24;

// =====================
// Wi-Fi Credentials
// =====================
inline const char ap_ssid[] = "ESP32-Gateway";
inline const char ap_password[] = "12345678";

inline const char sta_ssid[] = "LENOVO20_4046";
inline const char sta_password[] = "12341234";

// inline constexpr unsigned int wifi_timeout = 1000 * 3600; // ms
inline constexpr unsigned int wifi_timeout_attempt = 1000 * 5; // 5s

inline const char mDNS_hostname[] = "esp-gateway";

// =====================
// OTA Config
// =====================
inline const char ota_password[] = "ota_password";

// =====================
// Network Config
// =====================
inline const IPAddress staticIp(192, 168, 137, 100 + BOARD_ID);
inline const IPAddress gatewayIp(192, 168, 137, 101);
inline const IPAddress subnetIp(255, 255, 255, 0);

#define WS_DATA_MAX 256

// =====================
// UDP Config
// =====================
inline constexpr int max_clients = 10;
inline constexpr unsigned int udp_port = 8000;
inline constexpr unsigned int ws_port = udp_port + 1;
inline constexpr unsigned int gateway_clients_ping_check_timeout = 5000;

// =====================
// Logging Levels
// =====================
enum LogLevel
{
    LOG_LEVEL_NONE,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG
};
inline constexpr LogLevel CURRENT_LOG_LEVEL = LOG_LEVEL_DEBUG;
