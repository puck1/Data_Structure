
#ifndef MFSET_H
#define MFSET_H

#include "status.h"
typedef SetElemType TElemType;
#include "ParentTree.h"

// - - - - - ADT MFSet(并查集)的树的双亲表存储表示 - - - - -
typedef PTree MFSet;
#define MAX_SET_SIZE (MAX_TREE_SIZE - 1)    //树0号结点单元弃用

typedef struct {
    int x, y;
}BiRelation, ElemType;      //二元关系
#include "../Linear_List/ExtendedLinkList.h"
typedef LinkList RSet;      //链表作为等价关系集合

// - - - - - 基本操作的算法描述 - - - - -
Status Initial_MFSet(MFSet *S,int n,SetElemType x[n + 1]){
    //初始化操作，构造一个由n个子集x[n + 1]（每个子集只含单个成员）构成的集合S。
    //子集数组0号单元弃用。
    int i;
    if(n < 1 || n > MAX_SET_SIZE) return ERROR;
    for (i = 1; i <= n; ++i){
        (*S).nodes[i].data = x[i];
        (*S).nodes[i].parent = -1;
    }
    (*S).n = n; (*S).r = -1;
    return OK;
}//Initial_mfset

int Find_MFSet(MFSet S,int i){
    //找集合S中i所在子集的根。
    int j;
    if(i < 1 || i > S.n) return -1;     //i不属S中任一子集
    for (j = i; S.nodes[j].parent > 0; j = S.nodes[j].parent);
    return j;
}//find_mfset

Status Merge_MFSet(MFSet *S,int i,int j){
    //S.nodes[i]和S.nodes[j]分别为S的互不相交的两个子集Si和Sj的根结点。
    //求并集Si∪Sj。
    if(i < 1 || i > (*S).n || j < 1 || j > (*S).n) return ERROR;
    if(i != j) (*S).nodes[i].parent = j;    //将集合Si并入Sj中
    return OK;
}//merge_mfset

Status Mix_MFSet(MFSet *S,int i,int j){
    //函数Merge_mfset的改进，减少生成树的深度。
    //使含成员少的子集并入含成员多的子集，
    //根结点的parent域存储子集中所含成员数目的负值。
    if (i < 1 || i > (*S).n || j < 1 || j > (*S).n)
        return ERROR;
    if ((*S).nodes[i].parent > (*S).nodes[j].parent){
        //Si所含成员数比Sj少
        (*S).nodes[j].parent += (*S).nodes[i].parent;
        (*S).nodes[i].parent = j;
    }else if (i != j){      //i、j的双亲域相同时需保证i与j不相等★
        (*S).nodes[i].parent += (*S).nodes[j].parent;
        (*S).nodes[j].parent = i;
    }
    return OK;
}//Mix_mfset

int Fix_MFSet(MFSet *S,int i){
    //函数Find_MFSet的改进，减少生成树的深度。
    //确定i所在子集，并将从i至根路径上所有结点都变成根的孩子结点。
    int j, k, t;
    if(i < 1 || i > (*S).n) return -1;  //i不是S中任一子集的成员
    for (j = i; (*S).nodes[j].parent > 0; j = (*S).nodes[j].parent);
    for (k = i; k != j; k = t){         //“压缩路径”
        t = (*S).nodes[k].parent; (*S).nodes[k].parent = j;
    }
    return j;
}//Fix_MFSet

Status BuildRSet(RSet *rs,int rn){
    //建立等价关系集合rs，rn为二元关系数目
    int i;
    BiRelation r;
    InitList(rs);
    Link p = rs->head; Link q;
    if(rn < 0) return ERROR;
    printf("Enter each binary relationship seperated by delimiter:\n");
    for (i = 0; i < rn; ++i){
        scanf("%d%*c%d%*c",&r.x,&r.y);
        if(!MakeNode(&q,r) || !InsAfter(rs,&p,q)) return ERROR;
    }
    printf("Succeeded!\n");
    return OK;
}//BuildRSet

Status EquivalenceClass_1(MFSet *S,RSet rs){
    //根据函数Find、Merge求在等价关系rs下的等价类
    int i, j;
    Link p = rs.head->next;
    while (p){
        i = Find_MFSet(*S,p->data.x);
        j = Find_MFSet(*S,p->data.y);
        if(!Merge_MFSet(S,i,j)) return ERROR;   //Si与Sj是否相等的判定在Merge中进行
        p = p->next;
    }
    return OK;
}//EquivalenceClass_1

Status EquivalenceClass_2(MFSet *S,RSet rs){
    //根据函数Mix、Fix求在等价关系rs下的等价类
    int i, j;
    Link p = rs.head->next;
    while (p){
        i = Fix_MFSet(S,p->data.x);
        j = Fix_MFSet(S,p->data.y);
        if(!Mix_MFSet(S,i,j)) return ERROR;     //Si与Sj是否相等的判定在Mix中进行
        p = p->next;
    }
    return OK;
}//EquivalenceClass_2

int SubSetCount(MFSet S){
    //确定S中等价类的数目
    int i;
    int count = 0;
    for (i = 1; i <= S.n; ++i)
        if(S.nodes[i].parent < 0) count++;
    return count;
}

void PrintSet(MFSet S){
    //打印集合S的所有等价类
    int i, j;
    int root[S.n + 1];          //root[i]为1则i为根结点
    int tmp[S.n + 1][S.n + 1];  //tmp[i][j]为1则j是i的子孙
    int cnt = 0;
    int scnt = SubSetCount(S);
    for (i = 1; i <= S.n; ++i)
        for (j = 1; j <= S.n; ++j)
            {tmp[i][j] = 0; root[j] = 0;}
    for (i = 1; i <= S.n; ++i){
        j = Find_MFSet(S,i);
        if(i != j) tmp[j][i] = 1;
        root[j] = 1;
    }
    printf("All equivalence class:");
    for (i = 1; i <= S.n; ++i)
        if (root[i] == 1){
            printf("{%c",S.nodes[i].data);
            for (j = 1; j <= S.n; ++j){
                if(tmp[i][j] == 1)
                    printf(",%c",S.nodes[j].data);
            }//for
            printf("}");
            ++cnt;
            if(cnt < scnt) printf(",");
            else printf("\n");
        }//if
}//PrintSet

#endif // !MFSET_H
