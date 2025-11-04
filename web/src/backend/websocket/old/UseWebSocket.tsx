import { useContext } from "react";
import { WebSocketContext, type WebSocketContextType } from "./WebSocketContext";

/**
 * @deprecated 
 */
export const useWebSocket = (): WebSocketContextType => {
    const context = useContext<WebSocketContextType>(WebSocketContext);
    if (context === null) {
        throw new Error("useWebSocket must be used within a WebSocketProvider");
    }
    return context;
};
