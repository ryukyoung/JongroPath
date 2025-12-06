#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <cstddef>

struct Edge {
    int to;
    double length;
};

struct Node {
    long long id;
    double lat;
    double lon;
    std::vector<Edge> adj;
};

struct LatLonKey {
    double lat;
    double lon;
};

struct LatLonKeyHash {
    size_t operator()(LatLonKey const& k) const noexcept;
};

struct LatLonKeyEq {
    bool operator()(LatLonKey const& a, LatLonKey const& b) const noexcept;
};

class Graph {
public:
    std::vector<Node> nodes;
    std::unordered_map<long long, int> id_to_index;
    std::unordered_map<LatLonKey, int, LatLonKeyHash, LatLonKeyEq> coord_to_index;

    bool loadFromGraphML(const std::string& filename);
    int findNodeByCoord(double lat, double lon) const;
};