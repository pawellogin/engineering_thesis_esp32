/** @jsxImportSource @emotion/react */
import { css } from "@emotion/react";
import { Button, Col, Form, Input, Row } from "antd";
import { useCallback, useEffect, useMemo, useState } from "react";
import { useNavigate } from "react-router-dom";
import { RevolverLeaderboardPageRoute } from "../../backend/routes";
import { RevolverService } from "../../backend/services/revolverService";
import { useWebSocket } from "../../backend/websocket/useWebSocket";
import { msg } from "../utils/BasicMessage";
import { h100w100, pageWrapperCss } from "../utils/CSSC";

type GameClientResult = {
    valid: boolean;
    board_id: number;
    score: number;
    responded: boolean;
};

export default function RevolverGamePage() {
    const ws = useWebSocket();
    const [results, setResults] = useState<GameClientResult[] | null>(null);

    const [username1, setUsername1] = useState<string>("");
    const [username2, setUsername2] = useState<string>("");
    const [isSaved, setIsSaved] = useState<boolean>(false);

    const navigate = useNavigate();

    useEffect(() => {
        if (!ws.lastMessage) return;
        const msg = ws.lastMessage.data as GameClientResult[];
        setResults(msg);

    }, [ws.lastMessage]);

    const startGame = useCallback(() => {
        setResults(null);
        setIsSaved(false);
        ws.sendCommand("start_revolver_game");
    }, [ws]);


    const normalizedScores = useMemo(() => {
        if (!results) return null;

        const [p1, p2] = results;

        return {
            score1: p1.valid && p1.responded ? p1.score : 0,
            score2: p2.valid && p2.responded ? p2.score : 0
        };
    }, [results]);

    const isGameFinished = useMemo(() => {
        if (!results) return false;

        return results.every(r => r.responded);
    }, [results]);

    const canSave = useMemo(() => {

        return isGameFinished &&
            !isSaved &&
            username1.trim() !== "" &&
            username2.trim() !== "";
    }, [isGameFinished, isSaved, username1, username2]);


    const saveMatch = useCallback(async () => {
        if (!normalizedScores || !canSave) {
            msg.error("Save failed");
            return;
        };

        await RevolverService.saveScore(
            username1.trim(),
            username2.trim(),
            normalizedScores.score1,
            normalizedScores.score2,
        );
        msg.success("Saved");

        setIsSaved(true);

        setResults(null);
        setUsername1("");
        setUsername2("");
    }, [normalizedScores, canSave, username1, username2, msg]);



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

                        <Button onClick={() => { msg.info("asd"); }}>
                            END GAME
                        </Button>
                    </div>
                </Col>

                <Row gutter={[8, 8]}>

                    <Col span={12}>
                        <Form.Item label="Player 1 Name">
                            <Input
                                value={username1}
                                onChange={e => setUsername1(e.target.value)}
                            />
                        </Form.Item>
                    </Col>

                    <Col span={12}>
                        <Form.Item label="Player 2 Name">
                            <Input
                                value={username2}
                                onChange={e => setUsername2(e.target.value)}
                            />
                        </Form.Item>
                    </Col>

                    <Col span={24} css={css({ display: "flex", justifyContent: "center" })}>
                        <Button
                            color="default" variant="solid"
                            onClick={saveMatch}
                            disabled={!canSave}
                        >
                            SAVE SCORES
                        </Button>
                    </Col>
                    <Col span={24} css={css({ display: "flex", justifyContent: "center" })}>
                        <Button
                            color="default" variant="solid"
                            onClick={() => { navigate("/" + RevolverLeaderboardPageRoute); }}
                        >
                            LEADERBOARD
                        </Button>
                    </Col>
                </Row>

            </Row>
        </div>
    );
}


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
