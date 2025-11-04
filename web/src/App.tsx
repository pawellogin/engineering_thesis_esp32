import { Route, Routes } from "react-router-dom";
import { ConfigPanelRoute, ESP32DebugPageRoute } from "./backend/routes";
// import { WebSocketProvider } from "./backend/websocket/old/WebSocketProvider";
import { useWebSocket } from "./backend/websocket/UseWebSocket";
import { WebSocketProvider } from "./frontend/components/WebSocketProvider";
import { ConfigPanel } from "./frontend/pages/ConfigPanel";
import ESP32DebugPage from "./frontend/pages/ESP32DebugPage";
import Welcome from "./frontend/pages/WelcomePage";


function App() {

  // TODO move s
  const StatusBox: React.FC = () => {
    const { status, lastMessage, sendCommand } = useWebSocket();
    return (
      <div>
        <div>Status: {status}</div>
        <div>Last message: {lastMessage ? JSON.stringify(lastMessage) : '—'}</div>
        <button onClick={() => sendCommand('blink_led', { times: 3 })}>Blink LED</button>
      </div>
    );
  };

  return (
    <div >
      <WebSocketProvider>
        {/* <BaseHeader /> */}
        <StatusBox />
        <Routes>
          <Route path="/" element={<Welcome />} />
          <Route path={ESP32DebugPageRoute} element={<ESP32DebugPage />} />
          <Route path={ConfigPanelRoute} element={<ConfigPanel />} />
        </Routes>
      </WebSocketProvider>
    </div>
  );
}

export default App;
