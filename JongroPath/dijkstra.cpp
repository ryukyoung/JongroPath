#include "dijkstra.h"
#include <queue>
#include <limits>
#include <algorithm>

struct State {
    double cost;
    int v;
    bool operator<(State const& other) const {
        return cost > other.cost; // ÃÖ¼Ò Èü
    }
};

double dijkstraShortestPath(const Graph& g, int start, int dest, std::vector<int>& outPath)
{
    int n = static_cast<int>(g.nodes.size());
    std::vector<double> dist(n, std::numeric_limits<double>::infinity());
    std::vector<int> parent(n, -1);

    std::priority_queue<State> pq;
    dist[start] = 0.0;
    pq.push({ 0.0, start });

    while (!pq.empty()) {
        auto cur = pq.top();
        pq.pop();

        if (cur.cost > dist[cur.v]) continue;
        if (cur.v == dest) break;

        for (const auto& e : g.nodes[cur.v].adj) {
            int nv = e.to;
            double nc = cur.cost + e.length;
            if (nc < dist[nv]) {
                dist[nv] = nc;
                parent[nv] = cur.v;
                pq.push({ nc, nv });
            }
        }
    }

    if (!std::isfinite(dist[dest])) {
        outPath.clear();
        return std::numeric_limits<double>::infinity();
    }

    outPath.clear();
    for (int cur = dest; cur != -1; cur = parent[cur]) {
        outPath.push_back(cur);
    }
    std::reverse(outPath.begin(), outPath.end());
    return dist[dest];
}
