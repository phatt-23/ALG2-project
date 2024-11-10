#include "partition.h"
#include "graph.h"
#include "helper.h"

#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <vector>

int main(int argc, char** argv) 
{
    using std::cout;

    if (argc != 3) {
        cout << "Usage:\n";
        cout << "    kthmst <input_file> <print_bool>\n"; 
        cout << "        input_file        adjacent matrix\n"; 
        cout << "        print_bool        print all trees (slow IO)\n"; 
        return 0;
    }
    
    std::ifstream input(argv[1]); // read in the adjacencyMartix

    Matrix<int> adjMat = readAdjacencyMatrix(input);

    Graph g(adjMat); // graph with useful to pass around
    cout << g.ToString();
    
    if (!g.VertexCount() || !g.EdgeCount()) {
        cout << "ERROR: Cannot solve for a tree with no vertices or edges...\n";
        return 0;
    }

    std::vector<Partition> ks = solve(g); // minimal spanning trees

    cout << "Found " << ks.size() << " trees, from cost of " 
         << ks.front().mstCost << " to " << ks.back().mstCost << "\n";

    if (atoi(argv[2]) == 1) { // if the third argument is 1, then print trees
        for (size_t i=0; i<ks.size(); ++i) {
            cout << "[" << i << "]\n";
            cout << ks[i].ToString(g) << "\n";
        }
    }
    
    testTrees(ks, g); // if any non-trees, prints them out
    testDups(ks); // if finds any, prints them out
   
    return 0;
}

