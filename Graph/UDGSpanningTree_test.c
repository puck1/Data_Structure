
#include <stdio.h>
#include "status.h"
#include "UDGSpanningTree.h"

Status PrintElement(VertexType e){
    //输出元素e的值
    if(e == ' ') return ERROR;
    else printf("%c ",e);
    return OK;
}//PrintElement

int main(int argc, char const *argv[]){
    CSTree DT, BT;
    ALGraph G;
    CreateUDG(&G);

    printf("DFSTraverse:"); DFSTraverse(G,&PrintElement); printf("\n");
    printf("BFSTraverse:"); BFSTraverse(G,&PrintElement); printf("\n");

    DFSForest(G,&DT);
    BFSForest(G,&BT);

    printf("PreOrderTraverse DFSTree:");
    PreOrderTraverse_Recur(DT,PrintElement); printf("\n");
    printf("PostOrderTraverse DFSTree:");
    PostOrderTraverse_Recur(DT,PrintElement); printf("\n");
    printf("PreOrderTraverse BFSTree:");
    PreOrderTraverse_Recur(BT,PrintElement); printf("\n");
    printf("PostOrderTraverse BFSTree:");
    PostOrderTraverse_Recur(BT,PrintElement); printf("\n");
    return 0;
}//UDGSpanningTree_test
