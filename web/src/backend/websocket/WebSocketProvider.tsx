
import React, { useRef, useState } from 'react';
import { localStorageNames } from '../utils/LocalStorageUtils';
import { WebSocketContext } from './WebSocketContext';
import WebSocketManager from './WebSocketManager';



export const WebSocketProvider: React.FC<{ defaultUrl: string; children: React.ReactNode; }> = ({ defaultUrl: url, children }) => {
  // TODO [3]PEL this could be optimized 
  const [webSocket, setWebSocket] = useState<WebSocketManager | undefined>(undefined);
  const [webSocketUrl,] = useState<string>(() => localStorage.getItem(localStorageNames.hotspotUrl) || url);
  const webSocketRef = useRef<WebSocketManager | undefined>(undefined);

  if (webSocket == null && webSocketRef.current == null) {
    console.log("NEW WEBSOCKET CALL");
    const webSocketInstance = new WebSocketManager(webSocketUrl);
    setWebSocket(webSocketInstance);
    webSocketRef.current = webSocketInstance;
  }

  return (
    <WebSocketContext.Provider value={webSocket}>
      {children}
    </WebSocketContext.Provider>
  );
};

