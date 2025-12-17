#include "graph.h"

int merge_to_closer_seed(Graph *graph, int node, int seed1, int seed2, int num_nodes) {
    double dist1 = 0.0, dist2 = 0.0;
    
    for (int i = 0; i < num_nodes; i++) {
        if (i != node && i != seed1 && graph->matrix[node][i] == 1) {
            dist1 += graph->matrix[i][seed1];
        }
        if (i != node && i != seed2 && graph->matrix[node][i] == 1) {
            dist2 += graph->matrix[i][seed2];
        }
    }
    
    if (graph->matrix[node][seed1] == 1) dist1 += 2.0;
    if (graph->matrix[node][seed2] == 1) dist2 += 2.0;
    
    if (dist1 > dist2) return 0;
    if (dist2 > dist1) return 1;
    return (graph->degree[seed1] > graph->degree[seed2]) ? 0 : 1;
}

// get the two highest degree nodes from graph
void find_top_degree_nodes(Graph *graph, int *seed1, int *seed2) {
    int max1 = -1, max2 = -1;
    *seed1 = 0, *seed2 = 0;
    
    for (int i = 0; i < NUM_NODES; i++) {
        if (graph->degree[i] > max1) {
            max1 = graph->degree[i];
            *seed1 = i;
        }
    }
    
    for (int i = 0; i < NUM_NODES; i++) {
        if (i != *seed1 && graph->degree[i] > max2) {
            max2 = graph->degree[i];
            *seed2 = i;
        }
    }
}

// Statistics
void print_stats(Graph *graph, int* partition) {
    int internal_0 = 0, internal_1 = 0, cross_edges = 0;
    
    for (int i = 0; i < NUM_NODES; i++) {
        for (int j = i + 1; j < NUM_NODES; j++) {
            if (graph->matrix[i][j] == 1) {
                if (partition[i] == 0 && partition[j] == 0) internal_0++;
                else if (partition[i] == 1 && partition[j] == 1) internal_1++;
                else cross_edges++;
            }
        }
    }
    
    printf("Internal edges (Comm 1): %d\n", internal_0);
    printf("Internal edges (Comm 2): %d\n", internal_1);
    printf("Cross-community edges: %d\n", cross_edges);
}

void runMyAlgorithm(Graph *graph) {
    printf("\n--- My Algorithm ---\n\n");
    
    int seed1, seed2;
    find_top_degree_nodes(graph, &seed1, &seed2);
    
    int* partition = (int*)malloc(NUM_NODES * sizeof(int));
    
    printf("PHASE 1: Seed Selection\n");
    printf("Seed 1: Node %d (degree: %d)\n", seed1 + 1, graph->degree[seed1]);
    printf("Seed 2: Node %d (degree: %d)\n", seed2 + 1, graph->degree[seed2]);
    printf("----------------------------------\n");
    
    for (int i = 0; i < NUM_NODES; i++) {
        if (i == seed1) partition[i] = 0;
        else if (i == seed2) partition[i] = 1;
        else partition[i] = merge_to_closer_seed(graph, i, seed1, seed2, NUM_NODES);
    }
    
    printf("Initial Q: %.4f\n\n", CalculateModularity(graph, partition));
    printf("PHASE 2: Modularity Optimization\n");
    
    double improved = 1.0, iteration = 0;
    while (improved > 0.00001 && iteration < 100) {
        improved = 0.0;
        iteration++;
        double old_Q = CalculateModularity(graph, partition);
        
        for (int i = 0; i < NUM_NODES; i++) {
            if (i == seed1 || i == seed2) continue;
            
            int current_comm = partition[i];
            double best_Q = old_Q;
            int best_comm = current_comm;
            
            for (int c = 0; c < 2; c++) {
                if (c != current_comm) {
                    partition[i] = c;
                    double trial_Q = CalculateModularity(graph, partition);
                    
                    if (trial_Q > best_Q) {
                        best_Q = trial_Q;
                        best_comm = c;
                    }
                    
                    partition[i] = current_comm;
                }
            }
            
            if (best_Q > old_Q) {
                partition[i] = best_comm;
                improved = 1;
            }
        }
        
        if (improved) {
            double new_Q = CalculateModularity(graph, partition);
            if (new_Q > old_Q) {
                printf("  Iteration %.0lf: Q = %.4f\n", iteration, new_Q);
            } else {
                improved = 0;
            }
        }
    }
    
    double final_Q = CalculateModularity(graph, partition);
    printf("\n----------------------------------\n");
    printf("Final Modularity: %.4f\n", final_Q);
    printf("----------------------------------\n");
    
    print_communities(partition, 1, "FINAL COMMUNITIES:");
    printf("\n--- STATISTICS ---\n");
    print_stats(graph, partition);
    
    free(partition);
}