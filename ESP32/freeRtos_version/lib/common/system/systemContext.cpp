#include "systemContext.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "network/udp/udpManager.h"
#include "network/websocket/websocketManager.h"

SystemContext sys;

void systemContextInit()
{
    sys.systemEvents = xEventGroupCreate();
    configASSERT(sys.systemEvents);

    sys.udpCommandQueue = xQueueCreate(16, sizeof(UdpMessageDTO));
    sys.wsCommandQueue = xQueueCreate(8, sizeof(WebMessageDTO));

    configASSERT(sys.udpCommandQueue);
    configASSERT(sys.wsCommandQueue);
}
