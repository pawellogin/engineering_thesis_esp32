import React, { useContext, useEffect, useState } from 'react';
import { WebSocketContext } from '../../backend/websocket/WebSocketContext';

export const ConfigPanel: React.FC = () => {
    const { setConfig } = useContext(WebSocketContext);
    const [url, setUrl] = useState('');

    // Preload the current value from localStorage (optional)
    useEffect(() => {
        const raw = localStorage.getItem('ws:config');
        if (raw) {
            try {
                const parsed = JSON.parse(raw);
                if (parsed.url) setUrl(parsed.url);
            } catch { }
        } else {
            setUrl("ws://192.168.137.102:8001");
        }
    }, []);

    const apply = () => {
        if (!url.trim()) {
            setConfig(null);
        } else {
            setConfig({ url });
        }
    };

    return (
        <div style={{ padding: 8 }}>
            <label>WebSocket URL</label>
            <input
                value={url}
                onChange={(e) => setUrl(e.target.value)}
                style={{ width: '100%', marginBottom: 8 }}
            />
            <button onClick={apply}>Apply</button>
        </div>
    );
};
