#pragma once
#include <IPAddress.h>
#include <Arduino.h>

// TODO proably move every initialization to .h file, remove config.cpp file
#define DEBUG_ENABLED true
#define NETWORK_MODE 1 // 1=STA, 2=AP, AP == local wifi, STA == connects to hotspot

// Led pins
#define BUILTIN_LED 2
#define BUTTON_LED 25

// Wi-Fi configuration (declarations only)
extern const char *ap_ssid;
extern const char *ap_password;

extern const char *sta_ssid;
extern const char *sta_password;
extern const unsigned int wifi_timeout;

extern const char *mDNS_hostname;

// OTA config (Over The Air)
extern const char *ota_password;

extern IPAddress gatewayIp;
extern IPAddress subnetIp;

// UDP configuration
constexpr int max_clients = 10;
constexpr unsigned int udp_port = 8000;
constexpr unsigned int ws_port = udp_port + 1;

constexpr unsigned int gateway_clients_ping_check_timeout = 5000;

//  Logging Configuration
#define LOG_LEVEL_NONE 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3

// Set current log level here
#define LOG_LEVEL LOG_LEVEL_DEBUG