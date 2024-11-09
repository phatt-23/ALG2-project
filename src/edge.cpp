#include "edge.h"

Edge::Edge(int x, int y, int w) : nodeX(x), nodeY(y), weight(w) {}
Edge::~Edge() = default;

int Edge::compare(Edge rhs) const {
    return this->weight - rhs.weight;
}

std::ostream& operator << (std::ostream& os, const Edge& e) {
    os << "(" << e.nodeX << "-" << e.nodeY << "|" << e.weight << ")";
    return os;
}
