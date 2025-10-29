import { useEffect, type DependencyList } from "react";

export type AnyArray = any[];

export type PromiseFunctionOrValue<T, ARGS extends AnyArray = AnyArray> = FunctionOrValue<PromiseOrValue<T>, ARGS>;
export type FunctionOrValue<T, ARGS extends AnyArray = AnyArray> = T | BasicFunction<ARGS, T>;
export type BasicFunction<ARGS extends AnyArray = AnyArray, TReturn = any> = (...args: ARGS) => TReturn;
export type PromiseOrValue<T> = Promise<T> | T;
export async function promisePromiseOrValue<T>(vop: PromiseOrValue<T>): Promise<T> {
    if (vop instanceof Promise) {
        return await vop;
    } else {
        return Promise.resolve(vop);
    }
}
export function runFunctionOrValue<T, ARGS extends AnyArray = AnyArray>(func: FunctionOrValue<T, ARGS>, ...args: ARGS): T {
    if (func instanceof Function) {
        return func(...args);
    }
    return func;
}
export async function promisePromiseFunctionOrValue<T, ARGS extends AnyArray = AnyArray>(vop: PromiseFunctionOrValue<T>, ...args: ARGS): Promise<T> {
    return await promisePromiseOrValue(runFunctionOrValue(vop, ...args));
}

export function useAsyncEffect(effect: PromiseFunctionOrValue<void>, deps: DependencyList): void {
    useEffect(() => {
        promisePromiseFunctionOrValue(effect);
        // eslint-disable-next-line react-hooks/exhaustive-deps
    }, deps);
}