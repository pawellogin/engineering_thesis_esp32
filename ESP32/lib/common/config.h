#pragma once

#define DEBUG_ENABLED true
#define NETWORK_MODE 1

// Wi-Fi configuration (declarations only)
extern const char *ap_ssid;
extern const char *ap_password;

extern const char *sta_ssid;
extern const char *sta_password;

// General configuration
extern const int max_clients;
extern unsigned int udp_port;
extern unsigned int ws_port;

//  Logging Configuration
#define LOG_LEVEL_NONE 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3

// Set current log level here
#define LOG_LEVEL LOG_LEVEL_DEBUG