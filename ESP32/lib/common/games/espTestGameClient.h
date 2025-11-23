#pragma once
#include "gameBase.h"
#include "IO/ledUtils.h"
#include "IO/buttonUtils.h"

struct EspTestGameClient
{
    Game base;
    int score;
};

extern EspTestGameClient espTestGameClient;
void setupEspTestGameClient();
