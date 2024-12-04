#include "helper.h"
#include "partition.h"
#include "graph.h"
#include "disjoint_set.h"
#include "binary_heap.h"

#include <cassert>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>


Matrix<int> 
Helper::readAdjacencyMatrix(std::ifstream& inputStream) 
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
Helper::createEdges(const Matrix<int>& adjMat) 
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

/// Finds all the spanning trees and puts
/// them into an array ordered ascendingly 
/// by their cost.
std::vector<Partition>
Helper::solve(const Graph& graph) 
{
    // Storage for the MSTs.
    std::vector<Partition> spanningTrees;          
    
    // Shared among all iterations, stores found vertices.
    // Used in the createPartition function for Kruskal's.
    DisjointSet<int> disjointSet(graph.VertexCount());    

    // Ordinary binary heap to store the partitions 
    // (search spaces - holds info about the spanning tree)
    // in a way, so that its always ready to serve the partition
    // with the least mstCost.
    auto partCmp = [](const Partition* l, const Partition* r) { 
        return l->Less(*r); 
    };
    auto partitions = BinaryHeap<Partition*, decltype(partCmp)>(partCmp);    
    
    // Initial state is choice where all the edges all not assessed.
    std::vector<int> initChoices(graph.EdgeCount());                    
    fill(initChoices.begin(), initChoices.end(), Partition::NOT_ASSESSED);

    // Find the actual MST, put it in the storage of STs
    Partition* mst = createPartition(initChoices, graph, disjointSet);
    spanningTrees.push_back(*mst);
    partitions.Insert(mst); 

    // while all the search spaces still werent 
    // searched through, continue searching
    while (!partitions.Empty()) 
    {
        // search this partition's search space
        Partition* part = partitions.Poll();
        
        // make a new choice describing the search space
        // and see if a spanning tree is possible in this space
        // if yes, add it to the mstList 
        for (size_t x = 0; x < graph.VertexCount() - 1; x++) 
        {
            // matches the first still not assessed choice
            // and evaluates this space
            if (part->choices[part->mstEdges[x]] == Partition::EdgeChoice::NOT_ASSESSED)
            {
                // copy the choices of the previous iteration
                std::vector<int> choices(part->choices);

                // mark current as excluded and try a tree is possible
                choices[part->mstEdges[x]] = Partition::EdgeChoice::EXCLUDED;
                
                // mark all the previous choices that had already been assessed
                for (size_t y = 0; y < x; y++)
                    choices[part->mstEdges[y]] = Partition::EdgeChoice::INCLUDED;

                // try find a spanning tree
                Partition* nxt = createPartition(choices, graph, disjointSet);
                
                // if the nxt pointer is nulled then no spanning tree was found 
                if (nxt == nullptr) 
                    continue;

                // otherwise insert the newly found spanning tree into the heap
                // and add it to the list of valid spanning trees
                partitions.Insert(nxt); 
                spanningTrees.push_back(*nxt);
            }
        }
        
        // the partition is not needed anymore as this space was already searched
        delete part;
    }

    // sort them by mstWeight and return the list (vector)
    sort(spanningTrees.begin(), spanningTrees.end(), 
        [](const Partition& l, const Partition& r){ return l.Less(r); });

    return spanningTrees;
}

/// Finds the MST of the given search space.
/// If no tree is possible to construct given 
/// the search space, nullptr is returned.
/// Using Kruskal's algorithm.
Partition* 
Helper::createPartition(const std::vector<int>& choices, const Graph& graph, DisjointSet<int>& ds) 
{
    ds.Reset(); // Resets the disjoint set, reusing the same memory again.
    
    int mstCost = 0;
    std::vector<int> mstEdges(graph.VertexCount() - 1);
    std::fill(mstEdges.begin(), mstEdges.end(), -1); // first fill in with -1

    int eIdx = 0;
    
    // Add all the edges that are set to be included.
    for (size_t i = 0; i < graph.EdgeCount(); i++)
    {
        // If not yet found, add it, vertices wont be dupped thanks to the DS.
        if (choices[i] == Partition::EdgeChoice::INCLUDED) 
        {
            Edge e = graph.Edges()[i];
            ds.Unify(e.nodeX, e.nodeY);
            mstEdges[eIdx++] = i;
            mstCost += e.weight;
        }
    }

    for (size_t i = 0; i < graph.EdgeCount(); i++) 
    {
        // If its already connected then no additional edge is needed
        // break out
        if (ds.numberOfComponents == 1) 
            break;

        // Try adding edges still not included
        // hopefully they will compose a spanning tree.
        if (choices[i] == Partition::EdgeChoice::NOT_ASSESSED)            
        {
            Edge e = graph.Edges()[i];
            if (!ds.NodesConnected(e.nodeX, e.nodeY))
            {
                ds.Unify(e.nodeX, e.nodeY);
                mstEdges[eIdx++] = i;
                mstCost += e.weight;
            }
        }
    }

    // If no spanning tree is possible in this search space
    // toss this partition away as it's not possible for 
    // this search spaces to contain any spanning tree.
    // -> return NULL
    if (ds.numberOfComponents > 1) {
        return nullptr;
    }

    // If a spanning tree is possible, 
    // sort the edges by index and return the valid tree.
    std::sort(mstEdges.begin(), mstEdges.end(), 
              [](int l, int r){ return l < r; });

    return new Partition(choices, mstCost, mstEdges);
}

// Test for duplicit trees.
void Helper::testDups(const std::vector<Partition>& kts) {
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
void Helper::testTrees(const std::vector<Partition>& ks, const Graph& g) {
    using std::cout;
    // show that all "trees" are actual trees
    cout << "INFO: Testing for cycles...\n";
    int nonTreeCount=0;

    DisjointSet<int> ds(g.VertexCount());
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

void Helper::writeOnlyKth( 
    std::ofstream& output,
    const Graph& graph,
    const std::vector<Partition>& ks
)
{
    int kCost = 0;
    for (const Partition& k : ks)
    {
        if (kCost < k.mstCost) 
        {
            output << "[\n";
            for (const int i : k.mstEdges)
            {
                const Edge& e = graph.Edges()[i];
                output << "{ source: " 
                        << e.nodeX 
                        << ", target: "
                        << e.nodeY 
                        << ", cost: "
                        << e.weight
                        << "},\n";
            }
            output << "],\n";
            kCost = k.mstCost;
        }
    }
}

void Helper::writeAllTrees(
    std::ofstream& output,
    const Graph& graph,
    const std::vector<Partition>& ks
)
{
    for (const Partition& k : ks)
    {
        output << "[\n";
        for (const int i : k.mstEdges)
        {
            const Edge& e = graph.Edges()[i];
            output << "{ source: " 
                    << e.nodeX 
                    << ", target: "
                    << e.nodeY 
                    << ", cost: "
                    << e.weight
                    << "},\n";
        }
        output << "],\n";
    }
}

void Helper::writeToHtml(
    const char* filePath, 
    const char* headPath, 
    const char* tailPath, 
    const int mode,
    const Graph& graph,
    const std::vector<Partition>& ks
)
{
    std::string line;
    std::ofstream output(filePath);

    std::ifstream head(headPath);
    while (std::getline(head, line))
        output << line << "\n";
    head.close();
    
    output << "const vertexCount = " << graph.VertexCount() << ";\n";

    output << "const trees = [\n";

    switch (mode) {
        case 0:
        case 1:
            writeOnlyKth(output, graph, ks);
            break;
        case 2:
            writeAllTrees(output, graph, ks);
            break;
        default: break;
    }

    output << "];\n";

    std::ifstream tail(tailPath);
    while (std::getline(tail, line))
        output << line << "\n";
    tail.close();

    output.close();
}



void Helper::PrintTrees(const std::vector<Partition>& ks, const Graph& graph, int mode)
{
    std::cout << "Found " << ks.size() << " trees, from cost of " 
         << ks.front().mstCost << " to " << ks.back().mstCost << "\n";

    switch (mode) {
        case 1: {
            // print only kth trees (random kth)
            int k = 0;
            int kCost = 0;
            for (size_t i=0; i<ks.size(); ++i) {
                if (kCost < ks[i].mstCost) {
                    std::cout << "[" << k << "][" << i << "]\n";
                    std::cout << ks[i].ToString(graph) << "\n";
                    kCost = ks[i].mstCost;
                    k++;
                }
            }
        } break;
        case 2: {
            // if the third argument is 2, 
            // then print every trees to the console
            for (size_t i=0; i<ks.size(); ++i) {
                std::cout << "[" << i << "]\n";
                std::cout << ks[i].ToString(graph) << "\n";
            }
        } break;
        default: break;
    }
}

