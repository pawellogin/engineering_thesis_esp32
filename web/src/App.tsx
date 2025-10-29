import { Route, Routes } from "react-router-dom";
import { ESP32DebugPageRoute } from "./backend/routes";
import { WebSocketProvider } from "./backend/websocket/WebSocketProvider";
import ESP32DebugPage from "./frontend/pages/ESP32DebugPage";
import Welcome from "./frontend/pages/WelcomePage";


function App() {

  return (
    <div >
      <WebSocketProvider defaultUrl="wss://echo.websocket.org/">
        <Routes>
          <Route path="/" element={<Welcome />} />
          <Route path={ESP32DebugPageRoute} element={<ESP32DebugPage />} />
        </Routes>
      </WebSocketProvider>
    </div>
  );
}

export default App;
