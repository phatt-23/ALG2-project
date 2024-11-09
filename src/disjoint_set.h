#ifndef __DISJOINT_SET_H
#define __DISJOINT_SET_H

#include <stdexcept>
#include <vector>

class DisjointSet
{
public:
    std::vector<int> nodes; // node of index i links to node of index v that (the value of the i-th node
    std::vector<int> sizes; // sizes of each group, how many children does a node have
    int elemCount;          // number of elemes in whole set
    int numberOfComponents;

    DisjointSet(size_t elemCount);

    void    reset           ();
    int     find            (int nodeIndex);
    bool    nodesConnected  (int nodeX, int nodeY);
    int     componentSize   (int node);
    void    unify           (int nodeX, int  nodeY);
};

#endif//__DISJOINT_SET_H
