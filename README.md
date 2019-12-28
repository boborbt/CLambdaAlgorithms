# Optimal connectivity

You are given a connected graph with N nodes and N-1 bidirectional edges. Each edge has an integer weight W associated to it. 
Given M query edges, write an algorithm that checks, for each one of the queries, if the total weight of the network can be improved (decreased)
by swapping any edge in the network with the query edge and leaving a network that is still connected. For each query output "YES" if it improves
the network, "No" if it does not.

The input files starts with a line containing the number N of nodes in the network followed by N-1 lines each one containing 3 integers denoting the N-1 edges 
(the first two integers denote the edge end points, the third integer is the weight of the edgee). The file continues with a line containing the number Q of 
edge queries followed by Q lines containing 3 integers each (same format as the graph edges).

Your output should consist of Q lines each one containing a 'YES' or 'NO' depending on whether the corresponding query edge improves the network or not.

You can assume that: 

1 <= N <= 100 000
1 <= Q <= 100 000
nodes are denoted with integers in [1..100 000]
for each edge (u,v,w): u!=v, w in [1..1000 000 000]

## Example:

### Input file:
```
6
1 2 2
1 3 3
3 4 5
3 5 4
2 6 4
4
1 4 4
4 5 6
2 3 8
1 6 3
```

### Expected output:

```
YES
NO
NO
YES
```