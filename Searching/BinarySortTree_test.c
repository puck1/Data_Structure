
#include <stdio.h>
#include "BinarySortTree.h"

Status PrintElement(STElemType e){
    //输出元素e的值
    printf("%d ",e.key);
    return OK;
}//PrintElement

int main(int argc, char const *argv[]){
    //Operation;
    BiTree T;
    STElemType e;

    CreateBST(&T);
    printf("Ordered sequence:");
    InorderTraverseBST(T,PrintElement); printf("\n");

    SearchBST_1(T,40) == NULL ? printf("Key 40 isn't in T.\n") : printf("Key 40 is in T.\n");
    SearchBST_1(T,100) == NULL ? printf("Key 100 isn't in T.\n") : printf("Key 100 is in T.\n");

    printf("Insert key 40 into T..."); e.key = 40;
    InsertBST(&T,e) ? printf("Succeeded!\n") : printf("Failed!\n");
    printf("Insert key 100 into T..."); e.key = 100;
    InsertBST(&T,e) ? printf("Succeeded!\n") : printf("Failed!\n");
    printf("Ordered sequence:");
    InorderTraverseBST(T,PrintElement); printf("\n");

    printf("Delete key 35 in T...");
    DeleteBST(&T,35) ? printf("Succeeded!\n") : printf("Failed!\n");
    printf("Delete key 45 in T...");
    DeleteBST(&T,45) ? printf("Succeeded!\n") : printf("Failed!\n");
    printf("Delete key 100 in T...");
    DeleteBST(&T,100) ? printf("Succeeded!\n") : printf("Failed!\n");
    printf("Ordered sequence:");
    InorderTraverseBST(T,PrintElement); printf("\n");

    DestroyBST(&T); T ? printf("T isn't destroyed!\n") : printf("T is destroyed!\n");

    return 0;
}//BST_test
