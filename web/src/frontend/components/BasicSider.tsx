/** @jsxImportSource @emotion/react */

import {
    HomeOutlined,
    SettingOutlined
} from "@ant-design/icons";
import { css } from "@emotion/react";
import { Layout, Menu, theme } from "antd";
import { useMemo, useState, type ReactNode } from "react";
import { useNavigate } from "react-router-dom";
import { ESP32DebugPageRoute } from "../../backend/routes";
import WebsocketStatusBox from "./WebsocketStatusBox";

const { Content, Sider } = Layout;

type BasicSiderProps = { children: ReactNode; };

export default function BasicSider(props: BasicSiderProps) {
    const [collapsed, setCollapsed] = useState(false);
    const { token } = theme.useToken();
    const navigate = useNavigate();

    const basicSiderCss = useMemo(() => css({
        minHeight: "100vh",
        "& .sider, & .menu": {
            background: token.colorBgContainer,
            color: token.colorText
        }
    }), [token.colorBgContainer, token.colorText]);

    const menuItems = useMemo(() => [
        {
            key: "1",
            icon: <HomeOutlined />,
            label: "Main page",
            onClick: () => navigate("/")
        },
        {
            key: "2",
            icon: <SettingOutlined />,
            label: "Web socket config",
            onClick: () => navigate(ESP32DebugPageRoute)
        }
    ], [navigate]);

    return (
        <Layout css={basicSiderCss} style={{ minHeight: "100vh" }} >
            <Sider
                collapsible
                className="sider"
                collapsed={collapsed}
                onCollapse={(value) => setCollapsed(value)}
                style={{
                    background: token.colorBgContainer, // match theme background
                }}
            >
                <div>
                    <WebsocketStatusBox />
                </div>
                <Menu className="menu" mode="inline" items={menuItems} />
            </Sider>
            <Layout>
                {/* <Header style={{ padding: 0, background: colorBgContainer }} /> */}
                <Content >
                    {props.children}
                </Content>
            </Layout>
        </Layout>
    );
}


