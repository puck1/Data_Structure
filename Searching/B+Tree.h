
#ifndef BPTREE_H
#define BPTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>           //提供ceil():向上取整函数
#include "STypedef.h"
#include "../General/status.h"

// - - - - - B+树结构类型定义 - - - - -
#define m 3                                 //B+树的阶，暂设为3
typedef struct BPTNode{
    int             keynum;                 //结点中关键字个数，即结点的大小
    bool            leaf;                   //leaf == 1:叶子结点；leaf == 0：非终端结点
    struct BPTNode  *parent;                //指向双亲结点
    union {
        struct BPTNode *next;               //指向叶子结点的直接后继结点
    }lf;
    KeyType         key[m + 1];             //关键字向量，使用0号单元
    struct BPTNode  *ptr[m + 1];            //子树指针向量，使用0号单元
    Record          *recptr[m + 1];         //记录指针向量，使用0号单元
}BPTNode;                                   //B+树结点类型
typedef struct {
    BPTNode *root;                          //指向根结点的头指针
    BPTNode *sqt;                           //指向关键字最小的叶子结点的头指针
}BPTree;                                    //B+树类型
typedef struct {
    BPTNode  *pt;                           //指向找到的结点
    int         i;                          //0..m - 1，在结点中的关键字序号
    int         tag;                        //1：查找成功，0：查找失败
}Result;                                    //B+树的查找结果类型

typedef BPTNode* QElemType;
#include "../Queue/LinkQueue.h"

// - - - - - 需要调用的函数声明 - - - - -
void DestroyBPTNode(BPTNode *p);
int search(BPTNode *p,KeyType K);
Status InsertKey(BPTree *T,BPTNode *q,int i,KeyType K);
void Insert(BPTNode *q,int i,KeyType K,BPTNode *ap);
Status Split(BPTNode *q,int s,BPTNode **ap);
Status NewRoot(BPTree *T,BPTNode *q,KeyType K,BPTNode *ap);
void DeleteKey(BPTree *T,BPTNode *q,int i);
void Adjust(BPTNode *q,int i,KeyType K);
void Delete(BPTNode *q,int i);
Status InorderTraverse(BPTNode *q,Status Visit(KeyType));

// - - - - - 基本操作的算法实现 - - - - -
void InitBPTree(BPTree *T){
    //构造一个空的B+树*T
    (*T).root = (*T).sqt = NULL;
}//InitBPTree

void DestroyBPTree(BPTree *T){
    //销毁B+树*T
    DestroyBPTNode((*T).root);
    (*T).root = (*T).sqt = NULL;
}//DestroyBPTree

void DestroyBPTNode(BPTNode *p){
    //销毁B+树结点*p及其所有子树结点
    int i;
    if (p){
        for (i = 0; i < p->keynum; ++i){
            if (p->ptr[i]) DestroyBPTNode(p->ptr[i]);
            if (p->recptr[i]) free(p->recptr[i]);
        }//for
        free(p);
    }//if
}//DestroyBPTNode

Result RandomSearchBPTree(BPTree T,KeyType K){
    //在m阶B+树T上随机查找关键字K，返回结果(pt,i,tag)。若查找成功，则特征值tag == 1，
    //指针pt所指结点中第i个关键字等于K；否则特征值tag == 0，等于K的关键字应插入在
    //指针pt所指结点中第i - 1和第i个关键字之间
    int i; bool found; Result r;
    BPTNode *p;
        p = T.root; found = FALSE; i = 0;               //初始化，p指向待查结点
    while (p && !found){
        i = search(p,K);                                //在p->key[0..keynum - 1]中查找，
                                                        //i使得：p->key[i - 1] < K <= p->key[i]
        if (i < p->keynum && p->key[i] == K) found = TRUE;
                                                        //找到待查关键字
        else if (p->leaf) break;                        //未找到
        else p = (i == p->keynum ? p->ptr[--i] : p->ptr[i]);
    }//while
    if (found) {                //查找成功
        if (!p->leaf) {         //非终端结点
            p = p->ptr[i];
            while (!p->leaf) p = p->ptr[p->keynum - 1];
            i = p->keynum - 1;
        }//if
        r.tag = 1;
    }//if (found)
    else r.tag = 0;             //查找不成功
    r.i = i; r.pt = p;
    return r;
}//RandomSearchBPTree

int search(BPTNode *p,KeyType K){
    //在p->key[0..keynum - 1]中查找，返回i使得：p->key[i - 1] < K <= p->key[i]
    int i;
    for (i = 0; i < p->keynum; ++i)
        if (LQ(K,p->key[i])) break;
    return i;
}//search

Status InsertBPTree(BPTree *T,KeyType K){
    //若m阶B+树*T上不存在包括关键字K的结点，则在适当的位置插入关键字K，依情况调整B+树，
    //并返回TRUE，否则FALSE。
    Result r;
    r = RandomSearchBPTree(*T,K);
    if (r.tag == 0) {
        if (InsertKey(T,r.pt,r.i,K)) return TRUE;
        else return ERROR;      //未成功插入
    }//if
    else return FALSE;          //已存在关键字K
}//InsertBPTree

Status InsertKey(BPTree *T,BPTNode *q,int i,KeyType K){
    //在m阶B+树*T上结点*q的key[i - 1]与key[i]之间插入关键字K。
    //若引起结点过大，则沿双亲链进行必要的结点分裂调整，使*T仍是m阶B+树。
    KeyType x, y; BPTNode *ap; bool finished; int s;
    x = K; ap = NULL; finished = FALSE;
    while (q && !finished){
        Insert(q,i,x,ap);       //将x和ap分别插入到q->key[i]和q->ptr[i]
        if (q->keynum <= m) finished = TRUE;    //插入完成
        else {                                  //分裂结点*q
            s = ceil((double)(m + 1) / 2);      //s = ┌(m + 1)/2┐
            x = q->key[q->keynum - 1]; y = q->key[s - 1];
            if (!Split(q,s,&ap)) return ERROR;
            //将q->key[s..m],q->ptr[s..m]和q->recptr[s..m]移入新结点*ap
            q = q->parent;
            if (q)
               { i = search(q,x); q->key[i] = y; ++i;}
               //双亲结点中*q结点最大值转换为y，并在其后插入x与ap
        }//else
    }//while
    if (!finished)          //T是空树（参数q初值为NULL）或者根结点已分裂为结点*q和*ap
        if (!NewRoot(T,q,x,ap)) return ERROR;   //生成含信息(T.root,x,ap)的新根结点，原root和ap为子树指针
    return OK;
}//InsertKey

void Insert(BPTNode *q,int i,KeyType K,BPTNode *ap){
    //将关键字K和指针ap分别插入到q->key[i]和q->ptr[i]
    int j;
    BPTNode *p;
    for (j = q->keynum - 1; j >= i; --j){
        q->key[j + 1] = q->key[j];
        q->ptr[j + 1] = q->ptr[j];
        q->recptr[j + 1] = q->recptr[j];
    }//for
    q->key[i] = K; q->ptr[i] = ap; q->recptr[i] = NULL;
    if (ap) ap->parent = q;
    p = q->parent;
    if (i == q->keynum && p) Adjust(p,search(p,q->key[i - 1]),K);   //K作为最大关键字插入
    ++q->keynum;
}//InsertRight

Status Split(BPTNode *q,int s,BPTNode **ap){
    //将q->key[s..m],q->ptr[s..m]和q->recptr[s..m]移入新结点*ap
    int i;
    if (!(*ap = (BPTNode*)malloc(sizeof(BPTNode)))) exit(OVERFLOW);
    (*ap)->keynum = m - s + 1; (*ap)->parent = q->parent;
    if (q->leaf) {
        (*ap)->leaf = 1;
        (*ap)->lf.next = q->lf.next; q->lf.next = *ap;
    }//if
    else (*ap)->leaf = 0;
    for (i = 0; i <= m - s; ++i){
        (*ap)->key[i] = q->key[s + i];
        (*ap)->ptr[i] = q->ptr[s + i];
        (*ap)->recptr[i] = q->recptr[s + i];
        if ((*ap)->ptr[i]) (*ap)->ptr[i]->parent = (*ap);   //★
    }//for
    for (; i <= m; ++i){
        (*ap)->key[i] = INT_MAX;
        (*ap)->ptr[i] = (*ap)->recptr[i] = NULL;
    }//for
    for (i = s; i <= m; ++i){
        q->key[i] = INT_MAX;
        q->ptr[i] = q->recptr[i] = NULL;
    }//for
    q->keynum = s;
    return OK;
}//Split

Status NewRoot(BPTree *T,BPTNode *q,KeyType K,BPTNode *ap){
    //生成含信息(T.root,x,ap)的新根结点，原root和ap为子树指针
    int i;
    BPTNode *p, *r;
    r = T->root;
    if (!(p = (BPTNode *)malloc(sizeof(BPTNode)))) exit(OVERFLOW);
    p->keynum = 1; p->parent = NULL;
    p->key[0] = K; p->ptr[0] = ap; p->recptr[0] = NULL;
    for (i = 1; i <= m; ++i){
        p->key[i] = INT_MAX;
        p->ptr[i] = p->recptr[i] = NULL;
    }//for
    if (r) {
        Insert(p,0,r->key[r->keynum - 1],r);
        p->leaf = 0;
    }//if
    else {      //原B+树为空树
        p->leaf = 1; p->lf.next = NULL;
        T->sqt = p;
    }//else
    if (ap) ap->parent = p;
    T->root = p;
    return OK;
}//NewRoot

Status DeleteBPTree(BPTree *T,KeyType K){
    //若m阶B+树*T上存在包括关键字K的结点，删除该结点中的关键字K，
    //依情况调整B+树，并返回TRUE，否则FALSE。
    Result r;
    r = RandomSearchBPTree(*T,K);
    if (r.tag == 1) {
        DeleteKey(T,r.pt,r.i);
        return TRUE;
    }//if
    else return FALSE;
}//DeleteBPTree

void DeleteKey(BPTree *T,BPTNode *q,int i){
    //在m阶B+树*T上结点*q中删除第i个关键字，并依情况调整B+树
    BPTNode *pa, *si;
    int s, j, k;
    int LR; bool finished;
    pa = q->parent;
    s = ceil((double)m / 2);                // s = ┌m/2┐;
    if (!pa && q->keynum == 1){             //只有一个关键字的根结点
        if (q->recptr[0]) free(q->recptr[0]);
        T->root = T->sqt = NULL; free(q);
    }//if
    else if (!pa || q->keynum >= s){        //*q结点中关键字数目不小于2（根结点）或者┌m/2┐
        if (i == q->keynum - 1 && pa)       //待删除的为结点中最大关键字
            Adjust(pa,search(pa,q->key[i]),q->key[i - 1]);
        Delete(q,i);                        //直接删除*q结点中的key[i]与ptr[i]
    }//esle if
    else {
        LR = 0; finished = FALSE;
        j = search(pa,q->key[i]);           //pa->key[j] <= q->key[i] < pa->key[j + 1]
        if (j != pa->keynum - 1){           //*q右兄弟结点存在
            LR = 1;
            si = pa->ptr[j + 1];
            if (si->keynum > s - 1) {
                Delete(q,i);
                Insert(q,q->keynum,si->key[0],si->ptr[0]);
                pa->key[j] = si->key[0];
                Delete(si,0);
            }//if (si->keynum > s - 1)
        }//if (j != pa->keynum)
        if (j != 0){                        //*q左兄弟结点存在
            si = pa->ptr[j - 1];
            if (si->keynum > s - 1) {
                if (i == q->keynum - 1){    //待删除的为结点中最大关键字
                    if (i > 0)
                        Adjust(pa,j,q->key[i - 1]);
                    else
                        Adjust(pa,j,si->key[si->keynum - 1]);
                }//if (i == q->keynum - 1)
                Delete(q,i);
                Insert(q,0,si->key[si->keynum - 1],si->ptr[si->keynum - 1]);
                Delete(si,si->keynum - 1);
                pa->key[j - 1] = si->key[si->keynum - 1];   //兄弟结点中最大关键字变动
            }//if (si->keynum > s - 1)
        }//if (j != 0)
        if (!finished) {                //被删关键字所在结点和其相邻的兄弟结点中的关键字数目均等于┌m/2┐ - 1
            switch (LR) {
                case 1:                 //存在右兄弟，优先对右兄弟进行处理
                    Delete(q,i);
                    for (k = 0; k < q->keynum; ++k) Insert(si,0,q->key[k],q->ptr[k]);
                    DeleteKey(T,pa,j);
                    if (q == T->sqt) T->sqt = q->lf.next;
                    free(q);
                    break;
                case 0:
                    Delete(q,i);
                    for (k = 0; k < q->keynum; ++k) Insert(si,si->keynum,q->key[k],q->ptr[k]);
                    pa->key[j - 1] = si->key[si->keynum - 1];
                    DeleteKey(T,pa,j);
            }//switch
        }//if (!finished)
    }//else
}//DeleteKey

void Adjust(BPTNode *q,int i,KeyType K){
    //将*q结点中第i个关键字更改为K，并沿双亲链对B+树进行调整
    int j;
    KeyType old;
    BPTNode *p;
    old = q->key[i]; q->key[i] = K;
    p = q->parent;
    if (p && i == q->keynum - 1) {
        j = search(p,old);
        Adjust(p,j,K);
    }//if
}//Adjust

void Delete(BPTNode *q,int i){
    //在结点*q中删除q->key[i]与q->ptr[i]
    int j;
    for (j = i; j <= q->keynum; ++j){
        q->key[j] = q->key[j + 1];
        q->ptr[j] = q->ptr[j + 1];
        q->recptr[j] = q->recptr[j + 1];
    }//for
    --q->keynum;
}//Delete

Status InorderTraverseBPTree(BPTree T,Status Visit(KeyType)){
    //中序遍历B+树T的非终端结点，得到一个关键字的有序序列
    if (!InorderTraverse(T.root,Visit)) return ERROR;
    else return OK;
}//InorderTraverseBPTree

Status InorderTraverse(BPTNode *q,Status Visit(KeyType)){
    //中序遍历B+树结点*q及其子树上的非终端结点
    int i;
    if (q)
        for (i = 0; i < q->keynum; ++i){
            if (!InorderTraverse(q->ptr[i],Visit)) return ERROR;
            if (q->leaf && !Visit(q->key[i])) return ERROR;
        }//for
    return OK;
}//InorderTraverse

Status SeqTraverseBPTree(BPTree T,Status Visit(KeyType)){
    //顺序遍历B+树的非终端结点
    int i;
    BPTNode *q = T.sqt;
    for (; q; q = q->lf.next)
        for (i = 0; i < q->keynum; ++i)
            if (!Visit(q->key[i])) return ERROR;
    return OK;
}//SeqTraverseBPTree

Status LevelOrderTraverseBPTree(BPTree T,Status Visit(KeyType)){
    //层序遍历B+树T，得到一个关键字的序列
    int i, j, lv, num, cnt;
    LinkQueue Q;
    BPTNode *q;
    bool isleaf;
    InitQueue(&Q);
    lv = num = cnt = 0;
    isleaf = FALSE;
    if (T.root) { EnQueue(&Q,T.root); num = 1; }
    while (!QueueEmpty(Q)){
        if (isleaf) printf("Leaf Level:");
        else printf("Level %2d:",++lv);
        for (i = 1; i <= num; i++){
            DeQueue(&Q,&q);
            printf("(");
            for (j = 0; j < q->keynum; ++j){
                if (q->ptr[j]) {
                    if (q->ptr[j]->leaf) isleaf = TRUE;
                    EnQueue(&Q,q->ptr[j]); ++cnt;
                }//if
                if (!Visit(q->key[j])) return ERROR;
                if (j != q->keynum - 1) printf(",");
            }//for
            printf(")");
            if (i != num) printf(",");
        }//for
        printf("\n");
        num = cnt; cnt = 0;
    }//while
    return OK;
}//LevelOrderTraverseBPTree

void CreateBPTree(BPTree *T){
    //输入关键字序列，建立一棵B+树
    int i, n;
    KeyType K;
    InitBPTree(T);
    printf("Enter number of keywords:"); scanf("%d%*c",&n);
    printf("Enter all keyword values seperated by delimiter:");
    for (i = 1; i <= n; ++i){
        scanf("%d%*c",&K);
        InsertBPTree(T,K);
    }//for
    printf("Succeeded!\n");
}//CreateBPTree

#endif // !BPTREE_H
