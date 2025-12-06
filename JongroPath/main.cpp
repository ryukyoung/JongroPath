#include <iostream>
#include <vector>
#include "graph.h"
#include "dijkstra.h"
#include "randomwalk.h"

int main() {
    Graph g;

    // 1) 일단 절대 경로로 테스트
    // jongro.graphml 이 실제로 있는 경로로 바꿔줘야 함!
    std::string filename = "./jongro.graphml";

    std::cout << "Trying to open: " << filename << "\n";
    if (!g.loadFromGraphML(filename)) {
        std::cout << "Graph load failed.\n";
        return 1;
    }

    std::cout << "Sample nodes:\n";
    for (int i = 0; i < 10; ++i) {
        std::cout << i << ": lat=" << g.nodes[i].lat
            << " lon=" << g.nodes[i].lon << "\n";
    }

    std::cout << "Insert start position. (lat lon) ";
    double sLat, sLon;
    std::cin >> sLat >> sLon;

    std::cout << "Insert destination position. (lat lon) ";
    double dLat, dLon;
    std::cin >> dLat >> dLon;

    int s = g.findNodeByCoord(sLat, sLon);
    int d = g.findNodeByCoord(dLat, dLon);

    if (s == -1 || d == -1) {
        std::cout << "Start or destination not found in map." << std::endl;
        return 0;
    }

    // Random Path Sampling
    std::vector<int> rpath;
    double rcost = randomPathSampling(g, s, d, 1000, 1000, rpath);

    if (rpath.empty()) {
        std::cout << "Random Path Sampling could not reach destination." << std::endl;
    }
    else {
        std::cout << "Random Path Sampling shortest path length is : " << rcost << std::endl;
        std::cout << "Random Path Sampling path is : ";
        for (size_t i = 0; i < rpath.size(); ++i) {
            std::cout << g.nodes[rpath[i]].id;
            if (i + 1 < rpath.size()) std::cout << "-";
        }
        std::cout << std::endl;
    }

    // Dijkstra
    std::vector<int> dpath;
    double dcost = dijkstraShortestPath(g, s, d, dpath);

    if (dpath.empty()) {
        std::cout << "No path from start to destination (Dijkstra)." << std::endl;
    }
    else {
        std::cout << "Dijkstra algorithm path length is : " << dcost << std::endl;
        std::cout << "Dijkstra algorithm path is : ";
        for (size_t i = 0; i < dpath.size(); ++i) {
            std::cout << g.nodes[dpath[i]].id;
            if (i + 1 < dpath.size()) std::cout << "-";
        }
        std::cout << std::endl;
    }

    return 0;
}
