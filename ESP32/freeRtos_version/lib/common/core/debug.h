#pragma once
#include <Arduino.h>
#include "config.h"
#include <ArduinoJson.h>
#include <WebSocketsServer.h>

// Logging Macros
// ERROR
#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define LOG_ERROR(fmt, ...) Serial.printf("[ERROR] " fmt "\n", ##__VA_ARGS__)
#define LOG_ERROR_INLINE(fmt, ...) Serial.printf("" fmt, ##__VA_ARGS__)
#else
#define LOG_ERROR(fmt, ...)
#define LOG_ERROR_INLINE(fmt, ...)
#endif

// INFO
#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOG_INFO(fmt, ...) Serial.printf("[INFO]  " fmt "\n", ##__VA_ARGS__)
#define LOG_INFO_INLINE(fmt, ...) Serial.printf("" fmt, ##__VA_ARGS__)
#else
#define LOG_INFO(fmt, ...)
#define LOG_INFO_INLINE(fmt, ...)
#endif

// DEBUG
#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LOG_DEBUG(fmt, ...) Serial.printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)
#define LOG_DEBUG_INLINE(fmt, ...) Serial.printf("" fmt, ##__VA_ARGS__)
#else
#define LOG_DEBUG(fmt, ...)
#define LOG_DEBUG_INLINE(fmt, ...)
#endif

void restartESP();

static inline constexpr bool isGateway(void)
{
    return BOARD_ID == gateway_board_id;
}
