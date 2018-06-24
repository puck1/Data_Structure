
#ifndef INDEXSEQTABLE
#define INDEXSEQTABLE

#include <stdio.h>
#include <stdlib.h>
#include "STypedef.h"
#include "../General/status.h"

// - - - - - 索引顺序表结构定义 - - - - -
typedef struct {
    KeyType     maxkey;         //子表内的最大关键字
    int         index;          //子表的第一个记录在表中位置
}IndexTerm;                     //索引表项
typedef struct {
    IndexTerm   *it;            //对应的索引表空间基址，建表时按子表数目分配，0号单元留空
    int         subnum;         //子表的数目，用于对索引表的折半查找
    STElemType  *elem;          //数据元素存储空间基址，建表时按实际长度分配，0号单元留空
    int         length;         //表长度
}ISTable;

// - - - - - 基本操作的算法描述 - - - - -
Status CreateISTable(ISTable *ST){
    //构造一个含n个数据元素的索引顺序表ST。
    int i, j;
    int subcnt;
    int index = 1;
    printf("Enter number of element(s) in the table:");
    scanf("%d%*c",&(*ST).length);
    (*ST).elem = (STElemType *)malloc(((*ST).length + 1) * sizeof(STElemType));
    if (!(*ST).elem) exit(OVERFLOW);
    if ((*ST).length){
        printf("Enter max quantity of elements in every subtable:"); scanf("%d%*c",&subcnt);
                                                                    //subcnt指示子表最大元素数目
        if (!subcnt) return ERROR;
        (*ST).subnum = (*ST).length / subcnt;       //先暂时创建的子表数目
        if (!((*ST).it = (IndexTerm *)malloc(((*ST).subnum + 1) * sizeof(IndexTerm)))) exit(OVERFLOW);
        for (i = 1; i <= (*ST).subnum; ++i){
            printf("Enter all %d elements' keys seperated by delimiter of subtable %d:",subcnt,i);
            (*ST).it[i].maxkey = 0; (*ST).it[i].index = index;      //初始化最大关键字与索引表项
            for (j = 1; j <= subcnt; ++j, index++){
                scanf("%d%*c",&(*ST).elem[index].key);
                if ((*ST).elem[index].key > (*ST).it[i].maxkey)
                    (*ST).it[i].maxkey = (*ST).elem[index].key;
            }//for
            if (i - 1 && !LQ((*ST).it[i - 1].maxkey,(*ST).it[i].maxkey)) return ERROR;  //索引表必须有序
        }//for
        if ((*ST).length % subcnt){                 //存在剩余项，添加含有元素较少的一个子表
            if (!((*ST).it = (IndexTerm *)realloc((*ST).it,
                                (++(*ST).subnum + 1) * sizeof(IndexTerm)))) exit(OVERFLOW);
            printf("Enter all %d elements' keys seperated by delimiter of No.%d subtable:"
                                                    ,(*ST).length % subcnt,(*ST).subnum);
            (*ST).it[(*ST).subnum].maxkey = 0; (*ST).it[(*ST).subnum].index = index;
            for (j = 1; j <= (*ST).length % subcnt; ++j, index++){
                scanf("%d%*c",&(*ST).elem[index].key);
                if ((*ST).elem[index].key > (*ST).it[(*ST).subnum].maxkey)
                    (*ST).it[(*ST).subnum].maxkey = (*ST).elem[index].key;
            }//for
            if (!LQ((*ST).it[(*ST).length - 1].maxkey,(*ST).it[(*ST).length].maxkey)) return ERROR;
        }//if
    }//if
    printf("Succeeded!\n");
    return OK;
}//CreateISTable

void DestroyISTable(ISTable *ST){
    //销毁索引顺序表ST。
    if ((*ST).elem) free((*ST).elem);
    (*ST).elem = NULL; (*ST).length = 0;
    if ((*ST).it) free((*ST).it);
    (*ST).it = NULL; (*ST).subnum = 0;
    ST = NULL;
}//DestroyISTable

int Search_index(ISTable ST,KeyType key){
    //在索引顺序表ST中分块查找其关键字等于key的数据元素。若找到，则函数值为
    //该元素在表中的位置，否则为0。
    int i;
    int low, mid, high;
    int start = 0, end = 0;     //查找区间的始端和尾端
    low = 1; high = ST.subnum;
    while (low <= high){
        mid = (low + mid) / 2;
        if (LT(ST.it[mid].maxkey,key)){
            if (mid + 1 < ST.subnum && LQ(key,ST.it[mid + 1].maxkey)){
                // ST.it[mid].maxkey < key <= ST.it[mid + 1].maxkey
                start = ST.it[mid + 1].index;                           //始端为mid+1段开端（含）
                if (mid + 2 < ST.subnum) end = ST.it[mid + 2].index;    //尾端到mid+2段开端（不含）
                else end = ST.length;                                   //或总表结束（含）
                break;
            }//if
            else high = mid - 1;
        }//if
        else if ((mid - 1 && LT(ST.it[mid - 1].maxkey,key)) || !(mid - 1)){
            // 0 < key <= ST.it[mid].maxkey || ST.it[mid - 1].maxkey < key <= ST.it[mid].maxkey
            start = ST.it[mid].index;                                   //始端为mid段开端（含）
            if (mid + 1 < ST.subnum) end = ST.it[mid + 1].index;        //尾端到mid+1段开端（不含）
            else end = ST.length;                                       //或总表结束（含）
            break;
        }//else if
        else low = mid + 1;
    }//while
    if (end == ST.length) ++end;    //使查找的区间包含ST.length（总表结束）
    for (i = start; i < end; ++i)
        if (ST.elem[i].key == key) return i;
    return 0;
}//Search_index

Status Traverse(ISTable ST,Status Visit(STElemType)){
    //Visit是对元素操作的应用函数，按顺序对ST的每个子表元素调用函数Visit()一次且仅一次。
    //一旦Visit()失败，则操作失败。
    int i, j;
    int start, end;
    for (i = 1; i < ST.subnum; ++i){            //遍历前ST.subnum - 1个子表
        printf("Visit subtable %d:",i);
        start = ST.it[i].index;
        end = ST.it[i + 1].index;
        for (j = start; j < end; j++)
            if (!Visit(ST.elem[j])) return ERROR;
        printf("\n");
    }//for
    if (ST.subnum){                             //遍历第ST.subnum个子表
        printf("Visit subtable %d:",ST.subnum);
        start = ST.it[ST.subnum].index;
        end = ST.length;
        for (j = start; j <= end; ++j)          //此时j <= end,即包括第ST.length个元素
            if (!Visit(ST.elem[j])) return ERROR;
        printf("\n");
    }//if
    printf("Succeeded!\n");
    return OK;
}//Traverse

#endif // !INDEXSEQTABLE
