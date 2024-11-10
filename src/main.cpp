#include "disjoint_set.h"
#include "partition.h"
#include "edge.h"
#include "graph.h"
#include "helper.h"
#include "binary_heap.h"

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <vector>

/// Finds the MST of the given search space.
/// If no tree is possible to construct given 
/// the search space, nullptr is returned.
Partition* 
createPartition(const std::vector<int>& choices, const Graph& g, DisjointSet& ds) 
{
    ds.Reset(); // resets the disjoint set, using the same memory again
    
    int mstCost = 0;
    std::vector<int> mstEdges(g.VertexCount() - 1);
    std::fill(mstEdges.begin(), mstEdges.end(), -1);

    int eIdx = 0;
    for (size_t x=0; x<g.EdgeCount(); x++)
    {
        if (choices[x] == 1)
        {
            Edge e = g.Edges()[x];
            ds.Unify(e.nodeX, e.nodeY);
            mstEdges[eIdx++] = x;
            mstCost += e.weight;
        }
    }

    for (size_t x=0; x<g.EdgeCount(); x++) 
    {
        if (ds.numberOfComponents == 1)
            break;

        if (choices[x] == 0)
        {
            Edge e = g.Edges()[x];
            if (!ds.NodesConnected(e.nodeX, e.nodeY))
            {
                ds.Unify(e.nodeX, e.nodeY);
                mstEdges[eIdx++] = x;
                mstCost += e.weight;
            }
        }
    }

    if (ds.numberOfComponents > 1)
        return nullptr;

    std::sort(mstEdges.begin(), mstEdges.end(), [](int l, int r){ return l < r; });

    return new Partition(choices, mstCost, mstEdges);
}

/// Finds all the spanning trees and puts
/// them into an array ordered ascendingly 
/// by their cost.
std::vector<Partition>
solve(Graph g) 
{
    std::vector<Partition> ks;
    DisjointSet ds(g.VertexCount());

    auto bhComp = [](const Partition* a, const Partition* b){ return *a > *b; };
    auto bh = BinaryHeap<Partition*, decltype(bhComp)>(bhComp);
    
    std::vector<int> initChoices(g.EdgeCount());
    fill(initChoices.begin(), initChoices.end(), 0);

    Partition* mst = createPartition(initChoices, g, ds);
    ks.push_back(*mst);
    bh.Insert(mst); 

    while (!bh.Empty()) 
    {
        Partition* p = bh.Poll();
        
        for (size_t x=0; x<g.VertexCount()-1; x++) 
        {
            if (p->choices[p->mstEdges[x]] == 0)
            {
                std::vector<int> choices(p->choices);

                choices[p->mstEdges[x]] = 3;
                
                for (size_t y=0; y<x; y++)
                    choices[p->mstEdges[y]] = 1;

                Partition* nxt = createPartition(choices, g, ds);
                if (nxt == nullptr)
                    continue;

                bh.Insert(nxt); 
                ks.push_back(*nxt);

                // std::cout << *nxt << "\n"; // sick
            }
        }

        delete p;
    }

    auto cmp = [](const Partition& l, const Partition& r) { return l.Less(r); };
    sort(ks.begin(), ks.end(), cmp);

    return ks;
}

// Test for duplicit trees.
void testDups(const std::vector<Partition>& kts) {
    using std::cout;
    std::vector<Partition> ks(kts);
    cout << "INFO: Testing for duplicities...\n";

    auto cmp = [](const Partition& l, const Partition& r) { 
        return l.choices == r.choices; // choices are unique as are mstEdges
    };
    sort(ks.begin(), ks.end(), cmp);

    int dupCount=0; // show that every tree is unique
    for (size_t i=0; i<ks.size()-1; i++) {
        if (ks[i].choices == ks[i+1].choices) {
            cout << "Found-duplicate (" << ++dupCount << ")\n";
            cout << ks[i] << "\n" << ks[i+1] << "\n";
        }
    }

    if (!dupCount) {
        cout << "DONE: Found none\n";
        return;
    }
    
    cout << "DONE: Found " << dupCount << " dups\n";
}

/// Test that graphs in ks are all trees, 
/// meaning they have no cycles.
void testTrees(const std::vector<Partition>& ks, const Graph& g) {
    using std::cout;
    // show that all "trees" are actual trees
    cout << "INFO: Testing for cycles...\n";
    int nonTreeCount=0;

    DisjointSet ds(g.VertexCount());
    for (const Partition& k : ks) 
    {
        ds.Reset();
        for (int ke : k.mstEdges) 
        {
            Edge e = g.Edges()[ke];
            if (ds.NodesConnected(e.nodeX, e.nodeY)) {
                std::cout << "Not-a-tree " << k.ToString() << "\n";
                nonTreeCount++;
                break;
            }
            ds.Unify(e.nodeX, e.nodeY);
        }
    }

    if (!nonTreeCount) {
        cout << "DONE: Found none\n";
        return;
    }
    
    cout << "DONE: Found " << nonTreeCount << " non-trees\n";
}

int 
main(int argc, char** argv) 
{
    using std::cout;

    if (argc != 3) {
        cout << "Usage:\n";
        cout << "    kthmst <input_file> <print_bool>\n"; 
        cout << "        input_file        adjacent matrix\n"; 
        cout << "        print_bool        print all trees (slow IO)\n"; 
        return 0;
    }
    
    std::ifstream inputStream(argv[1]); 

    // read in the adjacencyMartix
    Matrix<int> adjMat = readAdjacencyMatrix(inputStream);

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

