
#ifndef RADIXSORT_H
#define RADIXSORT_H

#include <stdio.h>
#include <stdlib.h>
#include "../General/status.h"

// - - - - - 宏函数定义 - - - - -
#define ord(x) (x)                      //整数类型存储
// #define ord(ch) ((ch) - '0')         //字符型存储
#define succ(x) ((x) + 1)

// - - - - - 数据元素类型定义 - - - - -
#define   MAX_NUM_OF_KEY    8           //关键字项数的最大值
#define   RADIX         10              //关键字基数，此时是十进制整数的基数
#define   MAX_SPACE   10000
typedef   int   KeysType;               //暂定关键字类型为整型
typedef   char  *InfoType;              //暂定其他数据项类型为字符串型
typedef struct {
    KeysType keys[MAX_NUM_OF_KEY];      //关键字
    InfoType otheritems;                //其他数据项
    int next;
}SLCell;                                //静态链表的结点类型
typedef struct {
    SLCell  r[MAX_SPACE];               //静态链表的可利用空间，r[0]为头结点
    int      keynum;                    //记录的当前关键字个数
    int      recnum;                    //静态链表的当前长度
}SLList;                                //静态链表类型
typedef int ArrType[RADIX];             //指针数组类型

// - - - - - 建表、遍历、清空等操作算法描述 - - - - -
Status CreateSLList(SLList *L){
    //建立采用静态链表表示的顺序表*L。
    int i, j;
    int rn, kn;                         //用于暂存记录个数、关键字个数
    printf("Enter number of element(s) in the list:");
    scanf("%d%*c",&rn);
    if (rn > MAX_SPACE - 1) { printf("OVERFLOW!\n"); exit(OVERFLOW); }
    //r[0]为头结点不存储数据，实际可用单元MAX_SPACE - 1
    else (*L).recnum = rn;
    printf("Enter number of each record's key(s):");
    scanf("%d%*c",&kn);
    if (kn > MAX_NUM_OF_KEY)
        { printf("Exceed the keys number limit!\n"); return ERROR; }
    else (*L).keynum = kn;
    for(i = 1; i <= (*L).recnum; ++i){
        printf("Enter No. %d record's %d keys sperated by delimiter:",i,(*L).keynum);
        //输入某个记录的所有关键字（以分隔符隔开）
        for(j = 0; j < (*L).keynum; ++j) scanf("%d%*c",&(*L).r[i].keys[j]);
        (*L).r[i].next = i + 1;         //链成静态链表
    }
    (*L).r[0].next = 1;                 //(*L).r[0].next存放第一个结点下标
    (*L).r[i - 1].next = 0;             //最后一个单元指向头结点
    printf("Succeeded!\n");
    return OK;
}//CreateSLList

Status TraverseSLList(SLList L,Status (*visit)(SLCell)){
    //按序遍历静态链表表示的排序表L，若遍历成功返回OK，否则ERROR。
    int i;
    for(i = L.r[0].next; i; i = L.r[i].next)
        if (!visit(L.r[i])) return ERROR;
    return OK;
}//TraverseSLList

void ClearSLList(SLList *L){
    //清空采用静态链表标识的顺序表*L。
    int i, j;
    for(i = (*L).r[0].next, j = (*L).r[i].next; i; i = j) {
        if ((*L).r[i].otheritems) {
            free((*L).r[i].otheritems);
            (*L).r[i].otheritems = NULL;
        }
        j = (*L).r[i].next;         //用j记录r[i]的后继
        (*L).r[i].next = 0;
    }
    (*L).keynum = (*L).recnum = 0;
}//ClearSLList

// - - - - - Radix Sort - - - - -
void Distribute(SLCell *r,int i,ArrType f,ArrType e){
    //静态链表L的r域中记录已按(keys[0],...,keys[i - 1])有序。
    //本算法按第i个关键字keys[i]建立RADIX个子表，使同一子表中记录的keys[i]相同，
    //f[0..RADIX - 1]和e[0..RADIX - 1]分别指向各子表中第一个和最后一个记录。
    int j, p;
    for(j = 0; j < RADIX; ++j) f[j] = 0;    //各子表初始化为空表
    for(p = r[0].next; p; p = r[p].next){
        j = ord(r[p].keys[i]);              //ord将记录中第i个关键字映射到[0..RADIX - 1]
        if (!f[j]) f[j] = p;
        else r[e[j]].next = p;              //e[j]为keys[i]相等的上一个p值
        e[j] = p;                           //将p所指的结点插入第j个子表中
    }
}//Distribute

void Collect(SLCell *r,int i,ArrType f,ArrType e){
    //本算法按keys[i]自小至大地将f[0..RADIX - 1]所指各子表依次链接成一个链表，
    //e[0..RADIX - 1]为各子表的尾指针。
    int j, t;
    for(j = 0; !f[j]; j = succ(j));     //找第一个非空子表，succ为求后继函数
    r[0].next = f[j]; t = e[j];         //r[0].next指向第一个非空子表中第一个结点
    while(j < RADIX){
        for(j = succ(j);j < RADIX - 1 && !f[j]; j = succ(j));       //找下一个非空子表★
        if (f[j]) { r[t].next = f[j]; t = e[j]; }                   //链接两个非空子表
    }
    r[t].next = 0;                      //t指向最后一个非空子表中的最后一个结点
}//Collect

void RadixSort(SLList *L){
    //L是采用静态链表表示的顺序表。
    //对*L作基数排序，使得*L成为按关键字自小到大的有序静态链表，L.r[0]为头结点。
    int i;
    ArrType f, e;
    for(i = 0; i < (*L).keynum; ++i){   //按最低位优先依次对各关键字进行分配和收集
        Distribute((*L).r,i,f,e);       //第i趟分配
        Collect((*L).r,i,f,e);          //第i趟收集
    }
}//RadixSort

#endif // !RADIXSORT_H
