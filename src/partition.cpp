#include "partition.h"
#include <sstream>

Partition::Partition(std::vector<int> ch, int cost, std::vector<int> edges)
: choices(ch), mstCost(cost), mstEdges(edges) 
{}

std::string 
Partition::ToString() const {
    std::stringstream ss;
    ss << "(\n  choices = [ ";
    for (int c : choices) ss << c << " ";

    ss << "]\n  indices = [ ";
    for (int e : mstEdges) ss << e << " ";

    ss << "]\n     cost = " << mstCost << "\n)";
    return ss.str();
}

std::string 
Partition::ToString(const Graph& g) const {
    std::stringstream ss;
    ss << "(\n  choices = [ ";
    // for (int c : choices) ss << ((c < 0) ? "" : " ") << c << " ";
    for (int c : choices) ss << c << " ";

    ss << "]\n  indices = [ ";
    for (int e : mstEdges) ss << e << " ";

    ss << "]\n    edges = [ ";
    for (int e : mstEdges) ss << g.Edges()[e] << " ";

    ss << "]\n     cost = " << mstCost << "\n)";
    return ss.str();
}

std::ostream& 
operator << (std::ostream& os, const Partition& p) {
    return os << p.ToString();
}

int 
Partition::Compare(const Partition& rhs) const {
    return this->mstCost - rhs.mstCost;
}

bool 
Partition::operator < (const Partition& r) const {
    return this->mstCost < r.mstCost;
}

bool 
Partition::operator > (const Partition& r) const {
    return this->mstCost > r.mstCost;
}

bool 
Partition::operator <= (const Partition& r) const {
    return this->mstCost <= r.mstCost;
}

bool 
Partition::operator >= (const Partition& r) const {
    return this->mstCost >= r.mstCost;
}

bool 
Partition::operator == (const Partition& r) const {
    return this->mstCost == r.mstCost;
}

bool 
Partition::operator != (const Partition& r) const {
    return this->mstCost != r.mstCost;
}
