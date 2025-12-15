#pragma once
#include "WiFi.h"

extern WiFiUDP udp;

struct ClientInfo
{
    IPAddress ip;
    unsigned long lastSeen; // for timeout / ping
    bool connected;
    uint8_t boardId;
};

struct ClientsListDTO
{
    // TODO remove count
    uint8_t count;
    ClientInfo *items;
};

extern ClientInfo clients[];

enum class ClientRegisterResult
{
    ALREADY_REGISTERED,
    NEW_REGISTERED,
    NO_SPACE
};

ClientRegisterResult registerClient(IPAddress ip, uint8_t boardId);

ClientsListDTO getClientsListDTO();

enum class UdpMessageType
{
    UNKNOWN,
    COMMAND,
    // STATUS,
    // EVENT,
    ERROR,
};

enum class UdpMessageAction
{
    UNKNOWN,
    RESTART_ALL,
    BLINK_BUILTIN_LED,
    REGISTRATION_ACK,
    PING,

    TEST_GAME_START,
    TEST_GAME_STATUS,
    TEST_GAME_END,
};

/*
UdpMessage is for transfering data between client and gateway
*/
struct UdpMessageDTO
{
    UdpMessageType type;
    UdpMessageAction action;
    String data;             // raw JSON string or text
    unsigned long timestamp; // optional, set with millis()
};

String serializeUdpMessage(const UdpMessageDTO &msg);
bool deserializeUdpMessage(const uint8_t *payload, size_t length, UdpMessageDTO &msg);

void udpInit();

void udpSendAll(const char *msg);

void udpHandlePacket();

void udpTask(void *p);