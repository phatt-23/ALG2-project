#ifndef __HELPER_H
#define __HELPER_H

#include "edge.h"
#include "matrix.h"
#include "partition.h"
#include "disjoint_set.h"
#include <istream>
#include <vector>

class Helper 
{
public:
    static void writeAllTrees(
        std::ofstream& output, 
        const Graph& graph, 
        const std::vector<Partition>& ks
    );

    static void writeOnlyKth(
        std::ofstream& output, 
        const Graph& graph, 
        const std::vector<Partition>& ks
    );
    
    static Matrix<int> readAdjacencyMatrix(std::ifstream& inputStream);

    static std::vector<Edge> createEdges(const Matrix<int>& adjMat);

    static Partition* createPartition(
        const std::vector<int>& choices, 
        const Graph& g, 
        DisjointSet& ds
    );

    static std::vector<Partition> solve(const Graph& g);

    static void PrintTrees(
        const std::vector<Partition>& ks, 
        const Graph& graph, 
        int mode
    );

    static void testDups(const std::vector<Partition>& kts);

    static void testTrees(
        const std::vector<Partition>& ks, 
        const Graph& g
    );

    static void writeToHtml(
        const char* outputPath, 
        const char* headPath, 
        const char* tailPath, 
        const int mode,
        const Graph& g,
        const std::vector<Partition>& ks
    );
};

#endif//__HELPER_H
