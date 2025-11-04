import { useCallback, useContext, useEffect, useMemo, useState } from 'react';
import type { MessageAction, MessageDTO, MessageType } from './types/WsTypes';
import { WebSocketContext } from './WebSocketContext';


export function useWebSocket() {
    const { manager, status } = useContext(WebSocketContext);
    const [lastMessage, setLastMessage] = useState<MessageDTO | null>(null);


    useEffect(() => {
        if (!manager) return () => { };

        const unsub = manager.subscribe(null, (m) => setLastMessage(m));
        return () => {
            unsub();
        };
    }, [manager]);



    const send = useCallback((msg: MessageDTO) => manager?.send(msg), [manager]);

    const sendCommand = useCallback((action: MessageAction, data?: unknown) => manager?.sendCommand(action, data), [manager]);

    const subscribe = useCallback((type: MessageType | null, handler: (m: MessageDTO) => void) => manager?.subscribe(type, handler), [manager]);


    return useMemo(() => ({
        send,
        sendCommand,
        subscribe,
        status,
        lastMessage,
        manager,
    }), [send, sendCommand, subscribe, status, lastMessage, manager]);
}