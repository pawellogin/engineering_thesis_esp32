#include <Arduino.h>
#include <ArduinoJson.h>
#include <WebSocketsServer.h>
#include "config.h"

#if DEBUG_ENABLED
#define DEBUG_PRINT(x)     \
    do                     \
    {                      \
        Serial.println(x); \
    } while (0)

#define DEBUG_PRINTF(fmt, ...)                  \
    do                                          \
    {                                           \
        Serial.printf(fmt "\n", ##__VA_ARGS__); \
    } while (0)
#else
#define DEBUG_PRINT(x) \
    do                 \
    {                  \
    } while (0)
#define DEBUG_PRINTF(fmt, ...) \
    do                         \
    {                          \
    } while (0)
#endif

void heartbeatDebug();
void setWebSocket(WebSocketsServer *ws);
void broadcastDebug();
