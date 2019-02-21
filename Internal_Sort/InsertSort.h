
#ifndef INSERTSORT_H
#define INSERTSORT_H

#include "SqListType.h"

void StraightInsertSort(SqList *L){
    //对顺序表*L作直接插入排序。
    int i, j;
    for(i = 2; i <= (*L).length; ++i)
        if (LT((*L).r[i].key,(*L).r[i - 1].key)) {      //"<"，需将(*L).r[i]插入有序子表
            (*L).r[0] = (*L).r[i];                      //复制为哨兵
            (*L).r[i] = (*L).r[i - 1];
            for(j = i - 2; LT((*L).r[j].key,(*L).r[0].key); --j)
                (*L).r[j + 1] = (*L).r[j];              //记录后移
            (*L).r[j + 1] = (*L).r[0];                  //插入到正确位置
        }
}//StraightInsertSort

void BInsertSort(SqList *L){
    //对顺序表*L作折半插入排序。
    int low, high, m;
    int i, j;
    for(i = 2; i <= (*L).length; ++i) {
        (*L).r[0] = (*L).r[i];                      //将(*L).r[i]暂存到(*L).r[0]
        low = 1; high = i - 1;
        while (low <= high) {                       //在r[low..high]中折半查找有序插入的位置
            m = (low + high) / 2;                                           //折半
            if(LT((*L).r[0].key,(*L).r[m].key)) high = m - 1;               //插入点在低半区
            else low = m + 1;                                               //插入点在高半区
        }//while
        for(j = i - 1; j >= high + 1; --j) (*L).r[j + 1] = (*L).r[j];       //记录后移
        (*L).r[high + 1] = (*L).r[0];                                       //插入
    }//for
}//BInsertSort

void ShellInsert(SqList *L,int dk){
    //对顺序表*L作一趟希尔插入排序。本算法是和一趟直接插入排序相比，做了以下修改：
    //    1.前后记录位置的增量是dk，而不是1；
    //    2.r[0]只是暂存单元，不是哨兵。当j <= 0时，插入位置已找到。
    int i, j;
    for(i = dk + 1; i <= (*L).length; ++i)
        if (LT((*L).r[i].key,(*L).r[i - dk].key)) {     //需将L.r[i]插入有序增量子表
            (*L).r[0] = (*L).r[i];                      //暂存在L.r[0]
            for(j = i - dk; j > 0 && LT((*L).r[0].key,(*L).r[j].key); j -= dk)
                (*L).r[j + dk] = (*L).r[j];             //记录后移，查找插入位置
            (*L).r[j + dk] = (*L).r[0];                 //插入
        }
}//ShellInsert

void ShellSort(SqList *L,int dlta[],int t){
    //按增量序列dlta[0..t - 1]对顺序表*L作希尔排序。
    for(int k = 0; k < t; ++k)
        ShellInsert(L,dlta[k]);         //一趟增量为dlta[k]的插入排序
}//ShellSort

#endif // !INSERTSORT_H
