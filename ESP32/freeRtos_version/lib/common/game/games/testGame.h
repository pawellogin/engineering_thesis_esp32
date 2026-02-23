#pragma once
#include "game/gameBase.h"
#include <cstddef>

extern GameBase testGame;

// size_t gameResultToJson(const GameResult &game, char *buffer, size_t bufferSize);

void gameResultToJson(char *out,
                      size_t outSize,
                      const GameResult *game,
                      size_t count);