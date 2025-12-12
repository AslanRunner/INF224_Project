#include "graph.h"

double calculateDeltaQ_FastGreedy(Graph* graph, Communities* communs, int c1, int c2) {
    double m_doubled = (double)(graph->num_edges * 2); 
    int links = 0, k_c1 = 0, k_c2 = 0;

    for (int i = 0; i < NUM_NODES; i++) {
        if (communs->node_to_comm[i] == c1) {
            k_c1 += graph->degree[i];
            for (int j = 0; j < NUM_NODES; j++) {
                if (communs->node_to_comm[j] == c2 && graph->matrix[i][j] == 1)
                    links++;
            }
        }
        else if (communs->node_to_comm[i] == c2)
            k_c2 += graph->degree[i];
    }

    double e_ij = links / m_doubled;
    double a_i = k_c1 / m_doubled;
    double a_j = k_c2 / m_doubled;

    return 2.0 * (e_ij - (a_i * a_j));
}

void runFastGreedy(Graph *graph) {
    printf("\n--- Fast Greedy Algorithm ---\n\n");

    if (graph->num_edges == 0) return;

    Communities communs;
    communs.node_to_comm = (int*)malloc(NUM_NODES * sizeof(int));
    communs.num_community = NUM_NODES;

    for (int i = 0; i < NUM_NODES; i++) communs.node_to_comm[i] = i;

    // main loop
    while (1) {
        double max_delta_Q = 0.0;
        int best_c1 = -1, best_c2 = -1;

        // check actives communities
        for (int i = 0; i < NUM_NODES; i++) {
            for (int j = i + 1; j < NUM_NODES; j++) {
                int i_found = 0, j_found = 0;
                for(int n = 0; n < NUM_NODES && (!i_found || !j_found); n++) {
                    if(communs.node_to_comm[n] == i) i_found = 1;
                    if(communs.node_to_comm[n] == j) j_found = 1;
                }

                if (i_found && j_found) {
                    double delta_Q = calculateDeltaQ_FastGreedy(graph, &communs, i, j);
                    if (delta_Q > max_delta_Q) {
                        max_delta_Q = delta_Q;
                        best_c1 = i;
                        best_c2 = j;
                    }
                }
            }
        }

        // check improved
        if (max_delta_Q > 0.0001) {
            for (int k = 0; k < NUM_NODES; k++)
                if (communs.node_to_comm[k] == best_c2)
                    communs.node_to_comm[k] = best_c1;
            communs.num_community--;
        } else {
            break;
        }
    }

    double final_Q = CalculateModularity(graph, communs.node_to_comm);
    printf("Final Modularity: %.4f\n", final_Q);
    printf("----------------------------------\n");

    // print
    int final_membership[NUM_NODES];
    int max_id = -1;
    for(int i = 0; i < NUM_NODES; i++) {
        final_membership[i] = communs.node_to_comm[i];
        if(communs.node_to_comm[i] > max_id)
            max_id = communs.node_to_comm[i];
    }
    
    print_communities(final_membership, max_id, "COMMUNITIES FOUND:");
    free(communs.node_to_comm);
}
