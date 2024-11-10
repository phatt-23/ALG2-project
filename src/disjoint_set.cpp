#include "disjoint_set.h"
#include <sstream>
#include <unordered_map>
#include <iostream>

DisjointSet::DisjointSet(size_t elemCount) : 
    nodes(elemCount), 
    sizes(elemCount),
    elemCount(elemCount),
    numberOfComponents(elemCount)
{
    if (elemCount <= 0) 
        throw new std::runtime_error("Size must be bigger than 0");
    Reset();
}

void DisjointSet::Reset() {
    numberOfComponents = elemCount;
    for (size_t i = 0; i < elemCount; ++i) {
        nodes[i] = i; // self-root, link to itself
        sizes[i] = 1; // each gruop has exactly 1 node at the beginning
    }
}

int DisjointSet::Find(int nodeIndex) {
    // standard root finding
    int rootIndex = nodeIndex;
    while (rootIndex != nodes[rootIndex])
        rootIndex = nodes[rootIndex];

    // path compression, compressing path leading to the rootIndex
    // extra work but will lead to amortized constant time complexity
    while (nodeIndex != rootIndex) {
        int nextIndex = nodes[nodeIndex];
        nodes[nodeIndex] = rootIndex;
        nodeIndex = nextIndex;
    }

    return rootIndex;
}

int DisjointSet::find(int nodeIndex) const {
    int rootIndex = nodeIndex;
    while (rootIndex != nodes[rootIndex])
        rootIndex = nodes[rootIndex];
    return rootIndex;
}

bool DisjointSet::NodesConnected(int nodeX, int nodeY) { 
    return Find(nodeX) == Find(nodeY); 
}

size_t DisjointSet::ComponentSize(int node) { 
    return sizes[Find(node)]; 
}

void DisjointSet::Unify(int nodeX, int  nodeY) {
    // find roots of both nodes X and Y
    int rootX = Find(nodeX);
    int rootY = Find(nodeY);
    
    // if are the same no work needed, already in the same set
    if (rootX == rootY) return;

    // merge two component together, smaller comp to larger comp
    if (sizes[rootX] < sizes[rootY]) {
        sizes[rootY] += sizes[rootX];
        nodes[rootX] = rootY;
    } else {
        sizes[rootX] += sizes[rootY];
        nodes[rootY] = rootX;
    }
    
    // there is now one less components
    numberOfComponents--; 
}

std::string 
DisjointSet::ToString() const {
    std::unordered_map<int, std::vector<int>> groups;

    for (size_t i=0; i < elemCount; ++i) {
        int n = nodes[i];
        groups[find(n)].push_back(i);
    }
    
    std::stringstream ss;

    ss << "(";
    for (auto [r, cs] : groups) {
        ss << "(" << r << "|";
        for (size_t i=0; i<cs.size(); ++i) {
            ss << cs[i] << (i==(cs.size()-1) ? "" : " ");
        }
        ss << ")";
    }
    ss << ")";

    return ss.str();
}
