
#include <stdio.h>
typedef void Record;        //测试中记录定为void型
#include "B-Tree.h"

Status PrintKeyword(KeyType K){
    //输出整数关键字K的值
    printf("%d ",K);
    return OK;
}//PrintKeyword

int main(int argc, char const *argv[]){
    //Operation;
    //Input for test: number of keywords:10; Keyword values:45 24 53 3 37 50 61 90 100 70EOF
    BTree T;
    Result r;
    KeyType K;
    int choice;

    printf("Create B-Tree T...\n");
    CreateBTree(&T);
    printf("Ordered sequence of keyword value:");
    InorderTraverseBTree(T,PrintKeyword); printf("\n");
    printf("Level order traverse of B-Tree:\n");
    LevelOrderTraverseBTree(T,PrintKeyword);printf("\n");

    r = SearchBTree(T,24); r.tag ? printf("Key 24 is in T.\n") : printf("Key 24 isn't in T.\n");
    r = SearchBTree(T,30); r.tag ? printf("Key 30 is in T.\n") : printf("Key 30 isn't in T.\n");

    printf("\n");
    printf("Test of insertion or deletion?(0/1):");
    do{
        scanf("%d%*c",&choice);
    } while (choice != 1 && choice != 0);

    if (choice == 0){
        printf("Insert key 12 into T..."); K = 12;
        InsertBTree(&T,K) ? printf("Succeeded!\n") : printf("Failed!\n");

        printf("Insert key 30 into T..."); K = 30;
        InsertBTree(&T,K) ? printf("Succeeded!\n") : printf("Failed!\n");

        printf("Insert key 26 into T..."); K = 26;
        InsertBTree(&T,K) ? printf("Succeeded!\n") : printf("Failed!\n");

        printf("Insert key 85 into T..."); K = 85;
        InsertBTree(&T,K) ? printf("Succeeded!\n") : printf("Failed!\n");

        printf("Insert key 7 into T..."); K = 7;
        InsertBTree(&T,K) ? printf("Succeeded!\n") : printf("Failed!\n");

        printf("Ordered sequence of keyword value:");
        InorderTraverseBTree(T,PrintKeyword); printf("\n");
        printf("Level order traverse of B-Tree:\n");
        LevelOrderTraverseBTree(T,PrintKeyword);printf("\n");
    }//if
    else{
        // printf("Delete key 45 in T..."); K = 45;
        // DeleteBTree(&T,K) ? printf("Succeeded!\n") : printf("Failed!\n");

        printf("Delete key 50 in T..."); K = 50;
        DeleteBTree(&T,K) ? printf("Succeeded!\n") : printf("Failed!\n");

        printf("Delete key 53 in T..."); K = 53;
        DeleteBTree(&T,K) ? printf("Succeeded!\n") : printf("Failed!\n");

        printf("Delete key 37 in T..."); K = 37;
        DeleteBTree(&T,K) ? printf("Succeeded!\n") : printf("Failed!\n");

        printf("Ordered sequence of keyword value:");
        InorderTraverseBTree(T,PrintKeyword); printf("\n");
        printf("Level order traverse of B-Tree:\n");
        LevelOrderTraverseBTree(T,PrintKeyword);printf("\n");
    }//else

    DestroyBTree(&T); T ? printf("T isn't destroyed!\n") : printf("T is destroyed!\n");
    return 0;
}//B-Tree_test
