/** @jsxImportSource @emotion/react */
import { css } from '@emotion/react';
import { Button, Col, Flex, Form, Input, Row } from 'antd';
import { useForm } from 'antd/es/form/Form';
import { useCallback, useContext, useEffect } from 'react';
import type { ClientsListDTO } from '../../backend/dto/ClientsListDTO';
import type { MessageAction, MessageType } from '../../backend/websocket/types/WsTypes';
import { useWebSocket } from '../../backend/websocket/useWebSocket';
import { WebSocketContext } from '../../backend/websocket/WebSocketContext';
import { pageWrapperCss } from '../utils/CSSC';


type DebugPageFormType = {
    type: MessageType;
    action: MessageAction;
    data: string;
    url: string;

    clients: ClientsListDTO;
};

export default function ESP32DebugPage() {
    const [form] = useForm<DebugPageFormType>();
    const ws = useWebSocket();
    const { setConfig, manager } = useContext(WebSocketContext);

    useEffect(() => {
        form.setFieldsValue({
            type: 'status',
            action: undefined,
            data: 'test',
            url: 'ws://esp-gateway.local:8001',
        });
    }, [form]);

    useEffect(() => {

        if (ws.lastMessage?.action === "get_system_info") {
            console.log("system info received", ws.lastMessage);

            form.setFieldValue("clients", ws.lastMessage.data);
            form.validateFields(['clients']); // forces rerender for that field

        }
    }, [form, ws.lastMessage, ws.lastMessage?.action]);

    const sendMessage = useCallback(() => {
        if (ws) {
            const formData = form.getFieldsValue(true);
            ws.send({
                type: formData.type,
                action: formData.action,
                data: formData.data,
                timestamp: 0,
            });
        }
    }, [form, ws]);

    const connect = useCallback(() => {
        const url = form.getFieldValue('url');
        setConfig({ url: url });
        manager.setUrl(url);
        manager.disconnect();
        manager.connect();
    }, [form, manager, setConfig]);

    const blinkClientsLED = useCallback(() => {
        ws.sendCommand("blink_clients_led");
    }, [ws]);

    const restartGatewayAndClients = useCallback(() => {
        ws.sendCommand("restart_all");
    }, [ws]);

    const restartOnlyClients = useCallback(() => {
        ws.sendCommand("restart_clients");
    }, [ws]);

    const restartOnlyGateway = useCallback(() => {
        ws.sendCommand("restart_gateway");
    }, [ws]);

    const getSystemInfo = useCallback(() => {
        ws.sendCommand("get_system_info");
    }, [ws]);

    return (
        <div css={pageWrapperCss}>
            <Form form={form} layout="vertical" css={formCss}>
                <h1 >ESP32 Debug Panel</h1>
                <Flex>
                    {JSON.stringify(ws.getWebsocketSetup())}
                </Flex>

                <Row gutter={[8, 8]}>
                    <Col xs={24} md={12}>
                        <Form.Item<DebugPageFormType> name="data" label="Data">
                            <Input />
                        </Form.Item>
                    </Col>

                    <Col xs={24} md={12}>
                        <Form.Item<DebugPageFormType> name="url" label="WebSocket URL">
                            <Input />
                        </Form.Item>
                    </Col>


                </Row>

                <div css={buttonRowCss}>
                    <Button onClick={() => ws.sendCommand("blink_gateway_led")}>Blink gateway LED</Button>
                    <Button onClick={blinkClientsLED}>Blink clients LED</Button>
                    <Button onClick={restartGatewayAndClients}>Restart gateway and clients </Button>
                    <Button onClick={restartOnlyClients}>Restart only clients </Button>
                    <Button onClick={restartOnlyGateway}>Restart only gateway </Button>
                    <Button onClick={getSystemInfo}>Get system info </Button>
                    <Button onClick={connect}>Connect</Button>
                    <Button onClick={sendMessage} type="primary">
                        Send
                    </Button>
                </div>

                <Col xs={24} md={24}>
                    <Form.Item
                        shouldUpdate={(prev, curr) => prev.clients !== curr.clients}
                        noStyle
                    >
                        {({ getFieldValue }) => {
                            const clients = getFieldValue("clients") as ClientsListDTO | undefined;
                            return (
                                <div>
                                    {clients?.items
                                        ?.sort((x, y) => x.boardId - y.boardId)
                                        .map((client, i) => (
                                            <div key={i} style={{ marginBottom: "0.5rem" }}>
                                                <strong>Client {i + 1}</strong>:<br />
                                                IP: {client.ip} <br />
                                                Board ID: {client.boardId} <br />
                                                Last Seen: {client.lastSeen} ms <br />
                                                Connected: {client.connected ? "Yes" : "No"}
                                            </div>
                                        )) || <span>No clients</span>}
                                </div>
                            );
                        }}
                    </Form.Item>

                </Col>
            </Form>
        </div>
    );
}


export const formCss = css({
    width: '100%',
    height: "100%",
    maxWidth: 1200,
});

const buttonRowCss = css({
    display: 'flex',
    flexDirection: "column",
    overflowY: "auto",
    gap: '1rem',
    marginTop: '1rem',
    height: "300px",

    "& button": {
        maxWidth: '256px',
        minWidth: '256px',
        marginInline: "auto",
    }
});
