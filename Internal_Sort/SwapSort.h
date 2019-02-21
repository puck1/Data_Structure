
#ifndef SWAPSORT_H
#define SWAPSORT_H

#include "SqListType.h"

void swap(SqList *L,int s1,int s2){
    //交换顺序表*L中下标为s1与s2的两个记录
    RecType tmp = (*L).r[s1];
    (*L).r[s1] = (*L).r[s2];
    (*L).r[s2] = tmp;
}//swap

// - - - - - Bubble Sort - - - - -
void BubbleSort(SqList *L){
    //对顺序表*L作冒泡排序，布尔变量change指示上一趟冒泡是否发生变化
    int i, j;
    bool change;
    for(i = (*L).length, change = TRUE; i > 1 && change; --i){      //最多共(*L).length - 1趟冒泡
        change = FALSE;
        for(j = 1; j < i; ++j)
            if (LT((*L).r[j + 1].key,(*L).r[j].key)){               //将大的关键字向后冒泡
                (*L).r[0] = (*L).r[j + 1];
                (*L).r[j + 1] = (*L).r[j];
                (*L).r[j] = (*L).r[0];
                // swap(L,j,j + 1);
                change = TRUE;
            }//if
    }//for
}//BubbleSort

// - - - - - Quick Sort - - - - -
int Partition(SqList *L,int low,int high){
    //交换顺序表*L中子表r[low..high]的记录，枢轴记录到位，并返回其所在位置，此时
    //在它之前（后）的记录均不大（小）于它。
    (*L).r[0] = (*L).r[low];                                //用子表的第一个记录作枢轴记录
    KeyType pivotkey = (*L).r[low].key;                     //枢轴记录关键字
    while(low < high){                                      //从表的两端交替地向中间扫描
        while(low < high && LQ(pivotkey,(*L).r[high].key)) --high;
        (*L).r[low] = (*L).r[high];                         //将比枢轴记录小的记录移到低端
        while(low < high && LQ((*L).r[low].key,pivotkey)) ++low;
        (*L).r[high] = (*L).r[low];                         //将比枢轴记录大的记录移到高端
    }
    (*L).r[low] = (*L).r[0];                                //枢轴记录到位
    return low;                                             //返回枢轴位置
}//Partition

void QSort(SqList *L,int low,int high){
    //对顺序表*L中的子序列(*L).r[low..high]作快速排序
    int pivotloc;
    if (low < high) {                               //长度大于1
        pivotloc = Partition(L,low,high);           //将(*L).r[low..high]一分为二
        QSort(L,low,pivotloc - 1);                  //对低子表递归排序，pivotloc是枢轴位置
        QSort(L,pivotloc + 1,high);                 //对高子表递归排序
    }
}//QSort

void QuickSort(SqList *L){
    //对顺序表*L作快速排序。
    QSort(L,1,(*L).length);
}//QuickSort

// - - - - - Improved Quick Sort - - - - -
int Partition_1(SqList *L,int low,int high,bool *change_low,bool *change_high){
    //Partition算法的改良，取(*L).r[low].key、(*L).r[high].key、(*L).r[⌊(low + high) / 2⌋].key
    //三者中关键字取中值的记录为枢轴，并在“一次划分”的过程中，在指针high减1和low增1的同时进行“起泡”操作，
    //即在相邻两个记录处于“逆序”时进行互换，同时在算法中附设两个布尔型变量change_low和change_high
    //分别指示指针low和high在从两端向中间移动过程中是否进行过交换记录的操作，若指针low在从低端到中间的
    //移动过程中没有进行交换记录的操作，则不再需要对低端子表进行排序，指针high类似。
    //算法使枢轴记录到位，并返回其所在位置，此时在它之前（后）的记录均不大（小）于它。
    int pivot;
    int mid = (low + high) / 2;
    if (LQ((*L).r[low].key,(*L).r[high].key)) {
        if (LQ((*L).r[high].key,(*L).r[mid].key)) pivot = high;
        else if (LQ((*L).r[low].key,(*L).r[mid].key)) pivot = mid;
        else pivot = low;
    }
    else {
        if (LQ((*L).r[mid].key,(*L).r[high].key)) pivot = high;
        else if (LQ((*L).r[mid].key,(*L).r[low].key)) pivot = mid;
        else pivot = low;
    }                                                       //low、mid、high关键字三者取中为枢轴pivot
    swap(L,low,pivot);                                      //将枢轴记录交换到第一个位置
    (*L).r[0] = (*L).r[low];
    KeyType pivotkey = (*L).r[low].key;                     //枢轴记录关键字
    while(low < high){                                      //从表的两端交替地向中间扫描
        while(low < high && LQ(pivotkey,(*L).r[high].key)) {
            if (LT((*L).r[high].key,(*L).r[high - 1].key)) {
                swap(L,high - 1,high);                      //指针high减1同时进行“起泡”操作
                *change_high = TRUE;
            }//if
            --high;
        }//while
        (*L).r[low] = (*L).r[high];                         //将比枢轴记录小的记录移到低端
        while(low < high && LQ((*L).r[low].key,pivotkey)) {
            if (LT((*L).r[low + 1].key,(*L).r[low].key)) {
                swap(L,low,low + 1);                        //指针low加1同时进行“起泡”操作
                *change_low = TRUE;
            }//if
            ++low;
        }//while
        (*L).r[high] = (*L).r[low];                         //将比枢轴记录大的记录移到高端
    }//while(low < high)
    (*L).r[low] = (*L).r[0];                                //枢轴记录到位
    return low;                                             //返回枢轴位置
}//Partition_1

void QSort_1(SqList *L,int low,int high){
    //对顺序表*L中的子序列(*L).r[low..high]作改良版快速排序；
    //在“一次划分”算法中,作如下修改：取更合适的记录为枢轴，并在指针high减1和low增1的同时进行“起泡”操作；
    //算法同时采用尾递归，在一趟排序之后比较所得两部分的长度，先对长度短的子序列中的记录进行快速排序，
    //递归栈的最大深度可降为O(logn)。
    int pivotloc;                                               //pivotloc指示枢轴位置
    bool change_low,change_high;
    while (low < high) {                                        //长度大于1
        change_low = change_high = FALSE;
        pivotloc = Partition_1(L,low,high,&change_low,&change_high);
                                                                //将(*L).r[low..high]一分为二
        if (pivotloc - low <= high - pivotloc) {                //低子表长度比高子表短
            if (change_low) QSort_1(L,low,pivotloc - 1);        //若低子表交换过记录，则先对低子表递归排序
            if (change_high) low = pivotloc + 1;                //若高子表交换过记录，则再对其进行排序
            else break;
        }//if
        else {                                                  //低子表长度比高子表长
            if (change_high) QSort_1(L,pivotloc + 1,high);      //先对高子表递归排序
            if (change_low) high = pivotloc - 1;                //再对低子表进行排序
            else break;
        }//else
    }//while
}//QSort_1

void QuickSort_1(SqList *L){
    //对顺序表*L作改良版快速排序。
    QSort_1(L,1,(*L).length);
}//QuickSort_1

#endif // !SWAPSORT_H
