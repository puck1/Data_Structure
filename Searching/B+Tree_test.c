
#include <stdio.h>
typedef void Record;        //测试中记录定为void型
#include "B+Tree.h"

Status PrintKeyword(KeyType K){
    //输出整数关键字K的值
    printf("%d ",K);
    return OK;
}//PrintKeyword

int main(int argc, char const *argv[]){
    //Operation;
    //Input for test: number of keywords:7; Keyword values:2 4 7 10 17 21 28EOF
    BPTree T;
    KeyType K;

    printf("Create B-Tree T...\n");
    CreateBPTree(&T);
    printf("Ordered sequence of keyword value:");
    InorderTraverseBPTree(T,PrintKeyword); printf("\n");
    printf("Sequence of leaf keyword value:");
    SeqTraverseBPTree(T,PrintKeyword); printf("\n");
    printf("Level order traverse of B-Tree:\n");
    LevelOrderTraverseBPTree(T,PrintKeyword);printf("\n");

    printf("Delete key 28 in T..."); K = 28;
    DeleteBPTree(&T,K) ? printf("Succeeded!\n") : printf("Failed!\n");

    printf("Delete key 4 in T..."); K = 4;
    DeleteBPTree(&T,K) ? printf("Succeeded!\n") : printf("Failed!\n");

    printf("Delete key 7 in T..."); K = 7;
    DeleteBPTree(&T,K) ? printf("Succeeded!\n") : printf("Failed!\n");

    printf("Delete key 2 in T..."); K = 2;
    DeleteBPTree(&T,K) ? printf("Succeeded!\n") : printf("Failed!\n");

    printf("Ordered sequence of keyword value:");
    InorderTraverseBPTree(T,PrintKeyword); printf("\n");
    printf("Sequence of leaf keyword value:");
    SeqTraverseBPTree(T,PrintKeyword); printf("\n");
    printf("Level order traverse of B-Tree:\n");
    LevelOrderTraverseBPTree(T,PrintKeyword);printf("\n");

    DestroyBPTree(&T); (T.root || T.sqt) ? printf("T isn't destroyed!\n") : printf("T is destroyed!\n");
    return 0;
}//B+Tree_test
