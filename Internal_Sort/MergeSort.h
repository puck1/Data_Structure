
#ifndef MERGESORT_H
#define MERGESORT_H

#include "SqListType.h"

RecType TR2[MAXSIZE + 1];                               //全局变量，用于MSort算法递归中存放中间结果

void Merge(RecType SR[],RecType TR[],int i,int m,int n){
    //将有序的SR[i..m]和SR[m + 1..n]归并为有序的TR[i..n]
    int j, k;
    for(j = m + 1,k = i; i <= m && j <= n; ++k){        //将SR中记录由小到大地并入TR
        if (LQ(SR[i].key,SR[j].key)) TR[k] = SR[i++];
        else TR[k] = SR[j++];
    }
    while(i <= m) TR[k++] = SR[i++];                    //将剩余的SR[i..m]复制到TR
    while(j <= n) TR[k++] = SR[j++];                    //将剩余的SR[j..n]复制到TR
                                                        //以上两个while循环只有一个会执行
}//Merge

void MSort(RecType SR[],RecType TR1[],int s,int t){
    //将SR[s..t]归并排序为TR1[s..t]
    int m;
    if (s == t) TR1[s] = SR[s];
    else {
        m = (s + t) / 2;            //将SR[s..t]平分为SR[s..m]和SR[m + 1..t]
        MSort(SR,TR2,s,m);          //递归地将SR[s..m]归并为有序的TR2[s..m]
        MSort(SR,TR2,m + 1,t);      //递归地将SR[m + 1..t]归并为有序的TR2[m + 1..t]
        Merge(TR2,TR1,s,m,t);       //将TR2[s..m]和TR2[m + 1..t]归并到TR1[s..t]
    }
}//MSort

void MergeSort(SqList *L){
    //对顺序表*L作归并排序
    MSort((*L).r,(*L).r,1,(*L).length);
}//MergeSort

#endif // !MERGESORT_H
