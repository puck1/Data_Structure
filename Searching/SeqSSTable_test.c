
#include <stdio.h>
#include "SeqSSTable.h"

Status PrintElement(STElemType e){
    //输出元素e的值
    printf("%d ",e.key);
    return OK;
}//PrintElement

int main(int argc, char const *argv[]){
    //Operation;
    SSTable ST;
    CreateSSTable_Seq(&ST,10);      //含有10个元素
    printf("Sequential static table traverse:");
    Traverse_Seq(ST,PrintElement); printf("\n");
    printf("The order of ST's element whose key == 20 is %d.\n",Search_Seq(ST,20));
    return 0;
}//SeqSSTable_test
