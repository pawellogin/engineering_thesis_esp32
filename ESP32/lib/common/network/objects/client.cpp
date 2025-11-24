#include "client.h"

ClientInfo clients[max_clients];

// TODO it would be wise to improve this, make logic more clear, move some logic to separate functions
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
                clients[i].lastSeen = 0;
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