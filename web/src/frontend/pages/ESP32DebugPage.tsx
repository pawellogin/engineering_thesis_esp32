/** @jsxImportSource @emotion/react */
import { css } from '@emotion/react';
import { useState } from 'react';
import { useWebSocket } from '../../backend/websocket/old/UseWebSocket';

export default function ESP32DebugPage() {

    const ws = useWebSocket();
    const [message, setMessage] = useState('');
    const [wsIp, setWsIp] = useState('ws://192.168.137.86:8001');
    const sendMessage = () => {
        if (ws && message.trim() !== '') {
            ws.sendTextMessage(message);
        }
    };

    return (
        <div css={containerCss}>
            <h1 css={titleCss}>ESP32 Debug Panel</h1>

            <div className="row">

                <div className="col">
                    <input
                        css={inputCss}
                        type="text"
                        value={wsIp}
                        onChange={(e) => setWsIp(e.target.value)}
                        placeholder="IP"
                    />
                    <button css={buttonCss} onClick={() => ws?.connectWithNewUrl(wsIp)}>
                        Connect
                    </button>
                    <button css={buttonCss} onClick={ws?.close}>
                        Disconnect
                    </button>

                </div>

                <div className="col">
                    <input
                        css={inputCss}
                        type="text"
                        value={message}
                        onChange={(e) => setMessage(e.target.value)}
                        placeholder="Enter message"
                    />
                    <button css={buttonCss} onClick={sendMessage}>
                        Send
                    </button>
                    <button css={buttonCss} onClick={() => setMessage("")}>
                        Clear
                    </button>
                </div>
            </div>

        </div>
    );
};


const containerCss = css({
    padding: "2rem",
    fontFamily: "sans-serif",
    backgroundColor: "#1a1a1a",
    color: "#eee",
    minHeight: "100vh",

    "& .col": {
        display: "flex",
        gap: "8px",
    },

    "& .row": {
        display: "flex",
        flexDirection: "column",
        gap: "8px",
    }
});

const titleCss = css({
    fontSize: "2rem",
    marginBottom: "1.5rem",
});

const inputCss = css({
    borderRadius: '6px',
    border: '1px solid #ccc',
    width: '300px',
});

const buttonCss = css({
    backgroundColor: "#f44336",
    border: "none",
    borderRadius: "6px",
    color: "#fff",
    cursor: "pointer",
});
