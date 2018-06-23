/** @workspace/Searchinng/OrderTable.h
 *  This file includes binary search,Fibonacci search and interpolation search
 *  on a ordered static search table.
 */

#ifndef ORDERTABLE_H
#define ORDERTABLE_H

#include "STypedef.h"
#include "SeqSSTable.h"

// - - - - - 基本操作的算法描述 - - - - -
int Search_Bin(SSTable ST,KeyType key){
    //在有序表ST中折半查找其关键字等于key的数据元素。若找到，则函数值为
    //该元素在表中的位置，否则为0。
    int low, mid, high;
    low = 1; high = ST.length;                              //置区间初值
    while (low <= high){                                    //★
        mid = (low + high) / 2;
        if (EQ(key,ST.elem[mid].key)) return mid;           //找到待查元素
        else if (LT(key,ST.elem[mid].key)) high = mid - 1;  //继续在前半区间进行查找
        else low = mid + 1;                                 //继续在后半区间进行查找
    }
    return 0;                                               //顺序表中不存在待查元素
}//Search_Bin

int Search_Fib(SSTable ST,KeyType key){
    //在有序表ST中斐波那契查找其关键字等于key的数据元素。若找到，则函数值为
    //该元素在表中的位置，否则为0。
    //Fibonacci sequence:F0 = 0, F1 = 1, Fi = Fi-1 + Fi-2,i >= 2
    int i;
    int l, m, h;
    int F[ST.length + 5];               //足够大的斐波那契数
    F[0] = 0; F[1] = 1;
    for (i = 1; F[i] < ST.length; ++i) F[i+1] = F[i] + F[i-1];  //找出F[i] >= ST.length
    l = 1; h = ST.length;
    while (l <= h){
        m = l + F[i-1] - 1;
        if (m > ST.length) m = ST.length;
        if (EQ(key,ST.elem[m].key)) return m;
        else if (LT(key,ST.elem[m].key))
            { --i; h = m - 1; }         //在前部区域进行查找
        else
            { i -= 2; l = m + 1; }      //在后部区域进行查找
    }//while
    return 0;
}//Search_Fib

int Search_Int(SSTable ST,KeyType key){
    //在有序表ST中插值查找其关键字等于key的数据元素。若找到，则函数值为
    //该元素在表中的位置，否则为0。
    int l, m ,h;
    l = 1; h = ST.length;
    while (l <= h){
        m = l + (1.0 * (key - ST.elem[l].key)) / (ST.elem[h].key - ST.elem[l].key) * (h - l);
                    //转换浮点数计算★
        if (EQ(key,ST.elem[m].key)) return m;
        else if (LT(key,ST.elem[m].key)) h = m - 1;
        else l = m + 1;
    }
    return 0;
}//Search_Int

#endif // !ORDERTABLE_H
