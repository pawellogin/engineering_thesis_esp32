import type { MessageAction, MessageDTO, MessageType, WSStatus } from "./types/WsTypes";


type Handler = (msg: MessageDTO) => void;

export class WebSocketManager {
    private url: string;
    private socket: WebSocket | null = null;
    private status: WSStatus = 'idle';
    private reconnectAttempts = 0;
    private maxReconnectAttempts = 10;
    private reconnectTimer: ReturnType<typeof setTimeout> | null = null;
    private backoffBase = 500; // ms
    private messageQueue: MessageDTO[] = [];
    private handlers: Map<MessageType, Set<Handler>> = new Map();
    private genericHandlers: Set<Handler> = new Set();
    private heartbeatTimer: ReturnType<typeof setInterval> | null = null;
    private heartbeatInterval = 10000;

    constructor(url: string) {
        this.url = url;
    }


    getStatus() { return this.status; }
    getUrl() { return this.url; }


    setUrl(url: string) {
        if (this.url === url) return;
        this.url = url;
        this.reconnectAttempts = 0;
        this.disconnect();
        this.connect();
    }


    connect() {
        if (!this.url) return;
        if (this.socket && (this.status === 'connecting' || this.status === 'open')) return;


        this.status = 'connecting';
        this.socket = new WebSocket(this.url);
        this.socket.addEventListener('open', this.onOpen);
        this.socket.addEventListener('message', this.onMessage);
        this.socket.addEventListener('close', this.onClose);
        this.socket.addEventListener('error', this.onError);
    }


    disconnect() {
        if (!this.socket) return;
        this.status = 'closing';
        try { this.socket.close(); } catch { }
        this.cleanupSocket();
        this.status = 'closed';
    }


    send(msg: MessageDTO) {
        const full = { ...msg, timestamp: msg.timestamp ?? Date.now() };
        if (this.socket && this.status === 'open') {
            this.socket.send(JSON.stringify(full));
        } else {
            this.messageQueue.push(full);
        }
    }


    sendCommand(action: MessageAction, data: any = {}) {
        this.send({ type: 'command', action, data, timestamp: Date.now() });
    }

    sendStatus(action: MessageAction, data: any = {}) {
        this.send({ type: "status", action, data, timestamp: Date.now() });
    }


    subscribe(type: MessageType | null, handler: Handler) {
        if (!type) {
            this.genericHandlers.add(handler);
            return () => this.genericHandlers.delete(handler);
        }
        if (!this.handlers.has(type)) this.handlers.set(type, new Set());
        this.handlers.get(type)!.add(handler);
        return () => this.handlers.get(type)!.delete(handler);
    }


    private emit(msg: MessageDTO) {
        const set = this.handlers.get(msg.type);
        if (set) set.forEach(h => h(msg));
        this.genericHandlers.forEach(h => h(msg));
    }


    private onOpen = () => {
        this.status = 'open';
        this.reconnectAttempts = 0;
        while (this.messageQueue.length) this.send(this.messageQueue.shift()!);
        // this.startHeartbeat();
    };


    private onMessage = (ev: MessageEvent) => {
        try {
            const parsed = JSON.parse(ev.data) as MessageDTO;
            this.emit(parsed);
        } catch (e) {
            console.warn('Invalid WS message', e, ev.data);
        }
    };


    private onClose = () => {
        this.cleanupSocket();
        this.status = 'closed';
        this.scheduleReconnect();
    };


    private onError = () => {
        this.status = 'error';
    };

    private scheduleReconnect() {
        if (this.reconnectAttempts >= this.maxReconnectAttempts) return;
        this.reconnectAttempts++;
        const delay = Math.min(30000, this.backoffBase * Math.pow(1.8, this.reconnectAttempts));
        this.reconnectTimer = setTimeout(() => this.connect(), delay);
    }


    private startHeartbeat() {
        this.stopHeartbeat();
        this.heartbeatTimer = setInterval(() => {
            if (this.socket && this.status === 'open') {
                this.send({ type: 'status', action: 'ping', data: {}, timestamp: Date.now() });
            }
        }, this.heartbeatInterval);
    }


    private stopHeartbeat() {
        if (this.heartbeatTimer) clearInterval(this.heartbeatTimer);
        this.heartbeatTimer = null;
    }


    private cleanupSocket() {
        if (!this.socket) return;
        this.socket.removeEventListener('open', this.onOpen);
        this.socket.removeEventListener('message', this.onMessage);
        this.socket.removeEventListener('close', this.onClose);
        this.socket.removeEventListener('error', this.onError);
        try { this.socket.close(); } catch { }
        this.socket = null;
        if (this.reconnectTimer) clearTimeout(this.reconnectTimer);
        this.reconnectTimer = null;
        this.stopHeartbeat();
    }

}
