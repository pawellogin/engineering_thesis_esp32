/** @jsxImportSource @emotion/react */
import { Button, Form } from 'antd';
import { useForm } from 'antd/es/form/Form';
import { useCallback, useEffect } from 'react';
import { useWebSocket } from '../../backend/websocket/useWebSocket';
import { pageWrapperCss } from '../utils/CSSC';
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

    return (
        <div css={pageWrapperCss}>
            <Form form={form} layout="vertical" css={formCss}>
                <div css={pageWrapperCss}>
                    <Button onClick={startTestGame} >START TEST GAME</Button>
                    {JSON.stringify(ws.lastMessage)}
                </div>
            </Form>
        </div>
    );
}
