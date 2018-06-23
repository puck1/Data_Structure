
#ifndef LINKEDSSTABLE_H
#define LINKEDSSTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include "STypedef.h"
#include "../General/status.h"

// - - - - - 静态查找表的链式存储结构（带尾结点） - - - - -
typedef struct STNode{      //结点类型
    STElemType      elem;
    struct STNode   *next;
}STNode, *STLink;
typedef struct {            //查找表类型
    STLink  head, tail;     //分别指向链表中的头结点和尾结点（均留空）
    int     length;         //查找表中数据元素的个数
}SSTable;

// - - - - - 基本操作的算法描述 - - - - -
Status CreateSSTable_Linked(SSTable *ST){
    //构造一个链式存储表示的含n个数据元素的静态查找表ST。
    int i;
    STLink p, q;
    printf("Enter number of element(s) in the table:");
    scanf("%d%*c",&(*ST).length);
    printf("Enter %d elements' keys seperated by delimiter:",(*ST).length);
    (*ST).head = (STLink)malloc(sizeof(STNode));    //头结点
    if (!(*ST).head) exit(OVERFLOW);
    for (i = 1; i <= (*ST).length; ++i) {
        if (!(p = (STLink)malloc(sizeof(STNode)))) exit(OVERFLOW);
        scanf("%d%*c",&(p->elem.key)); p->next = NULL;
        if (i == 1) (*ST).head->next = p;
        else q->next = p;
        q = p;
    }
    if (!(p = (STLink)malloc(sizeof(STNode)))) exit(OVERFLOW);
    p->next = NULL; q->next = p;        //插入一个尾结点作为“哨兵”
    printf("Succeeded!\n");
    return OK;
}//CreateSSTable_Linked

void DestroySSTable_Linked(SSTable *ST){
    //销毁链式存储表示的静态查找表ST。
    STLink p = (*ST).head->next, q;
    while (p){
        q = p->next;
        free(p); p = q;
    }
    free((*ST).head);
    (*ST).head = (*ST).tail = NULL;
    (*ST).length = 0;
    ST = NULL;
}//DestroySSTable_Linked

int Search_Linked(SSTable ST,KeyType key){
    //在链表ST中顺序查找其关键字等于key的数据元素。若找到，则函数值为
    //该元素在表中的位置，否则为0。
    STLink p = ST.head->next;
    int i;                              //i指示p所指数据元素在表中位置
    ST.tail->elem.key = key;            //“哨兵”
    for (i = 1; !EQ(p->elem.key,key); ++i) p = p->next;
    if (i > ST.length) return 0;       //找不到时，返回0
    else return i;
}//Search_Linked

Status Traverse_Linked(SSTable ST,Status Visit(STElemType)){
    //Visit是对元素操作的应用函数，按顺序对ST的每个元素调用函数Visit()一次且仅一次。
    //一旦Visit()失败，则操作失败。
    int i;
    STLink p = ST.head->next;
    for (i = 1; i <= ST.length; ++i){
        if (!Visit(p->elem)) return ERROR;
        p = p->next;
    }
    return OK;
}//Traverse_Linked

#endif // !LINKEDSSTABLE_H
