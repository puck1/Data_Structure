
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include "STypedef.h"
#include "../General/status.h"

// - - - - - 开放定址哈希表的存储结构 - - - - -
int hashsize[] = {0,7,13,17,101,211,307,401,503,601,701,809,907,997};
                                    //哈希表容量递增表，一个合适的素数序列（除0外）
typedef struct {
    STElemType  *elem;              //数据元素存储基址，动态分配数组
    int         count;              //当前数据元素个数
    int         sizeindex;          //hashsize[sizeindex]为当前容量
}HashTable;

#define SUCCESS     1
#define UNSUCCESS   0
#define DUPLICATE  -1
#define NULLKEY INT_MAX             //无关键字
#define DELKEY  INT_MIN             //已删除的关键字

// - - - - - 需要调用的函数原型声明 - - - - -
Status InsertHash(HashTable *H,STElemType e);
Status ReCreateHashTable(HashTable *H);
void DestroyHash(HashTable *H);

// - - - - - 哈希函数与冲突处理函数 - - - - -
int Hash(HashTable H,KeyType K){
    //哈希定址函数Hash(K),此处使用简单的除留余数法
    // return K % 13;
    return K % hashsize[H.sizeindex];
}//Hash

void collision(HashTable H,int *p){
    //处理冲突的方法，此处使用简单的线性探测再散列
    //*p指示本次处理冲突后待查元素在H中的位置
    *p = (*p + 1) % hashsize[H.sizeindex];
}//collision

// - - - - - 基本操作的算法实现 - - - - -
void InitHash(HashTable *H){
    //初始化哈希表*H
    (*H).elem = NULL;
    (*H).count = (*H).sizeindex = 0;
}//InitHash

Status CreateHash(HashTable *H){
    //输入哈希表*H的元素个数及所有关键字，创建哈希表*H
    Status s;
    int i , j;
    STElemType e;
    InitHash(H);
    printf("Enter number of elements:");
    scanf("%d%*c",&i);
    for (j = 1; j < sizeof(hashsize) / sizeof(hashsize[0]) && hashsize[j] < i; ++j);//寻找最适容量
    if (j == sizeof(hashsize) / sizeof(hashsize[0]))    //超过hashsize数组下标
        { printf("Excess capacity limit！\n"); return ERROR; }
    (*H).sizeindex = j;
    (*H).elem = (STElemType *)malloc(hashsize[(*H).sizeindex] * sizeof(STElemType));
    if (!(*H).elem) { printf("OVERFLOW!\n"); exit(OVERFLOW); }
    for (j = 0; j < hashsize[(*H).sizeindex]; ++j) (*H).elem[j].key = NULLKEY;
    printf("Enter %d elements' keys seperated by delimiter:",i);
    for (j = 1; j <= i; ++j){
        scanf("%d%*c",&e.key);
        do {
            s = InsertHash(H,e);
            if (s == OVERFLOW) { printf("OVERFLOW!\n"); return ERROR; }
        } while (s == UNSUCCESS);       //返回UNSUCCESS时，重新建立哈希表再插入
    }//for
    printf("Succeeded!\n");
    return OK;
}//CreateHash

Status SearchHash(HashTable H,KeyType K,int *p,int *c){
    //在开放定址哈希表H中查找关键码为K的元素，若查找成功，以p指示待查数据
    //元素在表中位置，并返回SUCCESS；否则，以*p为插入位置，并返回UNSUCCESS，
    //*c用以计冲突次数，其初值置零，供建表插入时参考
    *p = Hash(H,K);                             //求得哈希地址
    while (H.elem[*p].key != NULLKEY &&         //该位置中填有记录
          !EQ(K,H.elem[*p].key) &&              //并且关键字不相等
          *c < hashsize[H.sizeindex] / 2)       //且小于冲突次数阈值
        { ++(*c); collision(H,p); }             //求得下一探查地址p
    if (EQ(K,H.elem[*p].key))
        return SUCCESS;         //查找成功，p返回待查数据元素的位置
    else return UNSUCCESS;      //查找不成功（H.elem[p].key == NULLKEY，p返回的是插入位置；
                                //或*c < hashsize[H.sizeindex] / 2，冲突次数过大）
}//SearchHash


Status InsertHash(HashTable *H,STElemType e){
    //查找不成功时插入数据元素e到开放定址哈希表*H中，并返回OK；若冲突次数
    //过大，则重建哈希表
    int p = 0, c = 0;
    if (SearchHash(*H,e.key,&p,&c))
        return DUPLICATE;                               //表中已有与e有相同关键字的元素
    else if (c < hashsize[(*H).sizeindex] / 2) {        //冲突次数c未达到上限，（c的阈值可调）
        (*H).elem[p] = e; ++(*H).count; return OK;      //插入e
    }//else if
    else {
        if (!ReCreateHashTable(H)) exit(OVERFLOW);      //重建哈希表
        return UNSUCCESS;
    }//else
}//InsertHash

Status ReCreateHashTable(HashTable *H){
    //重建哈希表*H
    int i, j, cnt;
    Status s;
    HashTable new;
    i = (*H).sizeindex;
    j = sizeof(hashsize) / sizeof(hashsize[0]);
    if (i + 1 == j) return ERROR;       //超过hashsize数组下标
    new.elem = (STElemType *)malloc(hashsize[i + 1] * sizeof(STElemType));  //扩大容量
    if (!new.elem) exit(ERROR);
    new.sizeindex = i + 1; new.count = 0;
    for (j = 0; j < hashsize[new.sizeindex]; ++j) new.elem[j].key = NULLKEY;
    for (j = 0, cnt = 0; cnt < (*H).count; ++j)
        if ((*H).elem[j].key != NULLKEY && (*H).elem[j].key != DELKEY) {
            do {
                s = InsertHash(&new,(*H).elem[j]);
                if (s == OVERFLOW) return ERROR;
            } while (s == UNSUCCESS);
            ++cnt;
        }//if
    DestroyHash(H); *H = new;
    return OK;
}//ReCreateHashTable

Status DeleteHash(HashTable *H,KeyType K){
    //在哈希表*H中查找关键字等于K的记录，若找到，将其删除并返回TRUE，否则FALSE
    int p = 0, c = 0;
    if (SearchHash(*H,K,&p,&c))     //找到待删记录
        { (*H).elem[p].key = DELKEY; --(*H).count; return TRUE; }
        //在该记录的位置上填入删除符号，以免找不到在它之后填入的“同义词”的记录★
    else return FALSE;
}//DeleteHash

void DestroyHash(HashTable *H){
    //销毁哈希表*H，*H不再存在
    free((*H).elem); (*H).elem = NULL;
    (*H).count = 0; (*H).sizeindex = 0;
}//DestroyHash

Status TraverseHash(HashTable H,Status Visit(STElemType)){
    //对哈希表H中的每个元素按照下标先后依次调用函数Visit()，
    //一旦Visit()失败，则函数失败
    int i, cnt;
    for (i = 0, cnt = 0; cnt < H.count; ++i)
        if (H.elem[i].key != NULLKEY && H.elem[i].key != DELKEY) {
            if (!Visit(H.elem[i])) return ERROR;
            else ++cnt;
        }//if
    return OK;
}//TraverseHash

#endif // !HASHTABLE_H
