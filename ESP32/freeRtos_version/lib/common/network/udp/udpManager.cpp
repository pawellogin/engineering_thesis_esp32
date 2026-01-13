#include "network/udp/udpManager.h"
#include "core/debug.h"
#include "core/config.h"
#include "drivers/ledManager.h"
#include <ESPmDNS.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic error "-Wswitch-enum"

WiFiUDP udp;
QueueHandle_t udpCommandQueue;
SemaphoreHandle_t udpMutex = NULL;

/**
 * Remember to use incomingPacket only in this file and not in multiple tasks
 */
char incomingPacket[255];
ClientInfo clients[max_clients];

/**
 * right now this is not used, probably remove
 */
IPAddress dynamicGatewayIp;

void udpInit()
{
    udpCommandQueue = xQueueCreate(16, sizeof(UdpMessageDTO));
    configASSERT(udpCommandQueue);

    if (udp.begin(udp_port))
    {
        LOG_INFO("UDP Listening on port %d\n", udp_port);

        udpMutex = xSemaphoreCreateMutex();
        if (udpMutex == NULL)
        {
            LOG_ERROR("Fatal error - no heap");
            while (true)
            {
            }
        }
    }
    else
    {
        LOG_INFO("UDP Failed to start.");
        while (1)
            ;
    }

    // discoverIPFromMDNS(mDNS_hostname, wifi_timeout);
}

void udpHandlePacket()
{
    IPAddress localIp = WiFi.localIP();
    IPAddress ip = udp.remoteIP();

    if (ip == localIp)
    {
        return; // ignore own packets
    }

    int packetSize = udp.parsePacket();
    if (packetSize)
    {
        int len = udp.read(incomingPacket, sizeof(incomingPacket) - 1);
        if (len > 0)
            incomingPacket[len] = '\0';

        UdpMessageDTO msg;
        bool deserializeResult = deserializeUdpMessage((uint8_t *)incomingPacket, len, msg);

        LOG_INFO("UDP RX from %s, action=%d, ts=%lu", ip.toString().c_str(), (int)msg.action, msg.timestamp);

        if (!deserializeResult)
        {
            LOG_ERROR("deserializeUdpMessage failed");
            return;
        }

        ClientRegisterResult registerResult = registerClient(ip, (uint8_t)atoi(msg.data));

        if (registerResult == ClientRegisterResult::NEW_REGISTERED)
        {
            // TODO
            // gatewayUtilsRegistrationAck(ip);
        }

        LOG_INFO("UDP Received from %s: %s\n", ip.toString().c_str(), incomingPacket);

        if (deserializeResult)
        {
            switch (msg.type)
            {
            case UdpMessageType::COMMAND:
                xQueueSend(udpCommandQueue, &msg, 0);
                break;
            case UdpMessageType::UNKNOWN:
            case UdpMessageType::ERROR:
            default:
                LOG_ERROR("UNKNOWN or ERROR or missing udp msg type");
                break;
            }
        }
    }
}

ClientRegisterResult registerClient(IPAddress ip, uint8_t boardId)
{
    unsigned long now = millis();
    int freeIndex = -1;

    for (size_t i = 0; i < max_clients; ++i)
    {
        if (clients[i].connected)
        {
            if (clients[i].ip == ip)
            {
                clients[i].lastSeen = now - clients[i].lastSeen;
                return ClientRegisterResult::ALREADY_REGISTERED;
            }
        }
        else if (freeIndex < 0)
        {
            freeIndex = i;
        }
    }

    if (freeIndex >= 0)
    {
        LOG_INFO("INNER BOARD ID %d", boardId);
        clients[freeIndex] = {ip, now, true, boardId};
        LOG_INFO("Client registered: %s", ip.toString().c_str());
        return ClientRegisterResult::NEW_REGISTERED;
    }

    LOG_INFO("No space for new client: %s", ip.toString().c_str());
    return ClientRegisterResult::NO_SPACE;
}

ClientsListDTO getClientsListDTO()
{
    static ClientInfo buffer[max_clients];
    uint8_t count = 0;

    for (int i = 0; i < max_clients; i++)
    {
        if (!clients[i].connected)
            continue;

        buffer[count++] = clients[i];
    }

    ClientsListDTO dto;
    dto.count = count;
    dto.items = buffer;
    return dto;
}

static UdpMessageType typeFromString(const String &typeStr)
{
    if (typeStr == "command")
        return UdpMessageType::COMMAND;
    else if (typeStr == "error")
        return UdpMessageType::ERROR;
    // else if (typeStr == "status")
    //     return UdpMessageType::STATUS;
    else
    {
        LOG_ERROR("Missing typeFromString conversion");
        return UdpMessageType::UNKNOWN; // Unknown
    }
}
static UdpMessageAction actionFromString(const String &actionStr)
{

    // TODO find a way to check this for every possibility, switch wont work here i think
    if (actionStr == "restart")
        return UdpMessageAction::RESTART_ALL;
    else if (actionStr == "blink_builtin_led")
        return UdpMessageAction::BLINK_BUILTIN_LED;
    else if (actionStr == "registration_ack")
        return UdpMessageAction::REGISTRATION_ACK;
    else if (actionStr == "ping")
        return UdpMessageAction::PING;
    else if (actionStr == "esp_game_test")
        return UdpMessageAction::TEST_GAME_START;
    else if (actionStr == "test_game_status")
        return UdpMessageAction::TEST_GAME_STATUS;
    else if (actionStr == "test_game_end")
        return UdpMessageAction::TEST_GAME_END;
    else
    {
        LOG_ERROR("Missing udp actionFromString conversion");
        return UdpMessageAction::UNKNOWN; // Unknown
    }
}

static String actionToString(UdpMessageAction action)
{
    switch (action)
    {
    case UdpMessageAction::RESTART_ALL:
        return "restart";
    case UdpMessageAction::BLINK_BUILTIN_LED:
        return "blink_builtin_led";
    case UdpMessageAction::REGISTRATION_ACK:
        return "registration_ack";
    case UdpMessageAction::TEST_GAME_START:
        return "esp_game_test";
    case UdpMessageAction::TEST_GAME_STATUS:
        return "test_game_status";
    case UdpMessageAction::TEST_GAME_END:
        return "test_game_end";
    case UdpMessageAction::PING:
        return "ping";
    case UdpMessageAction::UNKNOWN:
    default:
        LOG_ERROR("Missing actionToString conversion");
        return "unknown";
    }
}

static String typeToString(UdpMessageType type)
{
    switch (type)
    {
    case UdpMessageType::COMMAND:
        return "command";
    case UdpMessageType::ERROR:
        return "error";
    // case UdpMessageType::STATUS:
    //     return "status";
    case UdpMessageType::UNKNOWN:
    default:
        LOG_ERROR("Missing typeToString conversion");
        return "unknown";
    }
}

/*
 * outSize is the maximum size a serialized message can be
 * outlEN is actual size of serialized message
 */
bool serializeUdpMessage(
    const UdpMessageDTO &msg,
    char *out,
    size_t outSize,
    size_t &outLen)
{
    StaticJsonDocument<256> doc;

    doc["type"] = typeToString(msg.type);
    doc["action"] = actionToString(msg.action);
    doc["timestamp"] = msg.timestamp;

    if (msg.data[0] != '\0')
    {
        doc["data"] = msg.data;
    }

    outLen = serializeJson(doc, out, outSize);
    return outLen > 0 && outLen < outSize;
}

bool deserializeUdpMessage(const uint8_t *payload, size_t length, UdpMessageDTO &msg)
{
    StaticJsonDocument<256> doc;

    DeserializationError error = deserializeJson(doc, payload, length);
    if (error)
    {
        LOG_ERROR("Invalid JSON in UDP message");
        // Safe logging: copy a small portion of payload
        char tmp[UDP_DATA_MAX];
        size_t logLen = length < sizeof(tmp) - 1 ? length : sizeof(tmp) - 1;
        memcpy(tmp, payload, logLen);
        tmp[logLen] = '\0';
        LOG_ERROR("Raw JSON: %s", tmp);
        return false;
    }

    if (doc["type"].is<const char *>())
    {
        const char *typeStr = doc["type"];
        msg.type = typeFromString(typeStr);
    }
    else
    {
        LOG_ERROR("Missing or invalid 'type' property");
        msg.type = UdpMessageType::UNKNOWN;
    }

    if (doc["action"].is<const char *>())
    {
        const char *actionStr = doc["action"];
        msg.action = actionFromString(actionStr);
    }
    else
    {
        LOG_ERROR("Missing or invalid 'action' property");
        msg.action = UdpMessageAction::UNKNOWN;
    }

    if (!doc["data"].isNull())
    {
        if (doc["data"].is<const char *>())
        {
            strlcpy(msg.data, doc["data"], sizeof(msg.data));
        }
        else if (doc["data"].is<long>() || doc["data"].is<int>())
        {
            snprintf(msg.data, sizeof(msg.data), "%ld", doc["data"].as<long>());
        }
        else if (doc["data"].is<float>() || doc["data"].is<double>())
        {
            snprintf(msg.data, sizeof(msg.data), "%.6f", doc["data"].as<double>());
        }
        else
        {
            LOG_ERROR("Unsupported 'data' type, clearing field");
            msg.data[0] = '\0';
        }

        if (strlen(msg.data) >= sizeof(msg.data) - 1)
        {
            LOG_ERROR("'data' field truncated to %d bytes", sizeof(msg.data) - 1);
        }
    }
    else
    {
        msg.data[0] = '\0'; // no data
    }

    if (doc["timestamp"].is<unsigned long>())
    {
        msg.timestamp = doc["timestamp"];
    }
    else
    {
        LOG_ERROR("Missing or invalid 'timestamp', using millis()");
        msg.timestamp = millis();
    }

    return true;
}
void udpTask(void *p)
{
    while (true)
    {
        udpHandlePacket();
        vTaskDelay(10 / portTICK_PERIOD_MS); // small delay to yield CPU
    }
}

void udpProccessCommand(const UdpMessageDTO &msg)
{
    switch (msg.action)
    {
        // TODO maybe make separate action for gateway and clients restart
    case UdpMessageAction::RESTART_ALL:
        // gatewayUtilsRestartClients();
        restartESP();
        break;
    case UdpMessageAction::BLINK_BUILTIN_LED:
        ledBlink(builtInLed, clientLedMutex);
        break;
    case UdpMessageAction::REGISTRATION_ACK:
    case UdpMessageAction::PING:
    case UdpMessageAction::TEST_GAME_START:
    case UdpMessageAction::TEST_GAME_STATUS:
    case UdpMessageAction::TEST_GAME_END:
        break;
    case UdpMessageAction::UNKNOWN:
    default:
        LOG_ERROR("Unknown command action");
        break;
    }
}

void udpCommandTask(void *p)
{
    UdpMessageDTO msg;

    while (true)
    {
        msg.action = UdpMessageAction::UNKNOWN;

        auto a = xQueueReceive(udpCommandQueue, &msg, portMAX_DELAY);

        LOG_INFO("queue recive %d", a);

        LOG_INFO("action: %d, time: %d", msg.action, msg.timestamp);

        udpProccessCommand(msg);

        // if (xQueueReceive(udpCommandQueue, &msg, portMAX_DELAY) == pdTRUE)
        // {
        //     // TODO add function to process the type, and then inside it the action
        //     udpProccessCommand(msg);
        // }
    }
}

void udpSend(IPAddress ip, const char *msg)
{
    udp.beginPacket(ip, udp_port);
    udp.print(msg);
    udp.endPacket();
    LOG_INFO("UDP Sent to %s: %s\n", ip.toString().c_str(), msg);
}

void udpSendAllClients(const char *msg)
{
    for (size_t i = 0; i < max_clients; ++i)
    {
        if (clients[i].connected)
        {
            udpSend(clients[i].ip, msg);
        }
    }
}

void updClientSendDiscoverPingTask(void *p)
{
    UdpMessageDTO msg;

    while (true)
    {

        msg.action = UdpMessageAction::PING;
        msg.data[0] = '\0';
        msg.timestamp = millis();
        msg.type = UdpMessageType::COMMAND;

        char out[256];
        size_t len;

        bool ok = serializeUdpMessage(msg, out, sizeof(out), len);

        if (!ok)
        {
            LOG_ERROR("serialization error in updClientSendDiscoverPing ");
        }
        else
        {
            udpSend(gatewayIp, out);
        }
        vTaskDelay(1000 * 5 / portTICK_PERIOD_MS); // small delay to yield CPU
    }
}

IPAddress discoverIPFromMDNS(const char *hostname, unsigned long timeoutMs)
{

    LOG_INFO("Looking for gateway: %s.local", hostname);

    IPAddress gIP;
    unsigned long start = millis();

    while (gIP.toString() == "0.0.0.0" && millis() - start < timeoutMs)
    {
        gIP = MDNS.queryHost(hostname);
        delay(500);
    }

    if (gIP.toString() != "0.0.0.0")
    {
        LOG_INFO("Gateway found: %s -> %s",
                 hostname, gIP.toString().c_str());
    }
    else
    {
        LOG_ERROR("Failed to resolve gateway: %s.local", hostname);
    }

    return gIP;
}

#pragma GCC diagnostic pop
