
#ifndef SEQSSTABLE_H
#define SEQSSTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include "STypedef.h"
#include "../General/status.h"

// - - - - - 静态查找表的顺序存储结构 - - - - -
typedef struct {
    STElemType  *elem;      //数据元素存储空间基址，建表时按实际长度分配，0号单元留空
    int         length;     //表长度
}SSTable;

// - - - - - 基本操作的算法描述 - - - - -
Status CreateSSTable_Seq(SSTable *ST){
    //构造一个顺序存储表示的含n个数据元素的静态查找表ST。
    int i;
    printf("Enter number of element(s) in the table:");
    scanf("%d%*c",&(*ST).length);
    (*ST).elem = (STElemType *)malloc(((*ST).length + 1) * sizeof(STElemType));
    if (!(*ST).elem) exit(OVERFLOW);
    printf("Enter %d elements' keys seperated by delimiter:",(*ST).length);
    for (i = 1; i <= (*ST).length; ++i) scanf("%d%*c",&((*ST).elem[i].key));
    printf("Succeeded!\n");
    return OK;
}//CreateSSTable_Seq

void DestroySSTable_Seq(SSTable *ST){
    //销毁顺序存储表示的静态查找表ST。
    if ((*ST).elem) free((*ST).elem);
    (*ST).elem = NULL; (*ST).length = 0;
    ST = NULL;
}//DestroySSTable_Seq

int Search_Seq(SSTable ST,KeyType key){
    //在顺序表ST中顺序查找其关键字等于key的数据元素。若找到，则函数值为
    //该元素在表中的位置，否则为0。
    int i;
    ST.elem[0].key = key;                               //“哨兵”
    for (i = ST.length; !EQ(ST.elem[i].key,key); --i);  //从后往前找
    return i;                                           //找不到时，i为0
}//Search_Seq

Status Traverse_Seq(SSTable ST,Status Visit(STElemType)){
    //Visit是对元素操作的应用函数，按顺序对ST的每个元素调用函数Visit()一次且仅一次。
    //一旦Visit()失败，则操作失败。
    int i;
    for (i = 1; i <= ST.length; ++i)
        if (!Visit(ST.elem[i])) return ERROR;
    return OK;
}//Traverse_Seq

#endif // !SEQSSTABLE_H
