#include "randomwalk.h"
#include <random>
#include <limits>
#include <algorithm>

double randomPathSampling(
    const Graph& g,
    int start,
    int dest,
    int M,
    int N,
    std::vector<int>& outBestPath
) {
    std::mt19937 rng(std::random_device{}());

    double bestCost = std::numeric_limits<double>::infinity();
    std::vector<int> bestPath;

    for (int iter = 0; iter < M; ++iter) {
        int cur = start;
        double cost = 0.0;
        std::vector<int> path;
        path.push_back(cur);

        for (int step = 0; step < N; ++step) {
            if (cur == dest) break;

            const auto& adj = g.nodes[cur].adj;
            if (adj.empty()) break;

            std::uniform_int_distribution<int> distIdx(0, static_cast<int>(adj.size()) - 1);
            int idx = distIdx(rng);
            const auto& e = adj[idx];

            cost += e.length;
            cur = e.to;
            path.push_back(cur);
        }

        if (cur == dest && cost < bestCost) {
            bestCost = cost;
            bestPath = path;
        }
    }

    outBestPath = bestPath;
    return bestCost;
}
