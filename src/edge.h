#ifndef __EDGE_H
#define __EDGE_H

#include "i_comparable.h"
#include <ostream>

struct Edge : public IComparable<Edge>
{
    int nodeX, nodeY, weight;
    
    Edge(int x, int y, int w);
    ~Edge() override;

    int Compare(const Edge& rhs) const override;
    friend std::ostream& operator << (std::ostream& os, const Edge& e);
};

#endif//__EDGE_H
