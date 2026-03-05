import { Table } from "antd";
import { useEffect, useState } from "react";
import { RevolverScore, RevolverService } from "../../backend/services/revolverService";

type TableRow = RevolverScore & {
    winner: string;
};

export default function RevolverLeaderboardPage() {

    const [data, setData] = useState<TableRow[]>([]);
    const [loading, setLoading] = useState(false);

    useEffect(() => {
        load();
    }, []);

    async function load() {
        setLoading(true);

        const matches = await RevolverService.getLeaderboard();

        const rows: TableRow[] = matches.map(m => {
            let winner = "DRAW";

            if (m.score1 !== m.score2) {
                winner = m.score1 < m.score2 ? m.username1 : m.username2;
            }

            return {
                ...m,
                winner
            };
        });

        setData(rows);
        setLoading(false);
    }

    const columns = [
        {
            title: "Date",
            dataIndex: "createdAt",
            render: (date: Date) =>
                date ? new Date(date).toLocaleString() : "-"
        },
        {
            title: "Player 1",
            dataIndex: "username1"
        },
        {
            title: "Score",
            dataIndex: "score1"
        },
        {
            title: "Player 2",
            dataIndex: "username2"
        },
        {
            title: "Score",
            dataIndex: "score2"
        },
        {
            title: "Winner",
            dataIndex: "winner"
        }
    ];

    return (
        <Table
            rowKey="id"
            columns={columns}
            dataSource={data}
            loading={loading}
            pagination={{ pageSize: 10 }}
        />
    );
}