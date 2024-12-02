#ifndef __EDGE_H
#define __EDGE_H

#include "i_comparable.h" 
#include <ostream> 

/// @brief Represents an edge in a graph, connecting two nodes with a weight.
/// 
/// The Edge structure implements the IComparable interface, allowing edges to
/// be compared based on their weights.
struct Edge : public IComparable<Edge>
{
    int nodeX; // The first node of the edge
    int nodeY; // The second node of the edge
    int weight; // The weight of the edge, representing the cost/distance

    /// @brief Constructs an Edge with specified node indices and weight.
    /// @param x The index of the first node.
    /// @param y The index of the second node.
    /// @param w The weight of the edge.
    Edge(int x, int y, int w);
    
    /// @brief Destroys the Edge object.
    ~Edge() override;

    /// @brief Compares the current edge with another edge based on weight.
    /// 
    /// This method returns a positive value if this edge is heavier, a negative value 
    /// if it is lighter, and zero if both edges are equal in weight.
    /// @param rhs The edge to compare against.
    /// @return An integer representing the comparison result.
    int Compare(const Edge& rhs) const override;

    /// @brief Overloads the output stream operator to print edge information.
    /// 
    /// This function formats the edge output as (nodeX.nodeY|weight) for display.
    /// @param os The output stream to write to.
    /// @param e The edge to print.
    /// @return The output stream after writing the edge information.
    friend std::ostream& operator<<(std::ostream& os, const Edge& e);
};

#endif // __EDGE_H
