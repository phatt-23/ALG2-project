#ifndef __GRAPH_H
#define __GRAPH_H

#include "edge.h"
#include <cstddef>
#include <vector>

class Graph
{
public:
    size_t vertexCount;
    size_t edgeCount;
    std::vector<Edge> edges;

    Graph(size_t vertexCount, std::vector<Edge>& edges) : 
        vertexCount(vertexCount), edges(edges), edgeCount(edges.size()) {}
};

#endif//__GRAPH_H
