#include "gameBase.h"
#include <cstddef>
#include "core/debug.h"

inline bool append(char *out, size_t outSize, size_t &offset, const char *fmt, ...)
{
    if (offset >= outSize)
        return false;

    va_list args;
    va_start(args, fmt);
    int written = vsnprintf(out + offset, outSize - offset, fmt, args);
    va_end(args);

    if (written < 0 || (size_t)written >= outSize - offset)
    {
        out[outSize - 1] = '\0';
        return false;
    }

    offset += written;
    return true;
}

void gameResultToJson(char *out,
                      size_t outSize,
                      const GameResult *game,
                      size_t count)
{
    if (outSize == 0)
    {
        LOG_ERROR("gameResultToJson e1");
        return;
    }

    size_t offset = 0;

    if (!append(out, outSize, offset, "["))
    {
        LOG_ERROR("gameResultToJson e2");
        return;
    }

    bool first = true;

    for (size_t i = 0; i < count; i++)
    {
        const GameClientResult &r = game->results[i];

        // optional filter (skip invalid entries)
        if (!r.valid)
        {
            LOG_ERROR("gameResultToJson e3");
            continue;
        }

        if (!append(out, outSize, offset,
                    "%s{\"valid\":%s,\"board_id\":%u,\"score\":%u,\"responded\":%s}",
                    first ? "" : ",",
                    r.valid ? "true" : "false",
                    r.board_id,
                    r.score,
                    r.responded ? "true" : "false"))
        {
            LOG_ERROR("gameResultToJson e4");
            return;
        }

        first = false;
    }

    append(out, outSize, offset, "]");
}
