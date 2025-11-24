/** @jsxImportSource @emotion/react */
import { css } from '@emotion/react';
import { useForm } from 'antd/es/form/Form';
import { useEffect, useState } from 'react';
import type { MessageDTO } from '../../backend/websocket/types/WsTypes';
import { useWebSocket } from '../../backend/websocket/useWebSocket';
import { pageWrapperCss } from '../utils/CSSC';

export default function MemoryGamePage() {
    const [form] = useForm<{ data: MessageDTO<string> | undefined; }>();
    const ws = useWebSocket();
    const [circles, setCircles] = useState(Array(9).fill({ color: '#ccc' }));

    useEffect(() => {
        form.setFieldsValue({});
    }, [form]);

    return (
        <div css={pageWrapperCss}>
            <div
                css={wrapper}
            >
                {circles.map((c, i) => (
                    <div
                        key={i}
                        css={css(circleCss, { backgroundColor: c.color })}
                    />
                ))}
            </div>
        </div>
    );
}


const wrapper = css({
    display: "grid",
    gridTemplateColumns: "repeat(3, 1fr)",
    gap: "24px",
    width: '320px',
});

const circleCss = css({
    width: "80px",
    height: "80px",
    borderRadius: "50%",
    transition: "background 150ms ease",
});
