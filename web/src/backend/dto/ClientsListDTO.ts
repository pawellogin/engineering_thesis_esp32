
import type { ClientInfo } from "./ClientInfo.ts";

export type ClientsListDTO = {
    //uint8_t count;
    count: number,
    // ClientInfo * items;
    items: ClientInfo[],
};