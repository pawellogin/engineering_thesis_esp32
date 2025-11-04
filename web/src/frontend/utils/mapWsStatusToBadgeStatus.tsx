import type { WSStatus } from "../../backend/websocket/types/WsTypes";

export type BadgeStatusType = "success" | "processing" | "default" | "error" | "warning";


export function mapWsStatusToBadgeStatus(wsStatus: WSStatus): BadgeStatusType {
    switch (wsStatus) {
        case "idle":
            return "default";
        case "connecting":
            return "processing";
        case "open":
            return "success";
        case "closing":
            return "warning";
        case "closed":
            return "default";
        case "error":
            return "error";
        default:
            return "default";
    }
}
