#include "system/clients/clientsManager.h"
#include "network/udp/udpManager.h"

ClientInfo clients[max_clients];

void initClients()
{
    for (int id = first_client_board_id; id < first_client_board_id + max_clients; id++)
    {
        int idx = id - 2;
        clients[idx].boardId = id;
        clients[idx].ip = IPAddress(192, 168, 137, 100 + id);
        clients[idx].lastSeen = 0;
        clients[idx].state = ClientState::CLIENT_OFFLINE;
    }
}

void onClientPacket(int boardID)
{

    // LOG_INFO("onClientPacket check board result %d", checkBoardID(boardID));
    if (!checkClientBoardID(boardID))
    {
        return;
    }

    ClientInfo &c = clients[boardID - first_client_board_id];
    c.lastSeen = millis();

    // LOG_INFO("onClientPacket");
    if (c.state == ClientState::CLIENT_OFFLINE)
    {
        c.state = ClientState::CLIENT_ONLINE;
        LOG_INFO("Client %d ONLINE", boardID);
    }
}

bool checkBoardID(int id)
{
    if (id < gateway_board_id || id > first_client_board_id + max_clients)
    {
        LOG_ERROR("Invalid boardID %d", id);
        return false;
    }
    return true;
}

bool checkClientBoardID(int id)
{
    if (id < first_client_board_id || id > first_client_board_id + max_clients)
    {
        // LOG_ERROR("Invalid client boardID %d", id);
        return false;
    }
    return true;
}

void clientPollTask(void *p)
{
    static char out[UDP_DATA_MAX];
    static size_t len;
    UdpMessageDTO msg;

    while (1)
    {
        buildUdpMessage(msg);
        msg.action = UdpMessageAction::STATUS_REQUEST;
        msg.type = UdpMessageType::COMMAND;
        msg.data[0] = '\0';

        bool ok = serializeUdpMessage(msg, out, sizeof(out), len);
        if (!ok)
        {
            LOG_ERROR("serialization error in clientPollTask");
            vTaskDelete(NULL);
        }
        else
        {
            udpSendAllClients(out, false, false);
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    }
}

void clientWatchdogTask(void *)
{
    while (1)
    {
        uint32_t now = millis();

        for (int i = 0; i < max_clients; i++)
        {
            ClientInfo &c = clients[i];

            if (c.state == ClientState::CLIENT_ONLINE &&
                now - c.lastSeen > offline_timeout)
            {
                c.state = ClientState::CLIENT_OFFLINE;
                LOG_INFO("Client %d OFFLINE", c.boardId);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
