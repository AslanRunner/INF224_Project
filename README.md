# Community Detection in Graphs

Community detection implementation for [Zachary Karate Club](https://en.wikipedia.org/wiki/Zachary%27s_karate_club) graph using three different algorithms.

**Course**: INF224 Data Structures and Algorithms - Galatasaray University

## Algorithms

1. **Louvain Method** - Multi-level modularity optimization
2. **Fast Greedy** - Greedy community merging 
3. **Hybrid Algorithm** - Custom seed-based approach

## Compile & Run

```bash
gcc main.c graph.c fastgreedy.c louvain.c original.c -o main
./main
```

## Usage

```
1. FastGreedy
2. Louvain
3. Hybrid Algorithm
4. Run All & Compare
```

Select an option from the menu to run the algorithm.

## Dataset

Zachary Karate Club: 34 nodes, 78 edges

## Output

- Community assignments
- Modularity score (Q)
- Performance metrics (ms)
