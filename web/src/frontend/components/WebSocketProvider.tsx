import React, { useEffect, useState } from 'react';
import type { WSConfig } from '../../backend/websocket/types/WsTypes';
import { WebSocketContext } from '../../backend/websocket/WebSocketContext';
import { WebSocketManager } from '../../backend/websocket/WebSocketManager';

const STORAGE_KEY = 'ws:config';

export const WebSocketProvider: React.FC<{ children: React.ReactNode; }> = ({ children }) => {
    const [manager] = useState(() => new WebSocketManager(""));  // always exists
    const [status, setStatus] = useState('idle');
    const [config, setConfigState] = useState<WSConfig | null>(null);

    // Load saved config
    useEffect(() => {
        const raw = localStorage.getItem(STORAGE_KEY);
        if (raw) {
            try {
                setConfigState(JSON.parse(raw));
            } catch {
                console.warn("Invalid config in storage");
            }
        } else {
            // setConfigState({ url: "ws://esp-gateway.local:8001" });
            setConfigState({ url: "ws://192.168.137.101:8001" });
            console.log("missing ws url");
        }
    }, []);

    // React to config change
    useEffect(() => {
        if (!config?.url) {
            manager.disconnect();
            setStatus('idle');
            return;
        }

        manager.setUrl(config.url);
        manager.connect();

        const t = setInterval(() => setStatus(manager.getStatus()), 300);
        return () => clearInterval(t);
    }, [config, manager]);

    // Expose config setter
    const setConfig = (c: WSConfig | null) => {
        setConfigState(c);
        if (c) localStorage.setItem(STORAGE_KEY, JSON.stringify(c));
        else localStorage.removeItem(STORAGE_KEY);
    };

    return (
        <WebSocketContext.Provider value={{ manager, status: status as any, setConfig }}>
            {children}
        </WebSocketContext.Provider>
    );
};
