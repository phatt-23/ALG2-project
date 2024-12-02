#include "helper.h"
#include "partition.h"
#include "graph.h"
#include "disjoint_set.h"
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

#define USE_STL_PRIORITY_QUEUE
#ifdef USE_STL_PRIORITY_QUEUE

#include <queue>
std::vector<Partition>
Helper::solve(const Graph& graph) 
{
    std::vector<Partition> ks;
    DisjointSet ds(graph.VertexCount());

    auto partCmp = [](const Partition* l, const Partition* r) { return l->Less(*r); };
    auto q = std::priority_queue<Partition*, std::vector<Partition*>, decltype(partCmp)>(partCmp);
    
    std::vector<int> initChoices(graph.EdgeCount());
    fill(initChoices.begin(), initChoices.end(), 0);

    Partition* mst = createPartition(initChoices, graph, ds);
    ks.push_back(*mst);
    q.push(mst); 

    while (!q.empty()) 
    {
        Partition* p = q.top();
        q.pop();
        
        for (size_t x=0; x<graph.VertexCount()-1; x++) 
        {
            if (p->choices[p->mstEdges[x]] == 0)
            {
                std::vector<int> choices(p->choices);

                choices[p->mstEdges[x]] = 3;
                
                for (size_t y=0; y<x; y++)
                    choices[p->mstEdges[y]] = 1;

                Partition* nxt = createPartition(choices, graph, ds);
                if (nxt == nullptr)
                    continue;

                q.push(nxt); 
                ks.push_back(*nxt);
            }
        }

        delete p;
    }

    auto cmp = [](const Partition& l, const Partition& r) { return l.Less(r); };
    sort(ks.begin(), ks.end(), cmp);

    return ks;
}

#else //BENCHMARK_MY_PRIORITY_QUEUE
#include "binary_heap.h"

std::vector<Partition>
Helper::solve(const Graph& g) 
{
    std::vector<Partition> ks;
    DisjointSet ds(g.VertexCount());

    auto partCmp = [](const Partition* l, const Partition* r) { return l->Less(*r); };
    auto q = BinaryHeap<Partition*, decltype(partCmp)>(partCmp);
    
    std::vector<int> initChoices(g.EdgeCount());
    fill(initChoices.begin(), initChoices.end(), 0);

    Partition* mst = createPartition(initChoices, g, ds);
    ks.push_back(*mst);
    q.Insert(mst); 

    while (!q.Empty()) 
    {
        Partition* p = q.Poll();
        
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

                q.Insert(nxt); 
                ks.push_back(*nxt);
            }
        }

        delete p;
    }

    auto cmp = [](const Partition& l, const Partition& r) { return l.Less(r); };
    sort(ks.begin(), ks.end(), cmp);

    return ks;
}

#endif //BENCHMARK_MY_PRIORITY_QUEUE

/// Finds the MST of the given search space.
/// If no tree is possible to construct given 
/// the search space, nullptr is returned.
/// Using Kruskal's algorithm.
Partition* 
Helper::createPartition(const std::vector<int>& choices, const Graph& graph, DisjointSet& ds) 
{
    ds.Reset(); // resets the disjoint set, using the same memory again
    
    int mstCost = 0;
    std::vector<int> mstEdges(graph.VertexCount() - 1);
    std::fill(mstEdges.begin(), mstEdges.end(), -1);

    int eIdx = 0;
    for (size_t x=0; x<graph.EdgeCount(); x++)
    {
        if (choices[x] == 1)
        {
            Edge e = graph.Edges()[x];
            ds.Unify(e.nodeX, e.nodeY);
            mstEdges[eIdx++] = x;
            mstCost += e.weight;
        }
    }

    for (size_t x=0; x<graph.EdgeCount(); x++) 
    {
        if (ds.numberOfComponents == 1)
            break;

        if (choices[x] == 0)
        {
            Edge e = graph.Edges()[x];
            if (!ds.NodesConnected(e.nodeX, e.nodeY))
            {
                ds.Unify(e.nodeX, e.nodeY);
                mstEdges[eIdx++] = x;
                mstCost += e.weight;
            }
        }
    }

    if (ds.numberOfComponents > 1) {
        return nullptr;
    }

    std::sort(mstEdges.begin(), mstEdges.end(), [](int l, int r){ return l < r; });

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

