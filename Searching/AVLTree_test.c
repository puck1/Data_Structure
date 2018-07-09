
#include <stdio.h>
#include "AVLTree.h"

Status PrintElement(STElemType e){
    //输出元素e的值
    printf("%d ",e.key);
    return OK;
}//PrintElement

int main(int argc, char const *argv[]){
    //Operation;
    BBSTree T;
    STElemType e;
    bool taller, shorter;

    /** Eg:    NOT AN AVL TREE      ->      AVL TREE      ->      MODIFIED
     *                50                        50                    23
     *              /   \                     /   \                 /    \
     *             /     \                   /     \               /      \
     *            17     76                 17     72             17      67
     *          /   \    /                /   \   /  \          /   \    /  \
     *         9    23  54               12   23 54  76        12   19  54  72
     *         \    /    \              / \   /   \           /            /
     *         14  19    72            9  14 19   67         9            70
     *         /         /
     *        12        67
     */

    printf("Create AVL TREE T:\n");
    CreateAVL(&T);
    printf("Ordered sequence:");
    InorderTraverseAVL(T,PrintElement); printf("\n");

    SearchAVL(T,14) == NULL ? printf("Key 14 isn't in T.\n") : printf("Key 14 is in T.\n");
    SearchAVL(T,70) == NULL ? printf("Key 70 isn't in T.\n") : printf("Key 70 is in T.\n");

    printf("Insert key 14 into T..."); e.key = 14; taller = FALSE;
    InsertAVL(&T,e,&taller) ? printf("Succeeded!\n") : printf("Failed!\n");
    printf("Insert key 70 into T..."); e.key = 70; taller = FALSE;
    InsertAVL(&T,e,&taller) ? printf("Succeeded!\n") : printf("Failed!\n");
    printf("Ordered sequence:");
    InorderTraverseAVL(T,PrintElement); printf("\n");

    printf("Delete key 14 in T..."); shorter = FALSE;
    DeleteAVL(&T,14,&shorter) ? printf("Succeeded!\n") : printf("Failed!\n");
    printf("Delete key 76 in T..."); shorter = FALSE;
    DeleteAVL(&T,76,&shorter) ? printf("Succeeded!\n") : printf("Failed!\n");
    printf("Delete key 50 in T..."); shorter = FALSE;
    DeleteAVL(&T,50,&shorter) ? printf("Succeeded!\n") : printf("Failed!\n");
    printf("Delete key 100 in T..."); shorter = FALSE;
    DeleteAVL(&T,100,&shorter) ? printf("Succeeded!\n") : printf("Failed!\n");
    printf("Ordered sequence:");
    InorderTraverseAVL(T,PrintElement); printf("\n");

    DestroyAVL(&T); T ? printf("T isn't destroyed!\n") : printf("T is destroyed!\n");
    return 0;
}//AVLTree_test
