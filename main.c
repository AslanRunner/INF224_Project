#include "graph.h"

int main() {

  Graph karateGraph;
  initGraph(&karateGraph);
  loadFromFile(&karateGraph, "member.txt");
  printMatrix(&karateGraph);

  int choice;
  printf("\nWhich algorithm do you want to choose ? : \n\n");
  printf("1. FastGreedy \n2. Louvain \n3. My Algorithm\n\nSelection: ");
  scanf("%d", &choice);

  switch (choice) {
    case 1:
      runFastGreedy(&karateGraph);
      break;
    case 2:
      runLouvain(&karateGraph);
      break;
    case 3:
      runMyAlgorithm(&karateGraph);
      break;
    default:
      printf("Gecersiz secim.\n");
  }

  return 0;
}