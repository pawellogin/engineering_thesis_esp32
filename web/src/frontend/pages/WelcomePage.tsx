/** @jsxImportSource @emotion/react */
import { css } from '@emotion/react';
import { useNavigate } from 'react-router-dom';
import { ESP32DebugPageRoute } from '../../backend/routes';

export default function WelcomePage() {
    const navigate = useNavigate();

    const handleStart = () => {
        console.log("Start button clicked");
    };

    return (
        <div css={containerCss}>
            <h1 css={titleCss}>ESP32 Reflex Game</h1>
            <p css={subtitleCss}>Test your reflexes with multiple ESP32 clients</p>
            <div className='buttons'>
                <button css={buttonCss} onClick={handleStart}>
                    Start Game
                </button>
                <button css={buttonCss} onClick={() => navigate(ESP32DebugPageRoute)}>
                    ESP32 debug
                </button>
            </div>
        </div>
    );
};

const containerCss = css({
    "& .buttons": {
        display: "flex",
        flexDirection: "column",
        gap: "4px",
    },

    display: "flex",
    flexDirection: "column",
    alignItems: "center",
    justifyContent: "center",
    height: "100vh",
    backgroundColor: "#121212",
    color: "#ffffff",
    fontFamily: "sans-serif",
});

const titleCss = css({
    fontSize: "2.5rem",
    marginBottom: "1rem",
});

const subtitleCss = css({
    fontSize: "1.2rem",
    marginBottom: "2rem",
});

const buttonCss = css({
    padding: "0.75rem 1.5rem",
    backgroundColor: "#1e88e5",
    border: "none",
    borderRadius: "6px",
    color: "#fff",
    fontSize: "1rem",
    cursor: "pointer",
    ":hover": {
        backgroundColor: "#1565c0",
    },
});


