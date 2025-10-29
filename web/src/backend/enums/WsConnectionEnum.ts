export const WsConnectionEnum = {
    CONNECTING: WebSocket.CONNECTING,
    OPEN: WebSocket.OPEN,
    CLOSING: WebSocket.CLOSING,
    CLOSED: WebSocket.CLOSED,
} as const;

export type WsConnectionEnum = (typeof WsConnectionEnum)[keyof typeof WsConnectionEnum];
