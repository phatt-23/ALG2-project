#ifndef __HELPER_SHIT_H
#define __HELPER_SHIT_H

#include "edge.h"
#include "matrix.h"
#include <istream>
#include <vector>

Matrix<int> readAdjacencyMatrix(std::ifstream& inputStream);

std::vector<Edge> createEdges(const Matrix<int>& adjMat);

#endif//__HELPER_SHIT_H
