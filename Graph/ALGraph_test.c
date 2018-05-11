
#include <stdio.h>
#include "ALGraph.h"

int main(int argc, char const *argv[]){
    //Operation;
    ALGraph G;
    printf("Create graph G:\n");
    CreateGraph(&G);
    printf("The first vertex is '%c',second is '%c',thrid is '%c,4th is '%c'.\n",
                    GetVex(G,1),GetVex(G,2),GetVex(G,3),GetVex(G,4));
    printf("Change vertex 'A' into '@'.\n"); PutVex(&G,'A','@');
    printf("The first adjacency vertex of '@' is '%c',its next adjacency vertex of 'B' is '%c'.\n",
                    FirstAdjVex(G,'@'),NextAdjVex(G,'@','B'));
    printf("Insert vertex '#' and '!' into graph...");
    if(InsertVex(&G,'#') && InsertVex(&G,'!')) printf("Succeeded!"); printf("\n");
    printf("InsertArc <@,!> and <@,#> and <!,#> into graph...");
    if(InsertArc(&G,'@','!',"Hello world!") && InsertArc(&G,'@','#',"Good to see u!")
                                            && InsertArc(&G,'!','#',"Good Night!"))
        printf("Succeeded!"); printf("\n");
    //    if(InsertArc(&G,'@','!') && InsertArc(&G,'@','#')
    //                                         && InsertArc(&G,'!','#'))
    //     printf("Succeeded!"); printf("\n");
    printf("Delete arc <@,!>...");
    if(DeleteArc(&G,'@','!')) printf("Succeeded!"); printf("\n");
    printf("Delete vertrex '#' and '!'...");
    if(DeleteVex(&G,'#') && DeleteVex(&G,'!')) printf("Succeeded!"); printf("\n");
    printf("Delete vertrex '@'...");
    if(DeleteVex(&G,'@')) printf("Succeeded!"); printf("\n");
    DestroyGraph(&G);
    G.vexnum == 0 && G.arcnum == 0? printf("Graph is destroyed!\n"):printf("Graph isnot destroyed!\n");
    return 0;
}
