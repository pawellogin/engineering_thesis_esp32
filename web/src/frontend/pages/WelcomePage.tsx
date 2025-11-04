/** @jsxImportSource @emotion/react */
import { css } from '@emotion/react';

export default function WelcomePage() {
    return (
        <div css={welcomePageCss}>
            <h1 >ESP32 Reflex Game</h1>
            <p >Test your reflexes with multiple ESP32 clients</p>
        </div>
    );
};

const welcomePageCss = css({
    display: "flex",
    flexDirection: "column",
    alignItems: "center",
    justifyContent: "center",
    height: "100vh",
});



