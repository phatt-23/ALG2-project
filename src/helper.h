#ifndef __HELPER_SHIT_H
#define __HELPER_SHIT_H

#include "edge.h"
#include "matrix.h"
#include "partition.h"
#include "disjoint_set.h"
#include <istream>
#include <vector>

Matrix<int> readAdjacencyMatrix(std::ifstream& inputStream);

std::vector<Edge> createEdges(const Matrix<int>& adjMat);

Partition* createPartition(const std::vector<int>& choices, const Graph& g, DisjointSet& ds);

std::vector<Partition> solve(Graph g);

void testDups(const std::vector<Partition>& kts);

void testTrees(const std::vector<Partition>& ks, const Graph& g);

#endif//__HELPER_SHIT_H
