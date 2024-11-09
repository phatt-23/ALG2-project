#include "helper.h"
#include <iostream>
#include <ostream>
#include <algorithm>
#include <fstream>

int* readInAdjMatrix(std::ifstream& inputStream, size_t numOfVert) {
    int* adjMat = new int[numOfVert * numOfVert];
    
    // read in
    for (size_t row = 0; row < numOfVert; ++row) {
        for (size_t col = 0; col < numOfVert; ++col) {
            inputStream >> adjMat[row * numOfVert + col];
        }
    }
    
    // debug print
    std::cout << "|V| = " << numOfVert << '\n';
    for (size_t row = 0; row < numOfVert; ++row) {
        std::cout << "v_" << row << " = ";
        for (size_t col = 0; col < numOfVert; ++col) {
            std::cout << adjMat[row * numOfVert + col] << " ";
        }
        std::cout << '\n';
    }

    return adjMat;
}

std::vector<Edge> createEdges(const int* adjMat, int numOfVert) {
    std::vector<Edge> edges;
    
    // create
    for (size_t row = 0; row < numOfVert; ++row) {
        for (size_t col = row + 1; col < numOfVert; ++col) {
            int weight = adjMat[row * numOfVert + col];
            if (weight == 0) continue;
            edges.push_back(Edge(row, col, weight));
        }
    }
    
    std::sort(edges.begin(), edges.end(), [](Edge& l, Edge& r) { return l.less(r); });
    
    // print
    int ptr = 0;
    for (const Edge& e : edges)
        std::cout << "[" << ptr++ << "]\t" << e << '\n';
    
    return edges;
}
