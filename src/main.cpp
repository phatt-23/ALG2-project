#include "disjoint_set.h"
#include "partition.h"
#include "edge.h"
#include "graph.h"

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <vector>
#include <queue>

Partition createPartition(const std::vector<int>& choices, const Graph& graph) {
    DisjointSet ds(graph.vertexCount);
    
    int mstCost = 0;
    std::vector<int> mstEdges(graph.vertexCount-1, -123);

    int ptr = 0;
    // all edges set to 1 add them to the tree
    for (size_t i = 0; i < graph.edgeCount; ++i) {
        if (choices[i] == 1) {
            Edge e = graph.edges[i];
            ds.unify(e.nodeX, e.nodeY);
            mstEdges[ptr++] = i;
            mstCost += e.weight;
            std::cout << "\tunifying the 1 edges: " << ds.numberOfComponents << std::endl;
        }

    }
    
    // std::cout << "here-----------------------\n";

    for (size_t i = 0; i < graph.edgeCount; ++i) {
        if (ds.numberOfComponents == 1)
            break;

        if (choices[i] == 0) {
            Edge e = graph.edges[i];

            if (ds.find(e.nodeX) == ds.find(e.nodeY))
                continue;

            ds.unify(e.nodeX, e.nodeY);
            mstEdges[ptr++] = i;
            mstCost += e.weight;
            std::cout << "\tunifying the 0 edges: " << ds.numberOfComponents << std::endl;
        }

    }
    
    if (ds.numberOfComponents > 1) {
        throw new std::runtime_error("no tree");
    }

    std::sort(mstEdges.begin(), mstEdges.end(), [](int a, int b){ return a < b; });
    
    // if (ds.numberOfComponents == 1)
    //     std::cout << "cost: " << mstCost << '\n';
        
    return Partition(choices, mstCost, mstEdges);
}

int solve(size_t kth, const Graph& graph) {
    const size_t K = kth;

    auto partCmp = [](const Partition& l, const Partition& r) { return l.greater(r); };
    auto q = std::priority_queue<Partition, std::vector<Partition>, decltype(partCmp)>(partCmp);

    std::vector<int> choices(graph.edgeCount);
    for (size_t i = 0; i < graph.edgeCount; ++i)
        choices[i] = 0;

    std::cout << "choices: ";
    for (const auto& c : choices)
        std::cout << c << ' ';
    std::cout << "\n";

    q.push(createPartition(choices, graph));
    
    while (!q.empty()) {
        Partition p = q.top();
        q.pop();

        std::cout << "(K_" << K-kth << " = " << p.mstCost << ")\n";
        if (kth-- == 0) return p.mstCost;

        for (int i = 0; i < choices.size(); ++i) {
            if (p.choices[p.mstEdges[i]] == 0) {
                std::vector<int> newChoices = p.choices;

                newChoices[p.mstEdges[i]] = -1;     // exclude this edge
                for (int j = 0; j < i; ++j)         // every edge up to this definitely include
                    newChoices[p.mstEdges[j]] = 1;

                std::cout << "new_choices: ";
                for (const auto& c : newChoices)
                    std::cout << c << ' ';
                std::cout << '\n';

                try {                               // if no tree found we dont want it
                    Partition nextP = createPartition(newChoices, graph);
                    q.push(nextP);
                    std::cout << "(inner = " << nextP.mstCost << ")\n";
                } catch(std::runtime_error e) {
                    std::cout << e.what() << '\n';
                }
            }
        }
    }

    return -1;
}

int main(int argc, char** argv) {
    // if (argc != 2) {
    //     std::cout << "Usage: kthmst <input_file>\n"; 
    //     std::cout << "    input_file ... adjacent matrix\n"; 
    //     return -1;
    // }

    // read in the adjacencyMartix
    std::ifstream inputFile(
        "/home/phatt/School/3_semester/alg2/project/TestData/KMinimalniKostryGrafu/Graph1.txt"
    );
    
    size_t numberOfVertices = 0;
    inputFile >> numberOfVertices;

    std::cout << "|V| = " << numberOfVertices << '\n';
    
    int* adjMat = new int[numberOfVertices * numberOfVertices];
    for (size_t row = 0; row < numberOfVertices; ++row) {
        for (size_t col = 0; col < numberOfVertices; ++col) {
            inputFile >> adjMat[row * numberOfVertices + col];
        }
    }

    for (size_t row = 0; row < numberOfVertices; ++row) {
        std::cout << "V_" << row << " = ";
        for (size_t col = 0; col < numberOfVertices; ++col) {
            std::cout << adjMat[row * numberOfVertices + col] << " ";
        }
        std::cout << '\n';
    }
    
    std::vector<Edge> edges;

    for (size_t row = 0; row < numberOfVertices; ++row) {
        for (size_t col = row + 1; col < numberOfVertices; ++col) {
            int weight = adjMat[row * numberOfVertices + col];
            if (weight == 0) continue;
            edges.push_back(Edge(row, col, weight));
        }
    }
    
    auto edgeCmp = [](const Edge& l, const Edge& r) { return l.less(r); };

    std::sort(edges.begin(), edges.end(), edgeCmp);

    for (const Edge& e : edges) {
        std::cout << e << '\n';
    }

    Graph g(numberOfVertices, edges);
    
    try {
        solve(10, g);
    } catch (std::runtime_error) {
        std::cout << "error here-----------------------\n";
    }

    return 0;
}

