/** @jsxImportSource @emotion/react */

import { css, Global } from "@emotion/react";
import { ConfigProvider, theme } from "antd";
import { Route, Routes } from "react-router-dom";
import { ESP32DebugPageRoute } from "./backend/routes";
import BasicSider from "./frontend/components/BasicSider";
import { WebSocketProvider } from "./frontend/components/WebSocketProvider";
import ESP32DebugPage from "./frontend/pages/ESP32DebugPage";
import Welcome from "./frontend/pages/WelcomePage";


function App() {
  return (
    <>
      <ConfigProvider
        theme={{
          algorithm: theme.darkAlgorithm,
        }}
      >
        <WebSocketProvider>
          <Global styles={globalCss} />
          <BasicSider>
            {/* <StatusBox /> */}
            <Routes>
              <Route path="/" element={<Welcome />} />
              <Route path={ESP32DebugPageRoute} element={<ESP32DebugPage />} />
            </Routes>
          </BasicSider>
        </WebSocketProvider>
      </ConfigProvider>

    </>
  );
}

export default App;

const globalCss = css({
  body: {
    margin: 0,
  },
});

