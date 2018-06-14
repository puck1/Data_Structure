
#include <stdio.h>
#include "ALGraph.h"
#include "TopologicalSort.h"

int main(int argc, char const *argv[]){
    //Operation;
    ALGraph G;
    CreateDG(&G);
    printf("Topological order of direcred graph G:");
    TopologicalSort(G);
    printf("Topological sort to G by DFS:");
    TopologicalSort_DFS(G);
    return 0;
}//TopologicalSort_test
