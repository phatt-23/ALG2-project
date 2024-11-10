#include "helper.h"
#include <cassert>
#include <iostream>
#include <algorithm>
#include <fstream>

Matrix<int> 
readAdjacencyMatrix(std::ifstream& inputStream) 
{
    size_t numOfVert = 0;
    inputStream >> numOfVert;

    // read in
    int* elems = new int[numOfVert * numOfVert];
    for (size_t i = 0; i < numOfVert * numOfVert; ++i)
        inputStream >> elems[i];
    
    auto adjMat = Matrix<int>(numOfVert, numOfVert, elems);

    return adjMat;
}

std::vector<Edge> 
createEdges(const Matrix<int>& adjMat) 
{
    assert(adjMat.Rows() == adjMat.Columns() && "Must be square matrix");

    std::vector<Edge> edges;
    size_t numOfVert = adjMat.Rows();
    
    // create edges from adjMat
    for (size_t row = 0; row < numOfVert; ++row) {
        for (size_t col = row + 1; col < numOfVert; ++col) {
            int weight = adjMat.Get(row, col);
            if (weight == 0) continue;
            edges.push_back(Edge(row, col, weight));
        }
    }
    
    std::sort(edges.begin(), edges.end(), [](Edge& l, Edge& r) { return l.Less(r); });
 
    return edges;
}
