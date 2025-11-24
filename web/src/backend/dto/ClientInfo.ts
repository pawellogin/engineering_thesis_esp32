
export type ClientInfo =
    {
        // IPAddress ip;
        ip: string,
        // unsigned long lastSeen; // for timeout / ping
        lastSeen: number,
        // bool connected;
        connected: boolean,
        // uint8_t boardId;
        boardId: number;
    };