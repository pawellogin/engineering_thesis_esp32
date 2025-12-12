import { useCallback, useContext, useEffect, useMemo, useState } from 'react';
import type { MessageAction, MessageDTO, MessageType } from './types/WsTypes';
import { WebSocketContext } from './WebSocketContext';
import type { WebSocketManager } from './WebSocketManager';


interface WebSocketHook {
    send: (msg: MessageDTO) => void;
    getWebsocketSetup: () => { status: string; url: string; };
    sendCommand: (action: MessageAction, data?: unknown) => void;
    sendStatus: (action: MessageAction, data?: unknown) => void;
    subscribe: (type: MessageType | null, handler: (m: MessageDTO) => void) => void;
    lastMessage: MessageDTO | null;
    manager: WebSocketManager;
}

export function useWebSocket() {
    const { manager } = useContext(WebSocketContext);
    const [lastMessage, setLastMessage] = useState<MessageDTO | null>(null);


    useEffect(() => {
        if (!manager) return () => { };

        const unsub = manager.subscribe(null, (m) => setLastMessage(m));
        return () => {
            unsub();
        };
    }, [manager]);



    const getWebsocketStatus = useCallback(() => ({
        status: manager.getStatus(),
        url: manager.getUrl(),
    }), [manager]);

    const send = useCallback((msg: MessageDTO) => manager?.send(msg), [manager]);

    const sendCommand: ((action: MessageAction, data?: unknown) => void) = useCallback((action: MessageAction, data?: unknown) => manager?.sendCommand(action, data), [manager]);

    const sendStatus = useCallback((action: MessageAction, data?: unknown) => manager?.sendStatus(action, data), [manager]);

    const subscribe = useCallback((type: MessageType | null, handler: (m: MessageDTO) => void) => manager?.subscribe(type, handler), [manager]);


    return useMemo<WebSocketHook>(() => ({
        send,
        getWebsocketSetup: getWebsocketStatus,
        sendCommand,
        sendStatus,
        subscribe,
        lastMessage,
        manager,
    }), [send, getWebsocketStatus, sendCommand, sendStatus, subscribe, lastMessage, manager]);
}