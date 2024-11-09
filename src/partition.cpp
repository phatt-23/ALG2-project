#include "partition.h"

Partition::Partition(std::vector<int> ch, int cost, std::vector<int> edges)
    : choices(ch), mstCost(cost), mstEdges(edges) {}
    
int Partition::compare(Partition rhs) const {
    return this->mstCost - rhs.mstCost;
}

void Partition::print() const {
    std::cout << "(\n";
    std::cout << "\t" << "Choices: ";
    for (int c : choices) std::cout << ((c != -1)? " " : "") << c << " ";
    std::cout << "\n";
    std::cout << "\t" << "MstEdges: ";
    for (int e : mstEdges) std::cout << e << " ";
    std::cout << "\n";
    std::cout << "\t" << "MstCost: " << mstCost;
    std::cout << "\n)\n";
}

void Partition::print(const Graph& g) const {
    std::cout << "(\n";
    std::cout << "\t" << "Choices: ";
    for (int c : choices) std::cout << ((c != -1)? " " : "") << c << " ";
    std::cout << "\n";
    std::cout << "\t" << "MstEdges (indices): ";
    for (int e : mstEdges) std::cout << e << " ";
    std::cout << "\n";
    std::cout << "\t" << "MstEdges (edges): ";
    for (int e : mstEdges) std::cout << g.edges[e] << " ";
    std::cout << "\n";
    std::cout << "\t" << "MstCost: " << mstCost;
    std::cout << "\n)\n";
}
