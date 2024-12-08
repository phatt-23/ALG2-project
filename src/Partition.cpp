#include "Partition.h"
#include <sstream>

Partition::Partition(Partition&& other) noexcept 
    : mstCost(other.mstCost), 
      choices(std::move(other.choices)), 
      mstEdges(std::move(other.mstEdges)) {}

Partition& Partition::operator=(Partition&& other) noexcept {
    if (this != &other) {
        mstCost = other.mstCost;
        choices = std::move(other.choices);
        mstEdges = std::move(other.mstEdges);
    }
    return *this;
}

Partition::Partition(Vector<int> ch, int cost, Vector<int> edges)
: mstCost(cost), choices(ch), mstEdges(edges) 
{}

Partition::Partition(size_t edgeCount)
: mstCost(0), 
    choices(Vector<int>(edgeCount)), 
    mstEdges(Vector<int>(edgeCount))
{}

void Partition::Reset() {
    this->choices.Clear();
    this->mstCost = 0;
    this->mstEdges.Clear();
}

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
