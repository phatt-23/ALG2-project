#include "graph.h"

Graph::Graph(size_t vertexCount, std::vector<Edge>& edges) : 
    vertexCount(vertexCount), 
    edgeCount(edges.size()),
    edges(edges) 
{}
