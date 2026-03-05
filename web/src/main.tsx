/** @jsxImportSource @emotion/react */

import { App as AntdApp } from "antd";
import { StrictMode } from 'react';
import { createRoot } from 'react-dom/client';
import { BrowserRouter } from 'react-router-dom';
import App from './App';


createRoot(document.getElementById('root')!).render(
  <StrictMode>
    <BrowserRouter>
      <AntdApp>
        <App />
      </AntdApp>
    </BrowserRouter>
  </StrictMode>,
);
