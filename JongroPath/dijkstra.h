#pragma once
#include <vector>
#include "graph.h"

double dijkstraShortestPath(const Graph& g, int start, int dest, std::vector<int>& outPath);