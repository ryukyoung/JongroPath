#pragma once
#include <vector>
#include "graph.h"

double randomPathSampling(const Graph& g, int start, int dest, int M, int N, std::vector<int>& outBestPath);