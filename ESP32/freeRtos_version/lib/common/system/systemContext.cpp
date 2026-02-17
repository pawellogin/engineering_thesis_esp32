#include "systemContext.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "network/udp/udpManager.h"
#include "network/websocket/websocketManager.h"

SystemContext sys;

void systemContextInit(bool isGateway)
{
    sys.isGateway = isGateway;
    sys.systemEvents = xEventGroupCreate();
    configASSERT(sys.systemEvents);

    sys.udpCommandQueue = xQueueCreate(16, sizeof(UdpMessageDTO));
    sys.wsCommandQueue = xQueueCreate(8, sizeof(WebMessageDTO));
    sys.gameEngineQueue = xQueueCreate(32, sizeof(GameEngineEvent));

    configASSERT(sys.udpCommandQueue);
    configASSERT(sys.wsCommandQueue);
    configASSERT(sys.gameEngineQueue);
}
