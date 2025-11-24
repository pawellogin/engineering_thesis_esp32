#include "clientListDTO.h"
#include <ArduinoJson.h>

String serializeClientsDTO(const ClientsListDTO &dto)
{
    JsonDocument doc;

    doc["count"] = dto.count;

    JsonArray items = doc["items"].to<JsonArray>();

    for (uint8_t i = 0; i < dto.count; i++)
    {
        JsonObject clientObj = items.add<JsonObject>();
        clientObj["ip"] = dto.items[i].ip.toString();
        clientObj["lastSeen"] = dto.items[i].lastSeen;
        clientObj["connected"] = dto.items[i].connected;
        clientObj["boardId"] = dto.items[i].boardId;
    }

    String output;
    serializeJson(doc, output);
    return output;
}
