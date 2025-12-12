/** @jsxImportSource @emotion/react */
import { css } from '@emotion/react';
import { Button, Col, Form, Row } from 'antd';
import { useForm } from 'antd/es/form/Form';
import { useCallback, useEffect } from 'react';
import { useWebSocket } from '../../backend/websocket/useWebSocket';
import { h100w100, pageWrapperCss } from '../utils/CSSC';
import { formCss } from './ESP32DebugPage';

type EspTestGamePageForm = unknown;

export default function EspTestGamePage() {
    const [form] = useForm<EspTestGamePageForm>();
    const ws = useWebSocket();

    useEffect(() => {
        form.setFieldsValue({});
    }, [form]);

    const startTestGame = useCallback(() => {
        ws.sendCommand("start_esp_test_game");
    }, [ws]);

    const endTestGame = useCallback(() => {
        ws.sendCommand("end_esp_test_game");
    }, [ws]);

    return (
        <div css={pageWrapperCss}>
            <Form form={form} layout="vertical" css={formCss}>
                <Row css={h100w100}>
                    <Col xs={24} sm={24} css={css({ display: "flex", flexDirection: "column", alignItems: "center" })}>
                        <Button onClick={startTestGame} >START TEST GAME</Button>
                        <Button onClick={endTestGame} >END TEST GAME</Button>
                        {JSON.stringify(ws.lastMessage)}
                    </Col>
                </Row>
            </Form>
        </div>
    );
}
