/** @jsxImportSource @emotion/react */
import { css } from "@emotion/react";
import { Button, Col, Form, Row } from "antd";
import { useForm } from "antd/es/form/Form";
import { useCallback, useEffect, useMemo, useState } from "react";
import { useWebSocket } from "../../backend/websocket/useWebSocket";
import { h100w100, pageWrapperCss } from "../utils/CSSC";
import { formCss } from "./ESP32DebugPage";

type GameClientResult = {
    valid: boolean;
    board_id: number;
    score: number;
    responded: boolean;
};

const circlesWrapperCss = css({
    display: "flex",
    justifyContent: "center",
    alignItems: "center",
    gap: "3rem",
    marginBottom: "2rem",
    flexWrap: "wrap"
});

const circleBaseCss = css({
    width: "220px",
    height: "220px",
    borderRadius: "50%",
    display: "flex",
    flexDirection: "column",
    justifyContent: "center",
    alignItems: "center",
    fontSize: "18px",
    fontWeight: 600,
    transition: "all 0.3s ease",
    backgroundColor: "#444",
    color: "white",
    textAlign: "center"
});

const buttonRowCss = css({
    display: "flex",
    flexDirection: "column",
    gap: "1rem",
    marginTop: "1rem",
    alignItems: "center",

    "& button": {
        maxWidth: "256px",
        minWidth: "256px"
    }
});

export default function RevolverGamePage() {
    const [form] = useForm();
    const ws = useWebSocket();

    const [results, setResults] = useState<GameClientResult[] | null>(null);

    useEffect(() => {
        if (!ws.lastMessage) return;
        const msg = ws.lastMessage.data as GameClientResult[];
        setResults(msg);

    }, [ws.lastMessage]);

    const startGame = useCallback(() => {
        setResults(null);
        ws.sendCommand("start_revolver_game");
    }, [ws]);

    // const endGame = useCallback(() => {
    //     // ws.sendCommand("end_revolver_game");
    // }, [ws]);

    const winner = useMemo(() => {
        if (!results) return null;

        const [p1, p2] = results;

        const p1Invalid = !p1.valid || !p1.responded;
        const p2Invalid = !p2.valid || !p2.responded;

        if (p1Invalid && p2Invalid) return "draw";
        if (p1Invalid) return "p2";
        if (p2Invalid) return "p1";

        if (p1.score === p2.score) return "draw";
        return p1.score < p2.score ? "p1" : "p2";
    }, [results]);

    const getCircleColorCss = (player: "p1" | "p2") => {
        if (!winner) return css({});
        if (winner === "draw") return css({ backgroundColor: "#777" });
        if (winner === player) return css({ backgroundColor: "#15803d" });
        return css({ backgroundColor: "#b91c1c" });
    };

    const renderScore = (index: number) => {
        if (!results) return "-";

        const r = results[index];
        if (!r.valid) return "INVALID";
        if (!r.responded) return "NO RESPONSE";

        return `${r.score} ms`;
    };

    return (
        <div css={pageWrapperCss}>
            <Form form={form} layout="vertical" css={formCss}>
                <Row css={h100w100} justify="center" align="middle">
                    <Col xs={24}>
                        <div css={circlesWrapperCss}>
                            <div css={[circleBaseCss, getCircleColorCss("p1")]}>
                                <div>PLAYER 1</div>
                                <div>{renderScore(0)}</div>
                            </div>

                            <div css={[circleBaseCss, getCircleColorCss("p2")]}>
                                <div>PLAYER 2</div>
                                <div>{renderScore(1)}</div>
                            </div>
                        </div>

                        <div css={buttonRowCss}>
                            <Button onClick={startGame}>
                                START REVOLVER GAME
                            </Button>

                            {/* <Button onClick={endGame}>
                                END GAME
                            </Button> */}
                        </div>
                    </Col>
                </Row>
            </Form>
        </div>
    );
}