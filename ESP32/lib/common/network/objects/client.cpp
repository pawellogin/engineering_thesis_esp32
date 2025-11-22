#include "client.h"

ClientInfo clients[max_clients];

ClientRegisterResult registerClient(IPAddress ip)
{
    unsigned long now = millis();
    int freeIndex = -1;

    for (size_t i = 0; i < max_clients; ++i)
    {
        if (clients[i].connected)
        {
            if (clients[i].ip == ip)
            {
                clients[i].lastSeen = now;
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
        clients[freeIndex] = {ip, now, true};
        LOG_INFO("Client registered: %s", ip.toString().c_str());
        return ClientRegisterResult::NEW_REGISTERED;
    }

    LOG_INFO("No space for new client: %s", ip.toString().c_str());
    return ClientRegisterResult::NO_SPACE;
}