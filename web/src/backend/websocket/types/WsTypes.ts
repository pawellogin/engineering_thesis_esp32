export type MessageType = 'command' | 'status' | 'error';
export type MessageAction = 'restart' | 'blink_led' | 'blink_gateway_led' | string;

// eslint-disable-next-line @typescript-eslint/no-explicit-any
export interface MessageDTO<T = any> {
    type: MessageType;
    action: MessageAction;
    data: T;
    timestamp: number;
}


export type WSStatus = 'idle' | 'connecting' | 'open' | 'closing' | 'closed' | 'error';
export interface WSConfig { url: string; }