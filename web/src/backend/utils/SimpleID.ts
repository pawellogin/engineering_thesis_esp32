
export type Maybe<T = null> = T | undefined | null;

export namespace SimpleID {
    export const addID = 0;
    export function is(id?: Maybe<number>): id is Maybe {
        return id == null || id <= 0;
    }
    export function makeNew(listSize: number = 0) {
        const part1 = ((new Date().getTime() % 100000));
        const rand = Math.random();
        const part2 = Math.round((rand * 10_000_000)) % 100000;
        const part3 = id.next().value;//2
        const part4 = listSize % 100;
        const newId = -(part1 * 100 * 100 * 100000 + part2 * 100 * 100 + part3 * 100 + part4);

        return newId;
        // return -(((new Date().getTime() % 10_000_000_000) * 1000) + Math.round((Math.random() * 10)) * 100 + listSize);
    }
    const id = function* () { let i = 0; while (true) { i = (i + 1) % 100; yield i; } }();


    export function toServiceID(id?: Maybe<number>): number {
        return SimpleID.is(id) ? SimpleID.addID : id ?? SimpleID.addID;
    }

    export function toDisplay(id?: number): string {
        return SimpleID.is(id) ? "noweID" : (id?.toString() ?? "ERROR");
    }
    export function toDisplayInCDG(dataField: string) {
        return (row: any) => toDisplay(row?.[dataField]);

    }
}