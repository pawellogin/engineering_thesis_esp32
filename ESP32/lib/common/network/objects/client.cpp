#include "client.h"

bool registerClient(IPAddress ip)
{
    for (size_t i = 0; i < max_clients; ++i)
    {
        if (clients[i].connected && clients[i].ip == ip)
        {
            clients[i].lastSeen = millis();
            return true; // already registered
        }
    }

    for (size_t i = 0; i < max_clients; ++i)
    {
        if (!clients[i].connected)
        {
            clients[i] = {ip, millis(), true};
            LOG_INFO("Client registered: %s\n", ip.toString().c_str());
            return true;
        }
    }

    LOG_INFO("No space for new client: %s\n", ip.toString().c_str());
    return false; // max clients reached
}