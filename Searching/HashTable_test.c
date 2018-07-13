
#include <stdio.h>
#include "HashTable.h"

Status PrintElement(STElemType e){
    //输出元素e的值
    printf("%d ",e.key);
    return OK;
}//PrintElement

int main(int argc, char const *argv[]){
    //Operation;
    //Eg:14 01 68 27 55 19 20 84 79 23 11 10EOF

    HashTable H;
    KeyType K;

    printf("Create Hash Table...\n");
    CreateHash(&H);
    printf("Hash table capacity: %03d, total number of elements: %03d.\n",hashsize[H.sizeindex],H.count);
    printf("All keywords of each element:");
    TraverseHash(H,PrintElement); printf("\n\n");

    printf("Delete key 84 in T..."); K = 84;
    DeleteHash(&H,K) ? printf("Succeeded!\n") : printf("Failed!\n");
    printf("Delete key 100 in T..."); K = 100;
    DeleteHash(&H,K) ? printf("Succeeded!\n") : printf("Failed!\n");
    printf("Hash table capacity: %03d, total number of elements: %03d.\n",hashsize[H.sizeindex],H.count);
    printf("All keywords of each element:");
    TraverseHash(H,PrintElement); printf("\n\n");

    DestroyHash(&H);
    H.count || H.elem || H.sizeindex ? printf("H isnot destroyed!\n") : printf("H is destroyed!\n");

    return 0;
}//HashTable_test
