#ifndef __HELPER_SHIT_H
#define __HELPER_SHIT_H

#include "edge.h"
#include <istream>
#include <vector>

int* readInAdjMatrix(std::ifstream& inputStream, size_t numOfVert);

std::vector<Edge> createEdges(const int* adjMat, int numOfVert);

#endif//__HELPER_SHIT_H
