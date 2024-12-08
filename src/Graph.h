#ifndef __GRAPH_H
#define __GRAPH_H

#include "Edge.h"
#include "IToString.h"
#include "Matrix.h"
#include "Vector.h"
#include <cstddef>

/// @brief Represents a graph consisting of vertices and edges.
/// 
/// The Graph class manages a collection of edges and vertices, allowing for
/// various graph operations and representations.
class Graph : public IToString
{
    size_t vertexCount; ///< The number of vertices in the graph.
    size_t edgeCount;   ///< The number of edges in the graph.
    Vector<Edge> edges; ///< A vector holding all edges in the graph.

    /// @brief Creates edges from an adjacency matrix.
    /// 
    /// This function parses the adjacency matrix to construct edges based on 
    /// the weights specified in the matrix.
    /// @param adjMat The adjacency matrix representing the graph.
    /// @return A vector of edges created from the adjacency matrix.
    Vector<Edge> createEdges(const Matrix<int>& adjMat);

public:
    /// @brief Default constructor for Graph.
    Graph() = default;

    /// @brief Constructs a Graph with a specified number of vertices and edges.
    /// @param vertexCount The number of vertices in the graph.
    /// @param edges A vector of edges to initialize the graph.
    Graph(size_t vertexCount, Vector<Edge>& edges);
    
    /// @brief Constructs a Graph from an adjacency matrix.
    /// @param adjMatrix The adjacency matrix to create the graph from.
    Graph(const Matrix<int>& adjMatrix);

    /// @brief Retrieves the number of vertices in the graph.
    /// @return The count of vertices.
    size_t VertexCount() const;

    /// @brief Retrieves the number of edges in the graph.
    /// @return The count of edges.
    size_t EdgeCount() const;

    /// @brief Retrieves the edges of the graph.
    /// @return A reference to the vector of edges.
    const Vector<Edge>& Edges() const;

    /// @brief Converts the graph to a string representation.
    /// 
    /// This function formats the graph as a string, including the counts of 
    /// vertices and edges and details of each edge.
    /// @return A string representation of the graph.
    std::string ToString() const override;
};

#endif // __GRAPH_H
