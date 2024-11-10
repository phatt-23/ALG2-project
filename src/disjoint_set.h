#ifndef __DISJOINT_SET_H
#define __DISJOINT_SET_H

#include "i_to_string.h"
#include <stdexcept>
#include <vector>

// for all the thing that can be mapped to whole numbers
class DisjointSet : public IToString
{
    int find(int nodeIndex) const; // without path compression
public:
    std::vector<int> nodes; // node of index i links to node of index v that (the value of the i-th node
    std::vector<int> sizes; // sizes of each group, how many children does a node have
    size_t elemCount;          // number of elemes in whole set
    size_t numberOfComponents;

    DisjointSet(size_t elemCount);

    void Reset();
    int Find(int nodeIndex); // with path compression
    bool NodesConnected(int nodeX, int nodeY);
    size_t ComponentSize(int node);
    void Unify(int nodeX, int  nodeY);

    std::string ToString() const override;
};

#endif//__DISJOINT_SET_H
