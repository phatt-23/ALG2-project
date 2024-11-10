#ifndef __GRAPH_H
#define __GRAPH_H

#include "edge.h"
#include "i_to_string.h"
#include "matrix.h"
#include <cstddef>
#include <vector>

class Graph : public IToString
{
    size_t vertexCount;
    size_t edgeCount;
    std::vector<Edge> edges;

    std::vector<Edge> createEdges(const Matrix<int>& adjMat);
public:
    Graph() = default;
    Graph(size_t vertexCount, std::vector<Edge>& edges);
    Graph(const Matrix<int>& adjMatrix);

    size_t VertexCount() const;
    size_t EdgeCount() const;
    const std::vector<Edge>& Edges() const;

    std::string ToString() const override;
};

#endif//__GRAPH_H
