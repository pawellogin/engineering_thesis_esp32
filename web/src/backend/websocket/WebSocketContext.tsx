import React from 'react';
import { WebSocketManager } from './WebSocketManager';
import type { WSConfig, WSStatus } from './types/WsTypes';

export interface IWebSocketContext {
    manager: WebSocketManager;
    status: WSStatus;
    setConfig: (c: WSConfig | null) => void;
}

export const WebSocketContext = React.createContext<IWebSocketContext>({
    manager: new WebSocketManager(""), // always defined, empty URL initially
    status: 'idle',
    setConfig: () => { },
});
