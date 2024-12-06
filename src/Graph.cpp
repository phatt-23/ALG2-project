#include "Graph.h"
#include <cassert>
#include <sstream>
#include <vector>

Graph::Graph(size_t vertexCount, std::vector<Edge>& edges) : 
    vertexCount(vertexCount), 
    edgeCount(edges.size()),
    edges(edges) 
{}

std::vector<Edge> 
Graph::createEdges(const Matrix<int>& adjMat) 
{
    assert(adjMat.Rows() == adjMat.Columns() && "Must be square matrix");

    std::vector<Edge> edges;
    size_t numOfVert = adjMat.Rows();
    
    // create edges from adjMat
    for (size_t row = 0; row < numOfVert; ++row) {
        for (size_t col = row + 1; col < numOfVert; ++col) {
            int weight = adjMat.Get(row, col);
            if (weight == 0) continue;
            edges.push_back(Edge(row, col, weight));
        }
    }
    
    std::sort(edges.begin(), edges.end(), [](Edge& l, Edge& r) { return l.Less(r); });
 
    return edges;
}

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

