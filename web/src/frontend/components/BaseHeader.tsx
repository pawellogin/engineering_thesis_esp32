/** @jsxImportSource @emotion/react */
import { css } from "@emotion/react";
import { Badge, Layout, Typography } from "antd";
import { useCallback } from "react";
import { WsConnectionEnum } from "../../backend/enums/WsConnectionEnum";
import { useWebSocket } from '../../backend/websocket/old/UseWebSocket';
const { Header: AntHeader } = Layout;
const { Title } = Typography;

export default function BaseHeader() {

    const ws = useWebSocket();

    const getBadgeStatus = useCallback(() => {
        const wsStatus = ws?.getSocketConnectionStatus();
        switch (wsStatus) {
            case WsConnectionEnum.OPEN: return "success";
            case WsConnectionEnum.CONNECTING: return "processing";
            default: "error";
        }
    }, []);

    return (
        <AntHeader css={headerCss}>
            <Title level={4} style={{ margin: 0 }}>
                ESP32 Reflex System
            </Title>
            <Badge status={getBadgeStatus()} text={`WebSocket`} />
        </AntHeader>
    );
};

const headerCss = css({
    display: "flex",
    justifyContent: "space-between",
    alignItems: "center",
    backgroundColor: "#fff",
    boxShadow: "0 1px 4px rgba(0,0,0,0.1)",
    padding: "0 24px",
    height: "64px",
    zIndex: 100,
    position: "sticky",
    top: 0,
});