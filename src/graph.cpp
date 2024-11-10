#include "graph.h"
#include "helper.h"
#include <cassert>
#include <sstream>
#include <vector>

Graph::Graph(size_t vertexCount, std::vector<Edge>& edges) : 
    vertexCount(vertexCount), 
    edgeCount(edges.size()),
    edges(edges) 
{}

Graph::Graph(const Matrix<int>& adj) 
{
    assert(adj.Rows() == adj.Columns());
    std::vector<Edge> edges = createEdges(adj);
    this->vertexCount = adj.Rows();
    this->edgeCount = edges.size();
    this->edges = edges;
}


size_t Graph::VertexCount() const { return vertexCount; }
size_t Graph::EdgeCount() const { return edgeCount;}
const std::vector<Edge>& Graph::Edges() const { return edges; }

std::string Graph::ToString() const {
    std::stringstream ss;
    ss << "G = (V, E), |V| = " << this->vertexCount 
       << ", |E| = " << this->edgeCount << "\n";
    for (size_t i=0; i<edges.size(); ++i)
        ss << "e" << i << " = " << edges[i] << "\n";
    return ss.str();
}

