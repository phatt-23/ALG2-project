#ifndef __PARTITION_H
#define __PARTITION_H

#include "IComparable.h"
#include "IToString.h"
#include "Graph.h"
#include <ostream>
#include <string>
#include <vector>
#include <iostream>

/// @brief Represents a partition of edges in a graph.
struct Partition : public IComparable<Partition>, public IToString
{
public:
    enum EdgeChoice {
        NOT_ASSESSED = 0,
        EXCLUDED = -1,
        INCLUDED = 1,
    };

    int mstCost;                // Cost of the found MST
    std::vector<int> choices;   // 0, 1 or -1 per edge (use the enum for all practical purposes)
    std::vector<int> mstEdges;  // Indexes in the list of edges

    /// @brief Constructor that initializes a partition with a specified edge count.
    Partition(size_t edgeCount);

    /// @brief Constructor that initializes a partition with given choices, cost, and edges.
    Partition(std::vector<int> ch, int cost, std::vector<int> edges);

    // Copy constructors
    Partition(const Partition& p) = default;
    Partition& operator=(const Partition& p) = default; 

    // Copy constructors (rvalue)
    Partition(Partition&& other) noexcept;
    Partition& operator=(Partition&& other) noexcept;

    /// @brief Resets the partition to its initial state.
    void Reset();

    std::string ToString() const override;                    // String representation
    std::string ToString(const Graph& g) const;               // String with graph info

    int Compare(const Partition& rhs) const override;         // Compare based on MST cost
    
    // Comparison operators, they work with the mstCost
    bool operator < (const Partition& r) const;
    bool operator > (const Partition& r) const;
    bool operator <= (const Partition& r) const;
    bool operator >= (const Partition& r) const;
    bool operator == (const Partition& r) const;
    bool operator != (const Partition& r) const;

    /// @brief Outputs the partition details to an output stream.
    friend std::ostream& operator << (std::ostream& os, const Partition& p);
};

#endif // __PARTITION_H
