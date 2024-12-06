#include "SpanningTreesFinder.h"
#include "Partition.h"
#include "Graph.h"
#include "DisjointSet.h"
#include "BinaryHeap.h"
#include "Matrix.h"

#include <cassert>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>


Matrix<int> 
SpanningTreesFinder::ReadAdjacencyMatrix(std::ifstream& inputStream)
{
    size_t numOfVert = 0;
    inputStream >> numOfVert;

    // Read in the matrix
    const auto elems = new int[numOfVert * numOfVert];
    for (size_t i = 0; i < numOfVert * numOfVert; ++i)
        inputStream >> elems[i];
    
    auto adjMat = Matrix<int>(numOfVert, numOfVert, elems);

    return adjMat;
}

Vector<Edge>
SpanningTreesFinder::CreateEdges(const Matrix<int>& adjMat)
{
    assert(adjMat.Rows() == adjMat.Columns() && "Must be square matrix");

    Vector<Edge> edges;
    const size_t numOfVert = adjMat.Rows();
    
    // Create edges from adjMat
    for (size_t row = 0; row < numOfVert; ++row) {
        for (size_t col = row + 1; col < numOfVert; ++col) {
            const int weight = adjMat.Get(row, col);
            if (weight == 0) continue;
            edges.EmplaceBack(row, col, weight);
        }
    }

    // Edges are sorted by their cost. They will stay sorted throughout the algorithm
    std::ranges::sort(edges,
        [](const Edge& l, const Edge& r){ return l.Less(r); });
 
    return edges;
}

/// Finds all the spanning trees and puts
/// them into an array ordered ascendingly 
/// by their cost.
Vector<Partition>
SpanningTreesFinder::Solve(const Graph& g)
{
    // Storage for the MSTs.
    Vector<Partition> spanningTrees;

    // Shared among all iterations, stores found vertices.
    // Used in the createPartition function for Kruskal's.
    DisjointSet<int> disjointSet(g.VertexCount());

    // Ordinary binary heap to store the partitions
    // (search spaces - holds info about the spanning tree)
    // in a way, so that its always ready to serve the partition
    // with the least mstCost.
    auto partCmp = [](const Partition* l, const Partition* r) {
        return l->Less(*r);
    };

    auto partitions = BinaryHeap<Partition*, decltype(partCmp)>(partCmp);

    // Initial state is choice where all the edges all not assessed.
    const std::vector<int> initChoices(g.EdgeCount(), Partition::NOT_ASSESSED);

    // Find the actual MST, put it in the storage of STs
    Partition* mst = CreatePartition(initChoices, g, disjointSet);

    // Throws if the graph is not connected -> no spanning tree is possible
    if (mst == nullptr)
        throw std::runtime_error("The graph is not connected. Spanning tree not possible.");

    spanningTrees.PushBack(*mst);
    partitions.Insert(mst);

   	size_t spanningTreeCount = 1;

    // while all the search spaces still weren't
    // searched through, continue searching
    while (!partitions.Empty())
    {
        // Search this partition's search space
        const Partition* part = partitions.Poll();

        // Make a new choice describing the search space
        // and see if a spanning tree is possible in this space
        // If yes, add it to the mstList
        for (size_t x = 0; x < g.VertexCount() - 1; x++)
        {
            // Matches the first still not assessed choice
            // and evaluates this space
            if (part->choices[part->mstEdges[x]] == Partition::EdgeChoice::NOT_ASSESSED)
            {
                // copy the choices of the previous iteration
                std::vector<int> choices(part->choices);

                // Mark current as excluded and try a tree is possible
                choices[part->mstEdges[x]] = Partition::EdgeChoice::EXCLUDED;

                // Mark all the previous choices that had already been included
                for (size_t y = 0; y < x; y++)
                    choices[part->mstEdges[y]] = Partition::EdgeChoice::INCLUDED;

                // Try finding a spanning tree for this search space
                Partition* nxt = CreatePartition(choices, g, disjointSet);

                // If the nxt pointer is NULL then no spanning tree was found
                if (nxt == nullptr)
                    continue;

                // Otherwise insert the newly found spanning tree into the heap
                // and add it to the list of valid spanning trees
                partitions.Insert(nxt);
                spanningTrees.PushBack(*nxt);

                std::cout << spanningTreeCount++ << " " << partitions.Size() << "\n";
            }
        }

        // The partition is not needed anymore as this search space was already searched through
        delete part;
    }

    // sort them by mstWeight and return the list (vector)
    std::ranges::sort(spanningTrees,
        [](const Partition& l, const Partition& r){ return l.mstCost < r.mstCost; });

    return spanningTrees;
}



/// Finds the MST of the given search space.
/// If no tree is possible to construct given 
/// the search space, nullptr is returned.
/// Is using Kruskal's algorithm.
Partition* 
SpanningTreesFinder::CreatePartition(const std::vector<int>& choices, const Graph& g, DisjointSet<int>& ds)
{
    ds.Reset(); // Resets the disjoint set, reusing the same memory again.
    
    int mstCost = 0;

    // Holds indices of the mst's edges. for starters, put in invalid values.
    std::vector<int> mstEdges(g.VertexCount() - 1, -999);

    // Marks how many edges have already been added to the mstEdges vector
    int eIdx = 0;
    
    // Add all the edges that are set to be included.
    for (size_t i = 0; i < g.EdgeCount(); i++)
    {
        // If not yet found, add it, vertices won't be dupped thanks to the DisjointSet
        if (choices[i] == Partition::EdgeChoice::INCLUDED) 
        {
            const Edge e = g.Edges()[i];
            ds.Unify(e.nodeX, e.nodeY);
            mstEdges[eIdx++] = i;
            mstCost += e.weight;
        }
    }

    for (size_t i = 0; i < g.EdgeCount(); i++)
    {
        // If the graph is already connected then no additional edge is needed, break out
        if (ds.numberOfComponents == 1) 
            break;

        // Try adding edges still not included
        // hopefully they will compose a spanning tree.
        if (choices[i] == Partition::EdgeChoice::NOT_ASSESSED)            
        {
            if (const Edge e = g.Edges()[i]; !ds.NodesConnected(e.nodeX, e.nodeY))
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
    std::ranges::sort(mstEdges,
        [](const int l, const int r){ return l < r; });

    // Return the valid partition with the mst's edge indices and a total cost
    return new Partition(choices, mstCost, mstEdges);
}

// Test for duplicate trees.
void SpanningTreesFinder::TestDuplicates(const Vector<Partition>& kts) {
    using std::cout;
    Vector<Partition> ks = kts;
    cout << "INFO: Testing for duplicities...\n";

    // presort
    std::ranges::sort(ks,
        [](const Partition& l, const Partition& r) {
            return l.choices == r.choices; // choices should all be unique
        }
    );

    int dupCount = 0;

    // show that every tree is unique
    for (size_t i = 0; i < ks.Size() - 1; i++) {
        if (ks[i].choices == ks[i+1].choices) {
            cout << "Found-duplicate (" << ++dupCount << ")\n";
            cout << ks[i] << "\n" << ks[i+1] << "\n";
        }
    }

    if (dupCount == 0) {
        cout << "DONE: Found none\n";
        return;
    }
    
    cout << "DONE: Found " << dupCount << " dups\n";
}

/// Test that graphs in ks are all trees, 
/// meaning they have no cycles.
void SpanningTreesFinder::TestCycles(const Vector<Partition>& ks, const Graph& g) {
    using std::cout;

    // show that all "trees" are actual trees
    cout << "INFO: Testing for cycles...\n";
    int nonTreeCount=0;

    DisjointSet<int> ds(g.VertexCount());

    for (const Partition& k : ks)
    {
        // each edge addition should yield a new reachable vertex
        // if its already included, then by adding this edge a cycle is intorduced
        ds.Reset();
        for (const int ke : k.mstEdges)
        {
            const Edge e = g.Edges()[ke];
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

// helper function for the writeToHTML
void writeOnlyKth(
    std::ofstream& output,
    const Graph& graph,
    const Vector<Partition>& ks
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

// helper function for the writeToHTML
void writeAllTrees(
    std::ofstream& output,
    const Graph& graph,
    const Vector<Partition>& ks
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

void SpanningTreesFinder::WriteToHtml(
    const char* outputPath,
    const char* headPath, 
    const char* tailPath, 
    const int mode,
    const Graph& g,
    const Vector<Partition>& ks
)
{
    std::string line;
    std::ofstream output(outputPath);

	output << "<!DOCTYPE html>\n";
	output << "<html lang=\"en\">\n";

    std::ifstream head(headPath);
    while (std::getline(head, line))
        output << line << "\n";
    head.close();

	output << "<body></body>\n";
    output << "<script>\n";
    output << "const vertexCount = " << g.VertexCount() << ";\n";
    output << "const trees = [\n";

    switch (mode) {
        case 0:
        case 1:
            writeOnlyKth(output, g, ks);
            break;
        case 2:
            writeAllTrees(output, g, ks);
            break;
        default: break;
    }

    output << "];\n";
    output << "</script>\n";

    std::ifstream tail(tailPath);
    while (std::getline(tail, line))
        output << line << "\n";
    tail.close();

   	output << "</html>\n";


    output.close();
}


void SpanningTreesFinder::PrintTrees(const Vector<Partition>& ks, const Graph& graph, const int mode)
{
    std::cout << "Found " << ks.Size() << " trees, from cost of "
         << ks.Front().mstCost << " to " << ks.Back().mstCost << "\n";

    switch (mode) {
        case 1: {
            // print only kth trees (random kth)
            int k = 0;
            int kCost = 0;
            for (size_t i = 0; i < ks.Size(); ++i) {
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
            for (size_t i = 0; i < ks.Size(); ++i) {
                std::cout << "[" << i << "]\n";
                std::cout << ks[i].ToString(graph) << "\n";
            }
        } break;
        default: break;
    }
}

