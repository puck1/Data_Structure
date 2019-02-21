
#ifndef SQLISTTYPE_H
#define SQLISTTYPE_H

#include <stdio.h>
#include <stdlib.h>
#include "../General/status.h"

// - - - - - 数据元素类型定义 - - - - -
#define MAXSIZE 20              //一个用作示例的小顺序表的最大长度
typedef int   KeyType;          //定义关键字类型为整数类型
typedef char *InfoType;         //暂定其他数据项类型为字符串型
typedef struct {
    KeyType key;                //关键字项
    InfoType otherinfo;         //其他数据项
}RecType;                       //记录类型
typedef struct {
    RecType r[MAXSIZE + 1];     //r[0]闲置或用作哨兵单元
    int     length;             //顺序表长度
}SqList;                        //顺序表类型

// - - - - - 对两个关键字的比较宏定义 - - - - -
    // - - 对数值型关键字
    #define  EQ(a,b)  ((a) == (b))
    #define  LT(a,b)  ((a) <  (b))
    #define  LQ(a,b)  ((a) <= (b))

    // - - 对字符串型关键字
    // #define  EQ(a,b)  (!strcmp((a),(b)))
    // #define  LT(a,b)  (strcmp((a),(b)) < 0)
    // #define  LQ(a,b)  (strcmp((a),(b)) <= 0)
    /**
     *  int strcmp((a),(b)):
     *  (a) == (b), return 0;
     *  (a) <  (b), return <0;
     *  (a) >  (b), return >0;
     */

// - - - - - 建表、遍历、清空等操作算法描述 - - - - -
Status CreateSortList(SqList *L){
    //以顺序存储建立一个待排序的表*L。
    int i;
    int length;
    printf("Enter number of element(s) in the list:");
    scanf("%d%*c",&length);
    if (length > MAXSIZE) { printf("OVERFLOW!\n"); exit(OVERFLOW); }
    else (*L).length = length;
    printf("Enter %d elements' keys seperated by delimiter:",(*L).length);
    for (i = 1; i <= (*L).length; ++i) scanf("%d%*c",&((*L).r[i].key));
    printf("Succeeded!\n");
    return OK;
} //CreateSortList

Status TraverseSortList(SqList L,Status (*Visit)(RecType)){
    //顺序遍历顺序存储的排序表L。
    int i;
    for(i = 1; i <= L.length; ++i)
        if (!Visit(L.r[i])) return ERROR;
    return OK;
} //TraverseSortList

void ClearSortList(SqList *L){
    //清空顺序存储的排序表*L。
    // int i;
    // for(i = 0; i <= (*L).length; ++i)
    //     if ((*L).r[i].otherinfo) {
    //         free((*L).r[i].otherinfo);
    //         (*L).r[i].otherinfo = NULL;
    //     }
    (*L).length = 0;
}

#endif // !SQLISTTYPE_H
