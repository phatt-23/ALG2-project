# Algorithm Intuition

For a tree a N vertices there's possible N^(N-2) possible trees (Cayley's formula).
That is the entirity of our search space.

We find the MST by Kruskal's, Prim's whathever algorithm.

We still have got N^(N-2) - 1 possible trees to check. Instead we will fracture
the search space into disjoint pieces that somehow don't contain MST. Then we 
find the MSTs for each of the disjoint spaces, take the union on all of them
and finally taking the smallest spanning tree out of them, effectively we choose
the space that contained the current smallest ST. That is our second smallest ST.
We throw away all the other disjoint spaces except for out space that produced
the second smallest ST and fracture this space into more disjoint spaces not
containing either the 2nd smallest nor the 1nd smallest STs. 


But how do we do that? How can we characterize the subspaces we need to search?
How to we break a search space into disjoint regions? If we have an overlap between
these regions this will lead to discovering the same ST multiple times.


We can think of our graph G as list of m edges E.
```
E = [e_1, e_2, e_3, e_4, e_5, ..., e_m]
```
We can label each of these to say that we can take it, we must definitely take
it and we must definitely not take it to form the ST.

To do that we can think of the list E as a list of integer types. Zero (0) will 
denote the edge is free to take, it's up to us to include it to the ST.
One (1) denotes the edge will definitely belong to the final ST. Minus one (-1) 
denotes the edge will definitely not belong to the final ST.

| Label | Meaning |
| -------------- | --------------- |
| 0 | free |
| 1 | definitely include |
| -1 | definitely exclude |

Every edge, that is not labeled 0, say $E \exclude E_0$ or $E_1 \union E_{-1}$, 
is fixed all the 0 edges, say $E_0$, are free. 
This creates a forest - a structure with tree components.
Edges $E_0$ are the possible links for them to create forest 
of only of component - a tree.

