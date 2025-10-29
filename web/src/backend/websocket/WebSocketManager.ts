import type { HostRequest } from "../dto/HostRequest";
import type { WsConnectionEnum } from "../enums/WsConnectionEnum";
import { localStorageNames } from "../utils/LocalStorageUtils";


type SubscriberCallback = (data: string) => void;
type StatusCallback = (data: string) => void;


class WebSocketManager {
    private socket: WebSocket | null = null;
    private subscribers: SubscriberCallback[] = [];
    private url: string;
    private shouldReconnect: boolean = true;
    private statusCallback: StatusCallback | null = null;
    private errorCallback: StatusCallback | null = null;

    constructor(url: string) {
        this.url = url;
        this.connect();
    }

    private connect(): void {
        this.socket = new WebSocket(this.url);

        this.socket.onopen = () => {
            const message = 'Connected to WebSocket';
            this.statusCallback?.(message);
            console.log(message);
        };

        this.socket.onmessage = (event: MessageEvent) => {
            const messageData = event.data;
            this.subscribers.forEach((callback) => callback(messageData));
        };

        this.socket.onerror = (error: Event) => {
            this.errorCallback?.('WebSocket error:' + error);
            console.error('WebSocket error:', error);
        };

        this.socket.onclose = () => {
            const message = 'WebSocket connection closed';
            this.statusCallback?.(message);
            console.log(message);

            if (this.shouldReconnect == true) {
                this.statusCallback?.("reconnecting");
                console.log("reconnecting");
                this.reconnect();
            }
        };
    }

    private reconnect(): void {
        setTimeout(() => {
            this.connect();
        }, 5000);
    }

    public setMessagesCallback(callback: typeof this.subscribers[number]): () => void {
        this.subscribers.push(callback);

        return () => {
            this.subscribers = this.subscribers.filter((subscriber) => subscriber !== callback);
        };
    }

    public setStatusCallback(callback: StatusCallback) {
        this.statusCallback = callback;
    }

    public setErrorCallback(callback: StatusCallback) {
        this.errorCallback = callback;
    }

    public sendTextMessage(data: string): void {
        if (this.socket && this.socket.readyState === WebSocket.OPEN) {
            this.socket.send(data);
        }
    }

    public sendToHost(data: HostRequest) {
        this.sendTextMessage(JSON.stringify(data));
    }

    public onMessage(): void {
        console.log("TODO make on message method");
    }

    public close(): void {
        if (this.socket) {
            this.shouldReconnect = false;
            this.socket.close();
            // this.socket = null;
        }
    }

    public deleteSocket(): void {
        if (this.socket) {
            this.close();
            this.socket = null;
        }
    }

    public connectWithNewUrl(url: string): void {
        localStorage.setItem(localStorageNames.hotspotUrl, url);
        this.close();
        this.url = url;
        this.connect();
    }

    public getSocketConnectionStatus(): WsConnectionEnum | undefined {
        if (this.socket == null) return undefined;
        return this.socket.readyState as WsConnectionEnum;
    }
}

export default WebSocketManager;

