#ifndef __DISJOINT_SET_H
#define __DISJOINT_SET_H

#include "IToString.h"
#include "ComparableType.h"
#include "Vector.h"

#include <stdexcept>
#include <unordered_map>
#include <sstream>

/// @brief Disjoint Set data structure (Union-Find) for efficiently managing disjoint sets of elements.
template <Comparable T>
class DisjointSet final : public IToString
{
private:
    /// @brief Finds the root of a node without path compression.
    /// @param nodeIndex Index of the node.
    /// @return The root of the node.
    int find(T nodeIndex) const;

public:

    ///< Parent links for each node (node i links to nodes[i]).
    Vector<T> nodes;
    ///< Size of each component, stored at the root node.
    Vector<T> sizes;
    ///< Total number of elements in the set.
    size_t elemCount{};
    ///< Current number of disjoint components.
    size_t numberOfComponents{};

    /// @brief Default constructor for an empty Disjoint Set.
    DisjointSet() = default;

    /// @brief Constructs a Disjoint Set with a specified number of elements.
    /// @param elemCount Number of elements in the set.
    /// @throws std::runtime_error if `elemCount` is zero or negative.
    explicit DisjointSet(size_t elemCount);

    /// @brief Resets the Disjoint Set to its initial state with all elements in separate components.
    void Reset();

    /// @brief Finds the root of a node with path compression for optimization.
    /// @param nodeIndex Index of the node.
    /// @return The root of the node.
    int Find(T nodeIndex);

    /// @brief Checks if two nodes belong to the same component.
    /// @param nodeX First node index.
    /// @param nodeY Second node index.
    /// @return `true` if the nodes share the same root, otherwise `false`.
    bool NodesConnected(T nodeX, T nodeY);

    /// @brief Retrieves the size of the component containing a given node.
    /// @param node Node index.
    /// @return Size of the component.
    size_t ComponentSize(T node);

    /// @brief Unifies the components containing two nodes by merging smaller components into larger ones.
    /// @param nodeX First node index.
    /// @param nodeY Second node index.
    void Unify(T nodeX, T nodeY);

    /// @brief Converts the Disjoint Set structure into a human-readable string.
    /// @return String representation of the disjoint set, with groups displayed as `(root|elements)`.
    [[nodiscard]] std::string ToString() const override;
};

// Implementation

template <Comparable T>
DisjointSet<T>::DisjointSet(size_t elemCount) : 
    nodes(elemCount), 
    sizes(elemCount),
    elemCount(elemCount),
    numberOfComponents(elemCount)
{
    if (elemCount <= 0) 
        throw new std::runtime_error("Size must be bigger than 0");
    Reset();
}


template <Comparable T>
void DisjointSet<T>::Reset() {
    numberOfComponents = elemCount;
    for (size_t i = 0; i < elemCount; ++i) {
        nodes.Insert(i, i); // self-root, link to itself
        sizes.Insert(i, 1); // each gruop has exactly 1 node at the beginning
    }
}

template <Comparable T>
int DisjointSet<T>::Find(T nodeIndex) {
    // standard root finding
    int rootIndex = nodeIndex;
    while (rootIndex != nodes[rootIndex])
        rootIndex = nodes[rootIndex];

    // path compression, compressing path leading to the rootIndex
    // extra work but will lead to amortized constant time complexity
    while (nodeIndex != rootIndex) {
        int nextIndex = nodes[nodeIndex];
        nodes[nodeIndex] = rootIndex;
        nodeIndex = nextIndex;
    }

    return rootIndex;
}

template <Comparable T>
int DisjointSet<T>::find(T nodeIndex) const {
    int rootIndex = nodeIndex;
    while (rootIndex != nodes[rootIndex])
        rootIndex = nodes[rootIndex];
    return rootIndex;
}

template <Comparable T>
bool DisjointSet<T>::NodesConnected(T nodeX, T nodeY) { 
    return Find(nodeX) == Find(nodeY); 
}

template <Comparable T>
size_t DisjointSet<T>::ComponentSize(T node) { 
    return sizes[Find(node)]; 
}

template <Comparable T>
void DisjointSet<T>::Unify(T nodeX, T nodeY) {
    // find roots of both nodes X and Y
    int rootX = Find(nodeX);
    int rootY = Find(nodeY);
    
    // if are the same no work needed, already in the same set
    if (rootX == rootY) return;

    // merge two component together, smaller comp to larger comp
    if (sizes[rootX] < sizes[rootY]) {
        sizes[rootY] += sizes[rootX];
        nodes[rootX] = rootY;
    } else {
        sizes[rootX] += sizes[rootY];
        nodes[rootY] = rootX;
    }
    
    // there is now one less components
    numberOfComponents--; 
}

template <Comparable T>
std::string 
DisjointSet<T>::ToString() const {
    std::unordered_map<int, Vector<int>> groups;

    for (size_t i=0; i < elemCount; ++i) {
        int n = nodes[i];
        groups[find(n)].PushBack(i);
    }
    
    std::stringstream ss;

    ss << "(";
    for (auto [r, cs] : groups) {
        ss << "(" << r << "|";
        for (size_t i=0; i<cs.Size(); ++i) {
            ss << cs[i] << (i==(cs.Size()-1) ? "" : " ");
        }
        ss << ")";
    }
    ss << ")";

    return ss.str();
}

#endif // __DISJOINT_SET_H

