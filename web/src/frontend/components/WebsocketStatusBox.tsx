/** @jsxImportSource @emotion/react */

import {
    AlertOutlined
} from "@ant-design/icons";
import { css } from "@emotion/react";
import { Badge, Col, Row, Typography } from "antd";

import { useWebSocket } from "../../backend/websocket/useWebSocket";
import { mapWsStatusToBadgeStatus } from "../utils/mapWsStatusToBadgeStatus";

export default function WebsocketStatusBox() {

    const ws = useWebSocket();

    const { status } = ws.getWebsocketSetup(); // for example: "open", "connecting", etc.
    const badgeStatus = mapWsStatusToBadgeStatus(status);

    return (
        <Row gutter={[8, 8]} css={websocketStatusBoxCss}>
            <Col>
                <Badge
                    status={badgeStatus}
                    text={<Typography.Text>{status}</Typography.Text>}
                />
            </Col>
            <Col>
                <AlertOutlined onClick={() => ws.sendCommand("blink_gateway_led")} />
            </Col>
        </Row>
    );
};


const websocketStatusBoxCss = css({
    display: "flex",
    justifyContent: "center",
    padding: "12px",
    height: "100%",
    width: "100%",
});