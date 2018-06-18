
#include <stdio.h>
#include "status.h"
#include "MGraph.h"
#include "ShortestPath.h"

int main(int argc, char const *argv[]){
    //Operation;
    MGraph G;
    PathMatrix P1[MAX_VERTAX_NUM + 1][MAX_VERTAX_NUM + 1];
    ShortPathTable D1[MAX_VERTAX_NUM + 1];
    PathMatrix P2[MAX_VERTAX_NUM + 1][MAX_VERTAX_NUM + 1][MAX_VERTAX_NUM + 1];
    ShortPathTable D2[MAX_VERTAX_NUM + 1][MAX_VERTAX_NUM + 1];
    CreateDN(&G);   printf("\n");

    ShortestPath_DIJ(G,1,P1,D1);
    printf("Shortest path of No.1 vertex to others(Dijkstra algorithm):\n");
    PrintPath_1(G,1,P1,D1); printf("\n");

    ShortestPath_DIJK(G,P2,D2);
    printf("All shortest pathes between vertexes(Dijkstra algorithm):\n");
    PrintPath_2(G,P2,D2);

    ShortestPath_FLOYD(G,P2,D2);
    printf("All shortest pathes between vertexes(Floyd algorithm):\n");
    PrintPath_2(G,P2,D2);
    return 0;
}//ShortestPath_test(Dijkstra and Floyd algorithm)
