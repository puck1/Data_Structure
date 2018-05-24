
#include <stdio.h>
#include "status.h"
#include "MGraph.h"

Status PrintElement(VertexType e){
    //输出元素e的值
    if(e == ' ') return ERROR;
    else printf("%c ",e);
    return OK;
}//PrintElement

int main(){
    //Operation;
    MGraph G;
    printf("Create graph G:\n");
    CreateGraph(&G);

    printf("The first vertex is '%c',second is '%c',thrid is '%c,4th is '%c'.\n",
                    GetVex(G,1),GetVex(G,2),GetVex(G,3),GetVex(G,4));
    printf("Change vertex 'A' into '@'..."); if(PutVex(&G,'A','@')) printf("Succeeded!"); printf("\n");
    printf("The first adjacency vertex of '@' is '%c',its next adjacency vertex of 'B' is '%c'.\n",
                    FirstAdjVex(G,'@'),NextAdjVex(G,'@','B'));

    printf("DFSTraverse:"); DFSTraverse(G,&PrintElement); printf("\n");
    printf("BFSTraverse:"); BFSTraverse(G,&PrintElement); printf("\n");

    printf("Insert vertex '#' and '!' into graph...");
    if(InsertVex(&G,'#') && InsertVex(&G,'!')) printf("Succeeded!"); printf("\n");

    printf("InsertArc <@,!> and <@,#> and <!,#> into graph...");
    if(InsertArc(&G,'@','!',5,1,"Hello world!") && InsertArc(&G,'@','#',10,1,"Good to see u!")
                                            && InsertArc(&G,'!','#',15,1,"Good Night!"))
        printf("Succeeded!"); printf("\n");
    // if(InsertArc(&G,'@','!',0) && InsertArc(&G,'@','#',0) && InsertArc(&G,'!','#',0))
    // printf("Succeeded!"); printf("\n");

    printf("Delete arc <@,!>...");
    if(DeleteArc(&G,'@','!')) printf("Succeeded!"); printf("\n");
    printf("Delete vertex '#' and '!'...");
    if(DeleteVex(&G,'#') && DeleteVex(&G,'!')) printf("Succeeded!"); printf("\n");
    printf("Delete vertex '@'...");
    if(DeleteVex(&G,'@')) printf("Succeeded!"); printf("\n");
    DestroyGraph(&G);
    G.vexnum == 0 && G.arcnum == 0? printf("Graph is destroyed!\n"):printf("Graph isnot destroyed!\n");
    return 0;
}//MGraph_test
