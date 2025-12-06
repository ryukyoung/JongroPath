#include "graph.h"
#include <cmath>
#include <iostream>
#include <limits>

// tinyxml2 헤더는 별도 설치/추가 필요
#include "tinyxml2.h"

using namespace tinyxml2;

size_t LatLonKeyHash::operator()(LatLonKey const& k) const noexcept {
    long long a = static_cast<long long>(k.lat * 1e7);
    long long b = static_cast<long long>(k.lon * 1e7);
    return std::hash<long long>()(a ^ (b << 1));
}

bool LatLonKeyEq::operator()(LatLonKey const& a, LatLonKey const& b) const noexcept {
    return std::fabs(a.lat - b.lat) < 1e-8 &&
           std::fabs(a.lon - b.lon) < 1e-8;
}

bool Graph::loadFromGraphML(const std::string& filename) {
    XMLDocument doc;
    XMLError err = doc.LoadFile(filename.c_str());
    if (err != XML_SUCCESS) {
        std::cout << "Failed to load graphml file: " << filename << "\\n";
        return false;
    }

    XMLElement* root = doc.FirstChildElement("graphml");
    if (!root) {
        std::cout << "No <graphml> root element found.\\n";
        return false;
    }

    XMLElement* graph = root->FirstChildElement("graph");
    if (!graph) {
        std::cout << "No <graph> element found.\\n";
        return false;
    }

    // 1) node 읽기
    for (XMLElement* nodeElem = graph->FirstChildElement("node");
         nodeElem != nullptr;
         nodeElem = nodeElem->NextSiblingElement("node")) {

        const char* idStr = nodeElem->Attribute("id");
        if (!idStr) continue;
        long long id = std::atoll(idStr);

        double lat = 0.0, lon = 0.0;

        for (XMLElement* dataElem = nodeElem->FirstChildElement("data");
             dataElem != nullptr;
             dataElem = dataElem->NextSiblingElement("data")) {

            const char* key = dataElem->Attribute("key");
            const char* text = dataElem->GetText();
            if (!key || !text) continue;

            if (std::strcmp(key, "d4") == 0) {
                lat = std::atof(text);
            } else if (std::strcmp(key, "d5") == 0) {
                lon = std::atof(text);
            }
        }

        Node n;
        n.id = id;
        n.lat = lat;
        n.lon = lon;
        nodes.push_back(n);
        int idx = static_cast<int>(nodes.size()) - 1;
        id_to_index[id] = idx;

        LatLonKey k{lat, lon};
        coord_to_index[k] = idx;
    }

    // 2) edge 읽기
    for (XMLElement* edgeElem = graph->FirstChildElement("edge");
         edgeElem != nullptr;
         edgeElem = edgeElem->NextSiblingElement("edge")) {

        const char* srcStr = edgeElem->Attribute("source");
        const char* dstStr = edgeElem->Attribute("target");
        if (!srcStr || !dstStr) continue;

        long long srcId = std::atoll(srcStr);
        long long dstId = std::atoll(dstStr);

        double length = 0.0;

        for (XMLElement* dataElem = edgeElem->FirstChildElement("data");
             dataElem != nullptr;
             dataElem = dataElem->NextSiblingElement("data")) {

            const char* key = dataElem->Attribute("key");
            const char* text = dataElem->GetText();
            if (!key || !text) continue;

            // 프로젝트마다 length key id가 다를 수 있음(d15 등)
            if (std::strcmp(key, "d15") == 0 || std::strcmp(key, "d16") == 0) {
                length = std::atof(text);
            }
        }

        auto itS = id_to_index.find(srcId);
        auto itT = id_to_index.find(dstId);
        if (itS == id_to_index.end() || itT == id_to_index.end()) continue;

        int s = itS->second;
        int t = itT->second;
        nodes[s].adj.push_back({t, length});
        // 필요하면 무방향으로 사용:
        // nodes[t].adj.push_back({s, length});
    }

    // === Debug Info ===
    std::cout << "Loaded graph. nodes = " << nodes.size() << "\n";

    // edge 개수 계산해서 출력
    long long edgeCount = 0;
    for (const auto& n : nodes) edgeCount += n.adj.size();
    std::cout << "Total edges = " << edgeCount << "\n";

    // 첫 번째 노드 좌표 출력(테스트용)
    if (!nodes.empty()) {
        std::cout << "Example node[0]: lat=" << nodes[0].lat
            << " lon=" << nodes[0].lon << "\n";
    }

    return true;
}
int Graph::findNodeByCoord(double lat, double lon) const {
    double bestDist2 = std::numeric_limits<double>::infinity();
    int bestIdx = -1;

    for (size_t i = 0; i < nodes.size(); ++i) {
        double dlat = nodes[i].lat - lat;
        double dlon = nodes[i].lon - lon;
        double dist2 = dlat * dlat + dlon * dlon;

        if (dist2 < bestDist2) {
            bestDist2 = dist2;
            bestIdx = static_cast<int>(i);
        }
    }

    const double TH = 0.002;   // 대략 100m 범위
    if (bestDist2 > TH * TH) {
        return -1;
    }
    return bestIdx;
}