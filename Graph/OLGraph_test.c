
#include <stdio.h>
#include "OLGraph.h"

int main(int argc, char const *argv[]){
    //Operation;
    OLGraph G;
    printf("Create graph G:\n");
    CreateDG(&G);
    printf("The first vertex is '%c',second is '%c',thrid is '%c,4th is '%c'.\n",
                    GetVex(G,1),GetVex(G,2),GetVex(G,3),GetVex(G,4));
    printf("Change vertex 'A' into '@'.\n"); PutVex(&G,'A','@');
    printf("The first adjacency vertex (as HEAD of arc) of '@' is '%c',its next adjacency vertex of 'B' is '%c'.\n",
                    FirstAdjVex(G,'@'),NextAdjVex(G,'@','B'));
    printf("Insert vertex '#' and '!' into graph...");
    if(InsertVex(&G,'#') && InsertVex(&G,'!')) printf("Succeeded!"); printf("\n");
    printf("InsertArc <@,!> and <@,#> and <!,#> into graph...");

    // if(InsertArc(&G,'@','!',1,"Hello world!") && InsertArc(&G,'@','#',1,"Good to see u!")
    //                                         && InsertArc(&G,'!','#',1,"Good Night!"))
    //     printf("Succeeded!"); printf("\n");

    if(InsertArc(&G,'@','!',0) && InsertArc(&G,'@','#',0) && InsertArc(&G,'!','#',0))
    printf("Succeeded!"); printf("\n");

    printf("Delete arc <@,!>...");
    if(DeleteArc(&G,'@','!')) printf("Succeeded!"); printf("\n");
    printf("Delete vertex '#' and '!'...");
    if(DeleteVex(&G,'#') && DeleteVex(&G,'!')) printf("Succeeded!"); printf("\n");
    printf("Delete vertex '@'...");
    if(DeleteVex(&G,'@')) printf("Succeeded!"); printf("\n");
    DestroyGraph(&G);
    G.vexnum == 0 && G.arcnum == 0? printf("Graph is destroyed!\n"):printf("Graph isnot destroyed!\n");
    return 0;
}//OLGraph_test
