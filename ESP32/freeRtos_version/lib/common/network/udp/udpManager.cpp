#include "network/udp/udpManager.h"
#include "core/debug.h"
#include "core/config.h"

WiFiUDP udp;
SemaphoreHandle_t udpMutex = NULL;

/**
 * Remember to use this only in this file and not in multiple tasks
 */
char incomingPacket[255];
ClientInfo clients[max_clients];

void udpInit()
{
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
    }
}

void udpHandlePacket()
{
    int packetSize = udp.parsePacket();
    if (packetSize)
    {
        int len = udp.read(incomingPacket, sizeof(incomingPacket) - 1);
        if (len > 0)
            incomingPacket[len] = '\0';

        IPAddress ip = udp.remoteIP();

        UdpMessageDTO msg;
        bool deserializeResult = deserializeUdpMessage((uint8_t *)incomingPacket, len, msg);

        ClientRegisterResult registerResult = registerClient(ip, (uint8_t)msg.data.toInt());

        if (registerResult == ClientRegisterResult::NEW_REGISTERED)
        {
            // TODO
            // gatewayUtilsRegistrationAck(ip);
        }

        LOG_INFO("UDP Received from %s: %s\n", ip.toString().c_str(), incomingPacket);

        // TODO change this to use freeRtos queue
        if (deserializeResult)
        {
            switch (msg.type)
            {
            case UdpMessageType::COMMAND:
                // proccessClientCommand(msg, ip);
                // TODO
                LOG_INFO("udp message commands");
                break;
            // case UdpMessageType::STATUS:
            case UdpMessageType::ERROR:
            default:
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
    default:
        LOG_ERROR("Missing typeToString conversion");
        return "unknown";
    }
}

String serializeUdpMessage(const UdpMessageDTO &msg)
{
    JsonDocument doc;
    doc["type"] = typeToString(msg.type);
    doc["action"] = actionToString(msg.action);

    if (msg.data.length() > 0)
    {
        JsonDocument dataDoc;
        DeserializationError err = deserializeJson(dataDoc, msg.data);
        if (!err)
            doc["data"] = dataDoc;
        else
            doc["data"] = msg.data;
    }

    doc["timestamp"] = msg.timestamp;

    String json;
    serializeJson(doc, json);
    return json;
}

bool deserializeUdpMessage(const uint8_t *payload, size_t length, UdpMessageDTO &msg)
{
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload, length);
    if (error)
    {
        LOG_ERROR("Invalid JSON in udp message");
        String raw = String((const char *)payload).substring(0, length);
        LOG_ERROR("Raw JSON: %s", raw.c_str());
        return false;
    }

    if (doc["type"].is<const char *>())
    {
        msg.type = typeFromString(doc["type"].as<String>());
    }
    else
    {
        LOG_INFO("Missing 'type' property in json");
    }

    if (doc["action"].is<const char *>())
    {
        msg.action = actionFromString(doc["action"].as<String>());
    }
    else
    {
        LOG_INFO("Missing 'action' property in json");
    }

    // TODO handle this better, rn if we have number in json this do not work correct
    if (!doc["data"].isNull())
    {
        msg.data = String(doc["data"].as<String>());
    }
    else
    {
        msg.data = "NO DATA ERROR";
    }

    msg.timestamp = doc["timestamp"] | millis();
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