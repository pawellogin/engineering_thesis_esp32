#include "staticIpConfig.h"

IPAddress getStaticIP()
{
    return IPAddress(192, 168, 137, 100 + BOARD_ID);
}

IPAddress getGateway()
{
    return IPAddress(192, 168, 137, 1);
}

IPAddress getSubnet()
{
    return IPAddress(255, 255, 255, 0);
}
