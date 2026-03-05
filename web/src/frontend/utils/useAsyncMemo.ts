import { useEffect, useState } from "react";

export function useAsyncMemo<T>(
    factory: () => Promise<T>,
    deps: React.DependencyList
): T | undefined {
    const [value, setValue] = useState<T | undefined>(undefined);

    useEffect(() => {
        let active = true;

        factory().then((result) => {
            if (active) setValue(result);
        });

        return () => {
            active = false;
        };
    }, deps);

    return value;
}