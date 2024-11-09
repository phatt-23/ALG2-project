#ifndef __PARTITION_H
#define __PARTITION_H

#include "comparable.h"
#include <vector>

class Partition : public Comparable<Partition>
{
public:
    std::vector<int> choices;   // 0, 1 or -1 per edge
    int mstCost;                // cost of the found MST
    std::vector<int> mstEdges;  // indexes in the list of edges
    
    Partition(std::vector<int> ch, int cost, std::vector<int> edges)
        : choices(ch), mstCost(cost), mstEdges(edges) {}
    
    int compare(Partition rhs) const override {
        return this->mstCost - rhs.mstCost;
    }
};

#endif//__PARTITION_H
