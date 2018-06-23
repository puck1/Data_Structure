
#include <stdio.h>
#include "LinkedSSTable.h"

Status PrintElement(STElemType e){
    //输出元素e的值
    printf("%d ",e.key);
    return OK;
}//PrintElement

int main(int argc, char const *argv[]){
    //Operation;
    SSTable ST;
    CreateSSTable_Linked(&ST);
    printf("Linked static table traverse:");
    Traverse_Linked(ST,PrintElement); printf("\n");
    printf("The order of ST's element whose key == 20 is %d.\n",Search_Linked(ST,20));
    return 0;
}//LinkedSSTable_test
