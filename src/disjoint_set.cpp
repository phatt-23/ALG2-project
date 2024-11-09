#include "disjoint_set.h"

DisjointSet::DisjointSet(size_t elemCount) : 
    nodes(elemCount), 
    sizes(elemCount),
    elemCount(elemCount),
    numberOfComponents(elemCount)
{
    if (elemCount <= 0) 
        throw new std::runtime_error("Size must be bigger than 0");
    reset();
}

void DisjointSet::reset() {
    numberOfComponents = elemCount;
    for (int i = 0; i < elemCount; ++i) {
        nodes[i] = i; // self-root, link to itself
        sizes[i] = 1; // each gruop has exactly 1 node at the beginning
    }
}

int DisjointSet::find(int nodeIndex) {
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

bool DisjointSet::nodesConnected(int nodeX, int nodeY) { 
    return find(nodeX) == find(nodeY); 
}

int DisjointSet::componentSize(int node) { 
    return sizes[find(node)]; 
}

void DisjointSet::unify(int nodeX, int  nodeY) {
    // find roots of both nodes X and Y
    int rootX = find(nodeX);
    int rootY = find(nodeY);
    
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
