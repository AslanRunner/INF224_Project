#include "graph.h"

double calculate_delta_modularity(Graph *graph, Communities *communs, int node, int new_comm, int num_nodes){
    int old_com = communs->node_to_comm[node];
    double double_m = 2.0 * graph->num_edges;
    int degree_node = graph->degree[node];

    // Eski topluluk
    double sum_to_old = 0.0, degree_old = 0;
    for (int i = 0; i < num_nodes; i++) {
        if (i != node && communs->node_to_comm[i] == old_com) {
            sum_to_old += graph->matrix[node][i];
            degree_old += graph->degree[i];
        }
    }
    double old_Q = sum_to_old - (degree_old * degree_node / double_m);

    // Yeni topluluk
    double sum_to_new = 0.0, degree_new = 0;
    for (int j = 0; j < num_nodes; j++) {
        if (j != node && communs->node_to_comm[j] == new_comm) {
            sum_to_new += graph->matrix[node][j];
            degree_new += graph->degree[j];
        }
    }
    double new_Q = sum_to_new - (degree_new * degree_node / double_m);

    return (new_Q - old_Q) / double_m;
}

void merge_isolated_nodes(Communities *communs){
    int largest_comm = 0, max_size = 0;
    
    for (int c = 0; c < communs->num_community; c++) {
        int size = 0;
        for (int i = 0; i < NUM_NODES; i++)
            if (communs->node_to_comm[i] == c) size++;
        if (size > max_size) {
            max_size = size;
            largest_comm = c;
        }
    }

    for (int i = 0; i < NUM_NODES; i++) {
        int comm_size = 0;
        for (int j = 0; j < NUM_NODES; j++){
            if (communs->node_to_comm[j] == communs->node_to_comm[i]) comm_size++;
        }
        
        if (comm_size == 1 && communs->node_to_comm[i] != largest_comm)
            communs->node_to_comm[i] = largest_comm;
    }
    update_community_count(communs);
}

void move_nodes(Graph *graph, Communities *communs, int num_nodes){
    int improved = 1;
    while (improved) {
        double old_mod = CalculateModularity(graph, communs->node_to_comm);
        improved = 0;

        for (int i = 0; i < num_nodes; i++) {
            int best_comm = communs->node_to_comm[i];
            double max_delta_Q = 0.0;

            for (int j = 0; j < communs->num_community; j++) {
                double delta = calculate_delta_modularity(graph, communs, i, j, num_nodes);
                if (delta > max_delta_Q) {
                    max_delta_Q = delta;
                    best_comm = j;
                }
            }

            if (max_delta_Q > 0) {
                communs->node_to_comm[i] = best_comm;
                improved = 1;
            }
        }

        if (improved) {
            double new_mod = CalculateModularity(graph, communs->node_to_comm);
            if (new_mod <= old_mod) improved = 0;
        }
    }
    merge_isolated_nodes(communs);
}

double calc_modularity_matrix(double matrix[NUM_NODES][NUM_NODES], int *partition, int num_groups, double total_weight){
    double Q = 0;
    for (int i = 0; i < num_groups; i++) {
        for (int j = 0; j < num_groups; j++) {
            if (partition[i] == partition[j]) {
                double ki = 0, kj = 0;
                for(int k = 0; k < num_groups; k++) {
                    ki += matrix[i][k];
                    kj += matrix[j][k];
                }
                Q += matrix[i][j] - (ki * kj / total_weight);
            }
        }
    }
    return Q / total_weight;
}

void runLouvain(Graph* graph)
{
    printf("\n--- Louvain Algorithm---\n\n");

    // PHASE 1
    Communities communs;
    communs.node_to_comm = (int*)malloc(NUM_NODES * sizeof(int));
    for(int i = 0; i < NUM_NODES; i++){
        communs.node_to_comm[i] = i;
    } 
    communs.num_community = NUM_NODES;

    move_nodes(graph, &communs, NUM_NODES);
    printf("Phase 1 Modularity: %.4f\n", CalculateModularity(graph, communs.node_to_comm));
    printf("----------------------------------\n");

    // MAPPING
    int mapping[NUM_NODES];
    memset(mapping, -1, sizeof(mapping));
    int num_groups = 0;
    for(int i = 0; i < NUM_NODES; i++) {
        if(mapping[communs.node_to_comm[i]] == -1)
            mapping[communs.node_to_comm[i]] = num_groups++;
    }
    print_communities(communs.node_to_comm, communs.num_community-1, "COMMUNITIES FOUND:"); // ids (0 - 33) 
    printf("----------------------------------\n");

    // AGGREGATION
    double small_matrix[NUM_NODES][NUM_NODES];
    memset(small_matrix, 0, sizeof(small_matrix));
    double total_weight = 0;

    for(int i = 0; i < NUM_NODES; i++) {
        for(int j = 0; j < NUM_NODES; j++) {
            if(graph->matrix[i][j] > 0) {
                int g1 = mapping[communs.node_to_comm[i]];
                int g2 = mapping[communs.node_to_comm[j]];
                small_matrix[g1][g2] += graph->matrix[i][j];
                total_weight += graph->matrix[i][j];
            }
        }
    }

    // PHASE 2
    int partition2[NUM_NODES];
    for(int i = 0; i < num_groups; i++) partition2[i] = i;

    int merged = 1;
    while(merged) {
        merged = 0;
        double curr_Q = calc_modularity_matrix(small_matrix, partition2, num_groups, total_weight);
        int best_i = -1, best_j = -1;
        double max_Q = curr_Q;

        for(int i = 0; i < num_groups; i++) {
            for(int j = i + 1; j < num_groups; j++) {
                if(partition2[i] == partition2[j]) continue; // pass

                int backup[NUM_NODES];
                memcpy(backup, partition2, sizeof(backup));
                
                int old_comm_j = partition2[j];
                int target_comm = partition2[i];
                for(int k = 0; k < num_groups; k++)
                    if(partition2[k] == old_comm_j) partition2[k] = target_comm;

                double trial_Q = calc_modularity_matrix(small_matrix, partition2, num_groups, total_weight);
                if(trial_Q > max_Q + 0.000001) {
                    max_Q = trial_Q;
                    best_i = i;
                    best_j = j;
                }

                memcpy(partition2, backup, sizeof(backup));
            }
        }

        if(best_i != -1) {
            int old_comm_j = partition2[best_j];
            int target_comm = partition2[best_i];
            for(int k = 0; k < num_groups; k++)
                if(partition2[k] == old_comm_j) partition2[k] = target_comm;
            merged = 1;
        }
    }

    // RESULTS
    double final_Q = calc_modularity_matrix(small_matrix, partition2, num_groups, total_weight);
    printf("\nPhase 2 (Final) Modularity: %.4f\n", final_Q);
    printf("----------------------------------\n");

    int final_commties[NUM_NODES];
    int max_final_id = -1;
    for(int i = 0; i < NUM_NODES; i++) {  // Node -> phase1 -> phase2
        int g1 = mapping[communs.node_to_comm[i]];
        final_commties[i] = partition2[g1];
        if(partition2[g1] > max_final_id) max_final_id = partition2[g1];
    }

    print_communities(final_commties, max_final_id, "FINAL COMMUNITIES:");

    free(communs.node_to_comm);
}