/* eslint-disable @typescript-eslint/no-namespace */
import { addDoc, collection, getDocs, orderBy, query } from "firebase/firestore";
import { db } from "../../lib/firebase";

export type RevolverScore = {
    id?: string;
    username1: string;
    username2: string;
    score1: number;
    score2: number;
    createdAt: Date;
};

const collectionRef = collection(db, "revolverScores");

export namespace RevolverService {

    export async function saveScore(username1: string, username2: string, score1: number, score2: number) {
        await addDoc(collectionRef, {
            username1: username1,
            username2: username2,
            score1: score1,
            score2: score2,
            createdAt: new Date()
        });
    }

    export async function getLeaderboard(): Promise<RevolverScore[]> {
        const q = query(collectionRef, orderBy("createdAt", "desc"));
        const snapshot = await getDocs(q);

        return snapshot.docs.map(doc => {
            const data = doc.data();

            return {
                id: doc.id,
                username1: data.username1,
                username2: data.username2,
                score1: data.score1,
                score2: data.score2,
                createdAt: data.createdAt?.toDate()
            };
        });
    }
}
