import { createContext } from "react";
import WebSocketManager from "./WebSocketManager";

export type WebSocketContextType = WebSocketManager | undefined;
/**
 * @deprecated 
 */
export const WebSocketContext = createContext<WebSocketContextType>(undefined);
