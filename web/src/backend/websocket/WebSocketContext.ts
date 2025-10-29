import WebSocketManager from "./WebSocketManager";
import { createContext } from "react";

export type WebSocketContextType = WebSocketManager | undefined;

export const WebSocketContext = createContext<WebSocketContextType>(undefined);
