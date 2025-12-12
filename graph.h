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

// Topluluk bilgisini tutmak için basit yapı
typedef struct {
    int* node_to_comm;      // Her düğümün hangi toplulukta olduğu (0-33 arası ID)
    int num_community;    // Şu an kaç topluluk var?
} Communities;

// Ilk halini baslat (her sey 0 degerli)
void initGraph(Graph* graph);

// Baglanti Ekleme Fonksiyonu (Undirected - Yonsuz)
void addEdge(Graph* graph, int src, int dest);

// Dosyadan baglantilari oku (78 baglanti olmasi lazim)
void loadFromFile(Graph* graph, char* filename);

// Matrisi Ekrana Yazdir (Kontrol icin)
void printMatrix(Graph* graph); 

// Topluluk sayisini guncellemek icin
void update_community_count(Communities* communs);

// Toplulugun boyutunu ogrenmek icin
int get_comm_size(Communities* communs, int comm);

// Final modularity'i hesapla
double CalculateModularity(Graph* graph, int* partition);

// Topluluklari bastirmak icin
void print_communities(int *commties, int max_id, char *title);

void runFastGreedy(Graph *graph);

void runLouvain(Graph *graph);

void runMyAlgorithm(Graph *graph);


#endif