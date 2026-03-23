#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

// ==============================
// 資料結構定義
// ==============================
struct Edge {
    int u, v, w;
};

// 並查集 Disjoint Set Union
class DSU {
private:
    vector<int> parent, rankv;

public:
    DSU(int n) {
        parent.resize(n + 1);
        rankv.resize(n + 1, 0);
        for (int i = 1; i <= n; i++) {
            parent[i] = i;
        }
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
        return x; // 請修改
    }

    bool unite(int a, int b) {
        int rootA = find(a);
        int rootB = find(b);

        if (rootA == rootB) return false;

        // union by rank
        if (rankv[rootA] < rankv[rootB]) {
            parent[rootA] = rootB;
        } else if (rankv[rootA] > rankv[rootB]) {
            parent[rootB] = rootA;
        } else {
            parent[rootB] = rootA;
            rankv[rootA]++;
        }

        return true;
    }

};

// ==============================
// 工具函式
// ==============================
void printEdge(const Edge& e) {
    cout << e.u << " - " << e.v << " : " << e.w;
}

void printMST(const vector<Edge>& mst) {
    int total = 0;
    cout << "\nMST edges:\n";
    for (const auto& e : mst) {
        printEdge(e);
        cout << "\n";
        total += e.w;
    }
    cout << "Total weight = " << total << "\n";
}

// ==============================
// 1. Kruskal Algorithm Template
// ==============================
void kruskalMST(int n, vector<Edge> edges) {
    cout << "==============================\n";
    cout << "Kruskal Algorithm\n";
    cout << "==============================\n";

    vector<Edge> mst;

    // TODO:
    // Step 1. 將 edges 依照權重由小到大排序
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.w < b.w;
    });

    // TODO:
    // Step 2. 建立 DSU 物件
    DSU dsu(n);
    cout << "Selection steps:\n";

    // TODO:
    // Step 3. 逐一檢查排序後的每條邊
    for (const auto& e : edges) {
        cout << "Check ";
        printEdge(e);

        if (dsu.unite(e.u, e.v)) {
            cout << " -> Selected\n";
            mst.push_back(e);
            if ((int)mst.size() == n - 1) break;
        } else {
            cout << " -> Skipped (cycle)\n";
        }
    }

    printMST(mst);
    cout << "\n";
}

// ==============================
// 2. Prim Algorithm Template
//    從 start 開始
// ==============================
void primMST(int n, const vector<vector<pair<int, int>>>& adj, int start = 1) {
    cout << "==============================\n";
    cout << "Prim Algorithm\n";
    cout << "==============================\n";

    vector<bool> inMST(n + 1, false);
    vector<Edge> mst;

    // 最小堆: {權重, from, to}
    priority_queue<
        tuple<int, int, int>,
        vector<tuple<int, int, int>>,
        greater<tuple<int, int, int>>
    > pq;

    // TODO:
    // Step 1. 將起點 start 設為已加入 MST
    inMST[start] = true;
    // TODO:
    // Step 2. 把 start 相鄰的邊放入 priority queue

    for (const auto& [to, w] : adj[start]) {
        pq.push({w, start, to});
    }
    cout << "Selection steps:\n";

    // TODO:
    // Step 3. 當 pq 不為空且 mst 邊數 < n-1
    while (!pq.empty() && (int)mst.size() < n - 1) {
        auto [w, from, to] = pq.top();
        pq.pop();

        cout << "Check " << from << " - " << to << " : " << w;

        if (inMST[to]) {
            cout << " -> Skipped\n";
            continue;
        }

        cout << " -> Selected\n";
        inMST[to] = true;
        mst.push_back({from, to, w});

        for (const auto& [next, nextW] : adj[to]) {
            if (!inMST[next]) {
                pq.push({nextW, to, next});
            }
        }
    }

    printMST(mst);
    cout << "\n";
}

// ==============================
// 3. Sollin / Boruvka Template
// ==============================
void boruvkaMST(int n, const vector<Edge>& edges) {
    cout << "==============================\n";
    cout << "Sollin / Boruvka Algorithm\n";
    cout << "==============================\n";

    vector<Edge> mst;

    // TODO:
    DSU dsu(n);
    // Step 2. 初始 component 數量為 n

    int numComponents = n;
    int round = 1;

    while (numComponents > 1) {
        cout << "Round " << round << ":\n";

        // cheapest[i] = 第 i 個 component 目前找到的最便宜邊的 index
        vector<int> cheapest(n + 1, -1);

        // TODO:
        for (int i = 0; i < (int)edges.size(); i++) {
            int set1 = dsu.find(edges[i].u);
            int set2 = dsu.find(edges[i].v);

            if (set1 == set2) continue;

            if (cheapest[set1] == -1 || edges[i].w < edges[cheapest[set1]].w) {
                cheapest[set1] = i;
            }
            if (cheapest[set2] == -1 || edges[i].w < edges[cheapest[set2]].w) {
                cheapest[set2] = i;
            }
        }

        bool merged = false;

        // TODO:
        for (int i = 1; i <= n; i++) {
            if (cheapest[i] == -1) continue;

            Edge e = edges[cheapest[i]];
            int set1 = dsu.find(e.u);
            int set2 = dsu.find(e.v);

            if (set1 == set2) continue;

            if (dsu.unite(set1, set2)) {
                cout << "Selected ";
                printEdge(e);
                cout << "\n";

                mst.push_back(e);
                numComponents--;
                merged = true;
            }
        }

        if (!merged) break;

        cout << "\n";
        round++;
    }


    printMST(mst);
    cout << "\n";
}

// ==============================
// 主程式
// ==============================
int main() {
    int n = 6;

    // 圖中的無向邊
    vector<Edge> edges = {
        {1, 2, 16},
        {1, 5, 19},
        {5, 4, 18},
        {2, 4, 6},
        {2, 3, 5},
        {4, 3, 10},
        {1, 6, 22},
        {5, 6, 33},
        {6, 2, 12},
        {6, 4, 14}
    };

    // adjacency list，供 Prim 使用
    vector<vector<pair<int, int>>> adj(n + 1);
    for (const auto& e : edges) {
        adj[e.u].push_back({e.v, e.w});
        adj[e.v].push_back({e.u, e.w});
    }

    kruskalMST(n, edges);
    primMST(n, adj, 1);
    boruvkaMST(n, edges);

    return 0;
}
