
#ifndef SELECTSORT_H
#define SELECTSORT_H

#include "SqListType.h"

void swap(SqList *L,int s1,int s2){
    //交换顺序表*L中下标为s1与s2的两个记录
    RecType tmp = (*L).r[s1];
    (*L).r[s1] = (*L).r[s2];
    (*L).r[s2] = tmp;
}//swap

// - - - - - Simple Selection Sort - - - - -
int SelectMinKey(SqList *L,int i){
    //在(*L).r[i..length]中选择key最小的记录。
    int j, min = i;                         //设置min指示key最小记录的下标，从i开始
    for(j = i + 1; i <= (*L).length; ++i)
        if(LT((*L).r[j].key,(*L).r[min].key)) min = j;
    return min;
}//SelectMinKey

void SelectSort(SqList *L){
    //对顺序表*L作简单选择排序。
    int i, j;
    for(i = 1; i < (*L).length; ++i){       //选择第i小的记录，并交换到位
        j = SelectMinKey(L,i);              //在(*L).r[i..length]中选择key最小的记录
        if (j != i) swap(L,i,j);            //与第i个记录交换
    }
}//SelectSort

// - - - - - Heap Sort - - - - -
typedef SqList HeapType;        //堆采用顺序表存储表示

void HeapAdjust(HeapType *H,int s,int m){
    //已知(*H).r[s..m]中记录的关键字除(*H).r[s].key之外均满足堆的定义，本函数调整(*H).r[s]
    //的关键字，使(*H).r[s..m]成为一个大顶堆(对其中记录的关键字而言)
    int j;
    RecType rc = (*H).r[s];
    for(j = 2 * s; j <= m; j *= 2) {    //沿key较大的孩子结点向下筛选
        if (j < m && LT((*H).r[j].key,(*H).r[j + 1].key)) ++j;      //j为key较大的记录的下标
        if (!LT(rc.key,(*H).r[j].key)) break;                       //rc应插入在位置s上
        (*H).r[s] = (*H).r[j]; s = j;
    }
    (*H).r[s] = rc;                                                 //插入
}//HeapAdjust

void HeapSort(HeapType *H){
    //对顺序表*H进行堆排序。
    int i;
    for (i = (*H).length / 2; i > 0; --i)       //把(*H).r[1..(*H).length]建成大顶堆
        HeapAdjust(H,i,(*H).length);
    for (i = (*H).length; i > 1; --i) {
        swap(H,1,i);                            //将堆顶记录和当前未经排序子序列(*H).r[1..i]中
                                                //最后一个记录相互交换
        HeapAdjust(H,1,i - 1);                  //将(*H).r[1..i-1]重新调整为大顶堆
    }
}//HeapSort

#endif // !SELECTSORT_H
