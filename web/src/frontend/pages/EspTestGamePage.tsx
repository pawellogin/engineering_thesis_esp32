/** @jsxImportSource @emotion/react */
import { css } from '@emotion/react';
import { useForm } from 'antd/es/form/Form';
import { useEffect } from 'react';
import type { MessageDTO } from '../../backend/websocket/types/WsTypes';
import { useWebSocket } from '../../backend/websocket/useWebSocket';
import { pageWrapperCss } from '../utils/CSSC';

export default function EspTestGamePage() {
    const [form] = useForm<{ data: MessageDTO<string> | undefined; }>();
    const ws = useWebSocket();

    useEffect(() => {
        form.setFieldsValue({});
    }, [form]);

    return (
        <div css={pageWrapperCss}>
            <div
                css={wrapper}
            >
                TODO
            </div>
        </div>
    );
}


const wrapper = css({

});

