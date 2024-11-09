#ifndef __EDGE_H
#define __EDGE_H

#include "comparable.h"
#include <ostream>

class Edge : public Comparable<Edge>
{
public:
    int nodeX, nodeY, weight;
    
    Edge(int x, int y, int w);
    ~Edge() override;

    int                     compare     (Edge rhs) const override;
    friend std::ostream&    operator << (std::ostream& os, const Edge& e);
};

#endif//__EDGE_H
