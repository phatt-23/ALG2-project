#include "disjoint_set.h"
#include "partition.h"
#include "edge.h"
#include "graph.h"
#include "helper.h"

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <vector>
#include <queue>

using namespace std;

Partition* createPartition(std::vector<int> choices, Graph g) 
{
    DisjointSet ds(g.vertexCount);
    
    int mstCost = 0;
    vector<int> mstEdges(g.vertexCount - 1);
    std::fill(mstEdges.begin(), mstEdges.end(), -1);

    int ptr = 0;
    for (int x=0; x<g.edgeCount; x++)
    {
        if (choices[x] == 1)
        {
            Edge e = g.edges[x];
            ds.unify(e.nodeX, e.nodeY);
            mstEdges[ptr++] = x;
            mstCost += e.weight;
        }
    }

    for (int x=0; x<g.edgeCount; x++) 
    {
        if (ds.numberOfComponents == 1)
            break;

        if (choices[x] == 0)
        {
            Edge e = g.edges[x];
            if (ds.find(e.nodeX) != ds.find(e.nodeY))
            {
                ds.unify(e.nodeX, e.nodeY);
                mstEdges[ptr++] = x;
                mstCost += e.weight;
            }
        }
    }

    if (ds.numberOfComponents > 1)
        return nullptr;

    std::sort(mstEdges.begin(), mstEdges.end(), [](int l, int r){ return l < r; });

    return new Partition(choices, mstCost, mstEdges);
}

vector<Partition> solve(Graph g) 
{
    vector<Partition> ks;

    auto partCmp = [](const Partition* l, const Partition* r){ return l->less(*r); };
    auto q = priority_queue<Partition*, vector<Partition*>, decltype(partCmp)>(partCmp);
    
    vector<int> init_choices(g.edgeCount);
    fill(init_choices.begin(), init_choices.end(), 0);

    Partition* mst = createPartition(init_choices, g);
    ks.push_back(*mst);
    q.push(mst); 

    while (!q.empty()) 
    {
        Partition* p = q.top();
        q.pop();
        
        for (int x=0; x<g.vertexCount-1; x++) 
        {
            if (p->choices[p->mstEdges[x]] == 0)
            {
                std::vector<int> choices(p->choices);

                choices[p->mstEdges[x]] = -1;
                
                for (int y=0; y<x; y++)
                    choices[p->mstEdges[y]] = 1;

                Partition* nxt = createPartition(choices, g);
                if (nxt == nullptr)
                    continue;

                q.push(nxt); 
                ks.push_back(*nxt);
            }
        }
    }

    auto cmp = [](Partition l, Partition r) { return l.less(r); };
    sort(ks.begin(), ks.end(), cmp);

    return ks;
}

int main(int argc, char** argv) 
{
    if (argc != 3) {
        std::cout << "Usage:\n";
        std::cout << "    kthmst <input_file> <print_bool>\n"; 
        std::cout << "\n";
        std::cout << "        input_file        adjacent matrix\n"; 
        std::cout << "        print_bool        print all trees\n"; 
        std::cout << "\n";
        return -1;
    }

    std::ifstream input(argv[1]); // read in the adjacencyMartix

    size_t numberOfVertices = 0;
    input >> numberOfVertices;
    int* adjMat = readInAdjMatrix(input, numberOfVertices);
    std::vector<Edge> edges = createEdges(adjMat, numberOfVertices);

    Graph g(numberOfVertices, edges); // graph with useful data

    vector<Partition> ks = solve(g);

    if (atoi(argv[2]) == 1) {
        for (Partition k : ks) {
            k.print(g);
        }
    }

    int dupCount=0;
    for (int i=0; i<ks.size(); i++) {
        for (int j=i+1; j<ks.size(); j++) {
            if (ks[i].mstEdges == ks[j].mstEdges)
                std::cout << ++dupCount << " Found duplicate\n";
        }
    }

    return 0;
}

