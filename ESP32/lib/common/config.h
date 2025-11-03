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
