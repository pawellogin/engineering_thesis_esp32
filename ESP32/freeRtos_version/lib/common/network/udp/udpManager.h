#pragma once
#include "WiFi.h"

extern WiFiUDP udp;

// struct ClientInfo
// {
//     IPAddress ip;
//     unsigned long lastSeen; // for timeout / ping
//     bool connected;
//     uint8_t boardId;
// };

// struct ClientsListDTO
// {
//     // TODO remove count
//     uint8_t count;
//     ClientInfo *items;
// };

// extern ClientInfo clients[];

enum class ClientRegisterResult
{
    ALREADY_REGISTERED,
    NEW_REGISTERED,
    NO_SPACE
};

// ClientRegisterResult registerClient(IPAddress ip, uint8_t boardId);

// ClientsListDTO getClientsListDTO();

enum class UdpMessageType
{
    UNKNOWN,
    COMMAND,
    ERROR,
};

enum class UdpMessageAction
{
    UNKNOWN,
    RESTART_ALL,
    BLINK_BUILTIN_LED,
    REGISTRATION_ACK,
    STATUS_REQUEST,
    STATUS_RESPONSE,

    BUTTON_CLICK,
};

/*
UdpMessage is for transfering data between client and gateway
*/
#define UDP_DATA_MAX 256

struct UdpMessageDTO
{
    uint8_t boardID;
    UdpMessageType type;
    UdpMessageAction action;
    char data[UDP_DATA_MAX]; // raw JSON string or text
    unsigned long timestamp; // optional, set with millis()
};

void buildUdpMessage(UdpMessageDTO &msg);

/*
 * outSize is the maximum size a serialized message can be
 * outlEN is actual size of serialized message
 */
bool serializeUdpMessage(const UdpMessageDTO &msg, char *out, size_t outSize, size_t &outLen);
bool deserializeUdpMessage(const uint8_t *payload, size_t length, UdpMessageDTO &msg);

void udpInit();

void udpSend(IPAddress ip, const char *msg, bool showLog = true);

void udpSendAllClients(const char *msg, bool hasToBeOnline = true, bool showLog = true);

void udpHandlePacket();

void udpTask(void *p);

void udpProccessCommand(const UdpMessageDTO &msg);

void udpCommandTask(void *p);

// TODO remove
void updClientSendDiscoverPingTask(void *p);

// TODO remove
IPAddress discoverIPFromMDNS(const char *hostname, unsigned long timeoutMs);