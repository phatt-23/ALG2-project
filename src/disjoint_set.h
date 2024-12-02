#ifndef __DISJOINT_SET_H
#define __DISJOINT_SET_H

#include "i_to_string.h"
#include <stdexcept>
#include <vector>

/// @brief Disjoint Set data structure (Union-Find) for efficiently managing disjoint sets of elements.
/// Designed for any set of elements that can be mapped to integers.
class DisjointSet : public IToString
{
private:
    /// @brief Finds the root of a node without path compression.
    /// @param nodeIndex Index of the node.
    /// @return The root of the node.
    int find(int nodeIndex) const;

public:
    std::vector<int> nodes; ///< Parent links for each node (node i links to nodes[i]).
    std::vector<int> sizes; ///< Size of each component, stored at the root node.
    size_t elemCount; ///< Total number of elements in the set.
    size_t numberOfComponents; ///< Current number of disjoint components.

    /// @brief Default constructor for an empty Disjoint Set.
    DisjointSet() = default;

    /// @brief Constructs a Disjoint Set with a specified number of elements.
    /// @param elemCount Number of elements in the set.
    /// @throws std::runtime_error if `elemCount` is zero or negative.
    DisjointSet(size_t elemCount);

    /// @brief Resets the Disjoint Set to its initial state with all elements in separate components.
    void Reset();

    /// @brief Finds the root of a node with path compression for optimization.
    /// @param nodeIndex Index of the node.
    /// @return The root of the node.
    int Find(int nodeIndex);

    /// @brief Checks if two nodes belong to the same component.
    /// @param nodeX First node index.
    /// @param nodeY Second node index.
    /// @return `true` if the nodes share the same root, otherwise `false`.
    bool NodesConnected(int nodeX, int nodeY);

    /// @brief Retrieves the size of the component containing a given node.
    /// @param node Node index.
    /// @return Size of the component.
    size_t ComponentSize(int node);

    /// @brief Unifies the components containing two nodes by merging smaller components into larger ones.
    /// @param nodeX First node index.
    /// @param nodeY Second node index.
    void Unify(int nodeX, int nodeY);

    /// @brief Converts the Disjoint Set structure into a human-readable string.
    /// @return String representation of the disjoint set, with groups displayed as `(root|elements)`.
    std::string ToString() const override;
};

#endif // __DISJOINT_SET_H
