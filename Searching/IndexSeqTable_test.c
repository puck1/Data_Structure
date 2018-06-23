
#include <stdio.h>
#include "IndexSeqTable.h"

Status PrintElement(STElemType e){
    //输出元素e的值
    printf("%d ",e.key);
    return OK;
}//PrintElement

int main(int argc, char const *argv[]){
    //Operation;
    ISTable ST;
    printf("Create index sequential table ST:\n");
    CreateISTable(&ST);
    printf("All elements of table ST:\n"); Traverse(ST,PrintElement);
    printf("Element whose key is '%d' found at index %d.\n",44,Search_index(ST,44));
    printf("Element whose key is '%d' found at index %d.\n",80,Search_index(ST,80));
    DestroyISTable(&ST);
    (!ST.elem && !ST.it && !ST.length && !ST.subnum) ?
        printf("ST is destroyed!\n") : printf("ST isnot destroyed!\n");
    return 0;
}//IndexSeqTable_test
