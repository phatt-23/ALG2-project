#ifndef __PARTITION_H
#define __PARTITION_H

#include "i_comparable.h"
#include "i_to_string.h"
#include "graph.h"
#include <ostream>
#include <string>
#include <vector>
#include <iostream>

struct Partition : public IComparable<Partition>, public IToString
{
public:
    std::vector<int> choices;   // 0, 1 or -1 per edge
    int mstCost;                // cost of the found MST
    std::vector<int> mstEdges;  // indexes in the list of edges
    
    Partition(std::vector<int> ch, int cost, std::vector<int> edges);
    // copy constructors
    Partition(const Partition& p) = default;
    Partition& operator=(const Partition& p) = default; 


    std::string ToString() const override;
    std::string ToString(const Graph& g) const;
    
    int Compare(const Partition& rhs) const override;
    bool operator < (const Partition& r) const;
    bool operator > (const Partition& r) const;
    bool operator <= (const Partition& r) const;
    bool operator >= (const Partition& r) const;
    bool operator == (const Partition& r) const;
    bool operator != (const Partition& r) const;

    friend std::ostream& operator << (std::ostream& os, const Partition& p);
};

#endif//__PARTITION_H
