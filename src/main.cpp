#include "Partition.h"
#include "Graph.h"
#include "SpanningTreesFinder.h"
#include "Vector.h"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <vector>

int main(const int argc, const char** argv)
{
    using std::cout;

    // Simple command line argument checker.
    if (argc != 3) {
        cout << "Usage:\n";
        cout << "    kthmst <input_file> <print_bool>\n"; 
        cout << "        input_file        adjacent matrix\n"; 
        cout << "        print_type        0 - nothing\n"; 
        cout << "                          1 - only kth\n"; 
        cout << "                          2 - all\n"; 
        return 0;
    }
    
    // Read in the adjacencyMartix from the input file
    // and put it into a Matrix.
    std::ifstream input(argv[1]); 
    Matrix<int> adjMat = SpanningTreesFinder::readAdjacencyMatrix(input);

    // Construct a graph out of the adjacency matrix.
    // Debug print out. 
    Graph graph(adjMat); 
    cout << graph.ToString();

    // Check if it's a null graph. 
    // If so, no point in solving it.
    if (!graph.VertexCount() || !graph.EdgeCount()) {
        cout << "ERROR: Cannot solve for a tree with no vertices or edges...\n";
        return 0;
    }

    // Retrieve all the possible spanning trees 
    // and put it into a list.
    Vector<Partition> trees = SpanningTreesFinder::solve(graph);

    // Based on the inputted flag
    // vary the verbosity of debug printing.
    int mode = atoi(argv[2]);
    SpanningTreesFinder::PrintTrees(trees, graph, mode);
    
    // If there are any non-trees among the supposed spanning trees, 
    // find them and print them out.
    SpanningTreesFinder::testCycles(trees, graph);
   
    // If there are any duplicate trees, 
    // find them and print them out.
    SpanningTreesFinder::testDuplicates(trees);
  
    // Construct HTML document out of the found spanning trees.
    // Open in browser: `firefox ./treeees.html`
    SpanningTreesFinder::writeToHtml(
        "treeees.html",
        "./html-builder/head.html", 
        "./html-builder/tail.html", 
        mode, graph, trees 
    );

    return 0;
}

