
#include <stdio.h>
#include "MGraph.h"
#include "MiniSpanTree.h"

int main(int argc, char const *argv[]){
    //Operation;
    MGraph G;
    CreateUDN(&G);
    printf("Test of Prim algorithme:\n");
    MiniSpanTree_Prim(G,G.vexs[1]);
    printf("Test of Kruskal algorithme:\n");
    MiniSpanTree_Kruskal(G);
    return 0;
}//Minimum Cost Spanning Tree(Prim and Kruskal algorithme)
