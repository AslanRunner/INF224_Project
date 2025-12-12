#ifndef _GRAPH_H_
#define _GRAPH_H_

#define NUM_NODES 34

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int matrix[NUM_NODES][NUM_NODES];
    int degree[NUM_NODES];
    int num_edges;
} Graph;

typedef struct {
    int* node_to_comm; // community (0-33 IDs)
    int num_community;   
} Communities;

void initGraph(Graph* graph);
void addEdge(Graph* graph, int src, int dest);
void loadFromFile(Graph* graph, char* filename);
void printMatrix(Graph* graph); 
void update_community_count(Communities* communs);
int get_comm_size(Communities* communs, int comm);
double CalculateModularity(Graph* graph, int* partition);
void print_communities(int *commties, int max_id, char *title);
void runFastGreedy(Graph *graph);
void runLouvain(Graph *graph);
void runMyAlgorithm(Graph *graph);

#endif
