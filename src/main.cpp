#include "partition.h"
#include "graph.h"
#include "helper.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <vector>

int main(const int argc, const char** argv) 
{
    using std::cout;

    if (argc != 3) {
        cout << "Usage:\n";
        cout << "    kthmst <input_file> <print_bool>\n"; 
        cout << "        input_file        adjacent matrix\n"; 
        cout << "        print_type        0 - nothing\n"; 
        cout << "                          1 - only kth\n"; 
        cout << "                          2 - all\n"; 
        return 0;
    }
    
    
    // read in the adjacencyMartix
    std::ifstream input(argv[1]); 

    Matrix<int> adjMat = Helper::readAdjacencyMatrix(input);

    // graph with useful to pass around
    Graph graph(adjMat); 
    cout << graph.ToString();

    if (!graph.VertexCount() || !graph.EdgeCount()) {
        cout << "ERROR: Cannot solve for a tree with no vertices or edges...\n";
        return 0;
    }

    // minimal spanning trees
    std::vector<Partition> ks = Helper::solve(graph); 

    int mode = atoi(argv[2]);
    Helper::PrintTrees(ks, graph, mode);
    
    // if any non-trees, prints them out
    Helper::testTrees(ks, graph); 
   

    // if finds any, prints them out
    Helper::testDups(ks); 
  
    Helper::writeToHtml(
        "treeees.html", 
        "./html-builder/head.html", 
        "./html-builder/tail.html", 
        mode, graph, ks
    );

    return 0;
}

