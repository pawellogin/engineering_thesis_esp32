/** @jsxImportSource @emotion/react */
import { css } from '@emotion/react';
import { Button, Col, Flex, Form, Input, Row } from 'antd';
import { useForm } from 'antd/es/form/Form';
import { useCallback, useContext, useEffect } from 'react';
import type { MessageAction, MessageType } from '../../backend/websocket/types/WsTypes';
import { useWebSocket } from '../../backend/websocket/useWebSocket';
import { WebSocketContext } from '../../backend/websocket/WebSocketContext';
import { pageWrapperCss } from '../utils/CSSC';

type DebugPageFormType = {
    type: MessageType;
    action: MessageAction;
    data: string;
    url: string;
};

export default function ESP32DebugPage() {
    const [form] = useForm<DebugPageFormType>();
    const ws = useWebSocket();
    const { setConfig, manager } = useContext(WebSocketContext);

    useEffect(() => {
        form.setFieldsValue({
            type: 'status',
            action: 'ping',
            data: 'test',
            url: 'ws://192.168.137.102:8001',
        });
    }, [form]);

    const sendMessage = useCallback(() => {
        if (ws) {
            const formData = form.getFieldsValue(true);
            ws.send({
                type: formData.type as any,
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
                    <Button onClick={connect}>Connect</Button>
                    <Button onClick={sendMessage} type="primary">
                        Send
                    </Button>
                </div>
            </Form>
        </div>
    );
}


const formCss = css({
    width: '100%',
    maxWidth: 600,
});

const buttonRowCss = css({
    display: 'flex',
    gap: '1rem',
    marginTop: '1rem',
});
