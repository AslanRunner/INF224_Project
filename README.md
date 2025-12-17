# Community Detection in Graphs

Community detection implementation for [Zachary Karate Club](https://en.wikipedia.org/wiki/Zachary%27s_karate_club) graph using three different algorithms.

**Course**: INF224 Data Structures and Algorithms - Galatasaray University

## Algorithms

### 1. Fast Greedy

Greedy optimization approach that iteratively merges the two communities with the highest modularity gain. Starts with each node as its own community and combines them until no further improvement is possible.

### 2. Louvain

Multi-level community detection algorithm. Phase 1 optimizes modularity by moving nodes between communities. Phase 2 aggregates communities and repeats the process hierarchically until convergence.

### 3. My Own Algorithm

Seed-based approach that selects the two highest-degree nodes as seeds, assigns remaining nodes to the closer seed based on neighborhood proximity, then optimizes modularity through iterative node relocation. Guarantees exactly 2 communities with maximum modularity.

## Compile & Run

```bash
gcc main.c graph.c fastgreedy.c louvain.c original.c -o main
./main
```

## Usage

The program provides an interactive menu with the following options:

1. **Display Adjacency Matrix** - Print the 34x34 adjacency matrix representation of the graph
2. **Fast Greedy Algorithm** - Run greedy modularity optimization 
3. **Louvain Algorithm** - Run multi-level community detection
4. **My Algorithm** - Run seed-based community detection
5. **Run All Algorithms** - Execute all three algorithms sequentially

Select an option from the menu to run the algorithm. Each algorithm outputs the final modularity score (Q), community assignments.

## Dataset

Zachary Karate Club: 34 nodes, 78 edges

The graph represents social relationships in a university karate club that split into two groups. Nodes 1 and 34 represent the club administrator and instructor, around which the two natural communities form.

## Output

Each algorithm produces:

- Modularity score 
- Node assignments to communities

## References

### Academic Papers

- Zachary, W. W. (1977). An information flow model for conflict and fission in small groups. Journal of Anthropological Research, 33(4), 452-473. [PDF](http://www.communicationcache.com/uploads/1/0/8/8/10887248/an_information_flow_model_for_conflict_and_fission_in_small_groups.pdf)

- Clauset, A., Newman, M. E., & Moore, C. (2004). Finding community structure in very large networks. Physical Review E, 70(6), 066111.

- Newman, M. E. (2006). Modularity and community structure in networks. Proceedings of the National Academy of Sciences, 103(23), 8577-8582.

- Blondel, V. D., Guillaume, J. L., Lambiotte, R., & Lefebvre, E. (2008). Fast unfolding of communities in large networks. Journal of Statistical Mechanics: Theory and Experiment, 2008(10), P10008.

### Dataset

- [Zachary's Karate Club - Wikipedia](https://en.wikipedia.org/wiki/Zachary%27s_karate_club)
- [SuiteSparse Matrix Collection - Karate Network](https://www.cise.ufl.edu/research/sparse/matrices/Newman/karate.html)

### Learning Resources

- [Community Detection in Networks - YouTube Lecture](https://www.youtube.com/watch?v=0zuiLBOIcsw)
- [Network Analysis and Community Detection - YouTube Tutorial](https://www.youtube.com/watch?v=Xt0vBtBY2BU)
- [Zachary's Karate Club Visualization](https://studentwork.prattsi.org/infovis/visualization/social-network-in-zacharys-karate-club/)
- [Louvain Method - Wikipedia](https://en.wikipedia.org/wiki/Louvain_method)
