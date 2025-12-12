#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

void initGraph(Graph* graph){ 
    graph->num_edges = 0;
    for(int i = 0; i < NUM_NODES; i++){
        graph->degree[i] = 0; 
        for(int j = 0; j < NUM_NODES; j++){
            graph->matrix[i][j] = 0;
        }
    }
}

void addEdge(Graph* graph, int src, int dest){ // undirected
    src = src - 1; 
    dest = dest - 1; 

    if(src < 0 || src >= NUM_NODES){ 
        printf("ERROR(addEdge)\n");
        return;
    }

    if(dest < 0 || dest >= NUM_NODES){
        printf("ERROR (addEdge)\n");
        return;
    }
    // add edge
    if(graph->matrix[src][dest] == 0){
        graph->matrix[src][dest] = 1; 
        graph->matrix[dest][src] = 1; // simetric
        graph->num_edges++;
        graph->degree[src]++;
        graph->degree[dest]++;
    }
}

void loadFromFile(Graph* graph, char* filename){
    FILE *file = fopen(filename, "r");
    if(file == NULL){
        printf("ERROR (loadFromFile)\n");
        exit(1);
    }

    int src, dest;

    while(fscanf(file, "%d %d", &src, &dest) != EOF){
        addEdge(graph, src, dest);
    }
    
    fclose(file);
    printf("Data loading successfully completed. \n");
}

void printMatrix(Graph* graph){
    printf("\n   ");
    for(int i = 0; i < NUM_NODES; i++){
        printf("%2d ", i + 1);
    }
    printf("\n");

    for(int j = 0; j < NUM_NODES; j++){
        printf("%2d ", j + 1);
        for(int k = 0; k < NUM_NODES; k++){
            printf("%2d ", graph->matrix[j][k]); // 0 or 1
        }
        printf("\n");
    }

    printf("\nTotal number of edges : %d",graph->num_edges);
    printf("\n");

}

void update_community_count(Communities* communs){
    int max_comm = -1;

    for(int i = 0; i < NUM_NODES; i++){
        if(communs->node_to_comm[i] > max_comm) max_comm = communs->node_to_comm[i];        
    }

    communs->num_community = max_comm + 1;
}

int get_comm_size(Communities* communs, int comm){
    int count = 0;
    for(int i = 0; i < communs->num_community; i++){
        if(communs->node_to_comm[i] == comm) count++;        
    }
    return count;
}

void print_communities(int *commties, int max_id, char *title){
    printf("%s\n", title);
    int counter = 1;
    
    for(int c = 0; c <= max_id; c++) {
        int exists = 0;
        for(int i = 0; i < NUM_NODES; i++)
            if(commties[i] == c) { 
                exists = 1; 
                break; 
            }
        if(exists) {
            printf("Community %d: ", counter++);
            for(int i = 0; i < NUM_NODES; i++){
                if(commties[i] == c) printf("%d ", i+1);
            }
            printf("\n");
        }
    }
}

double CalculateModularity(Graph* graph, int* partition){
    double Q = 0.0;
    double m_double = (double)(graph->num_edges * 2);

    for(int i = 0; i < NUM_NODES; i++){
        for(int j = 0; j < NUM_NODES; j++){
            if(partition[i] == partition[j]){ // control 
                double A_ij =  (double)graph->matrix[i][j]; // 1 or 0 
                double k_i = (double)graph->degree[i]; // degree
                double k_j = (double)graph->degree[j];

                Q += (A_ij - ((k_i * k_j) / m_double)); // calculation
            }
        }
    }

    return Q / m_double; 
}
