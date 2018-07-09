
#ifndef BTREE_H
#define BTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>           //提供ceil():向上取整函数
#include "STypedef.h"
#include "../General/status.h"

// - - - - - B-树结构类型定义 - - - - -
#define m 3                                 //B-树的阶，暂设为3
typedef struct BTNode{
    int             keynum;                 //结点中关键字个数，即结点的大小
    struct BTNode   *parent;                //指向双亲结点
    KeyType         key[m + 1];             //关键字向量，0号单元未用
    struct BTNode   *ptr[m + 1];            //子树指针向量
    Record          *recptr[m + 1];         //记录指针向量，0号单元未用
}BTNode, *BTree;                            //B-树结点和B-树的类型
typedef struct {
    BTNode   *pt;                           //指向找到的结点
    int         i;                          //1..m，在结点中的关键字序号
    int         tag;                        //1：查找成功，0：查找失败
}Result;                                    //B-树的查找结果类型

typedef BTNode* QElemType;
#include "../Queue/LinkQueue.h"

// - - - - - 需要调用的函数声明 - - - - -
int search(BTNode *p,KeyType K);
Status InsertKey(BTree *T,BTNode *q,int i,KeyType K);
void InsertRight(BTNode *q,int i,KeyType K,BTNode *ap);
void InsertLeft(BTNode *q,int i,BTNode *ap,KeyType K);
Status Split(BTNode *q,int s,BTNode **ap);
Status NewRoot(BTree *T,BTNode *q,KeyType K,BTNode *ap);
void DeleteKey(BTree *T,BTNode *q,int i);
void DeleteRight(BTNode *q,int i);
void DeleteLeft(BTNode *q,int i);
void Move(BTNode *q,BTNode *si,int up,int down,int LR);
void Merge(BTNode *q,BTNode *si,int down,int LR);

// - - - - - 基本操作的算法实现 - - - - -
void InitBTree(BTree *T){
    //构造一个空的B-树*T
    (*T) = NULL;
}//InitBTree

void DestroyBTree(BTree *T){
    //销毁B-树T
    int i;
    if (*T){
        for (i = 0; i <= (*T)->keynum; ++i){
            if ((*T)->ptr[i]) DestroyBTree((*T)->ptr);
            if (i && (*T)->recptr[i]) free((*T)->recptr);
        }//for
        free(*T); *T = NULL;
    }//if
}//DestroyBTree

Result SearchBTree(BTree T,KeyType K){
    //在m阶B-树T上查找关键字K，返回结果(pt,i,tag)。若查找成功，则特征值tag == 1，
    //指针pt所指结点中第i个关键字等于K；否则特征值tag == 0，等于K的关键字应插入在
    //指针pt所指结点中第i和第i + 1个关键字之间
    int i; bool found; Result r;
    BTNode *p, *q;
    p = T; q = NULL; found = FALSE; i = 0;  //初始化，p指向待查结点，q指向p的双亲
    while (p && !found){
        i = search(p,K);                    //在p->key[1..keynum]中查找，
                                            //i使得：p->key[i] <= K < p->key[i + 1]
        if (i > 0 && p->key[i] == K) found = TRUE;  //找到待查关键字
        else { q = p; p = p->ptr[i]; }
    }//while
    r.i = i;
    if (found) { r.pt = p; r.tag = 1; }     //查找成功
    else { r.pt = q; r.tag = 0; }           //查找不成功，返回K的插入信息位置
    return r;
}//SearchBTree

int search(BTNode *p,KeyType K){
    //在p->key[1..keynum]中查找，返回i使得：p->key[i] <= K < p->key[i + 1]
    int i;
    for (i = 0; i < p->keynum; ++i)
        if (LT(K,p->key[i + 1])) break;
    return i;
}//search

Status InsertBTree(BTree *T,KeyType K){
    //若m阶B-树*T上不存在包括关键字K的结点，则在适当的位置插入关键字K，依情况调整B-树，
    //并返回TRUE，否则FALSE。
    Result r;
    r = SearchBTree(*T,K);
    if (r.tag == 0) {
        if (InsertKey(T,r.pt,r.i,K)) return TRUE;
        else return ERROR;      //未成功插入
    }//if
    else return FALSE;          //已存在关键字K
}//InsertBTree

Status InsertKey(BTree *T,BTNode *q,int i,KeyType K){
    //在m阶B-树*T上结点*q的key[i]与key[i + 1]之间插入关键字K。
    //若引起结点过大，则沿双亲链进行必要的结点分裂调整，使*T仍是m阶B-树。
    KeyType x; BTNode *ap; bool finished; int s;
    x = K; ap = NULL; finished = FALSE;
    while (q && !finished){
        InsertRight(q,i,x,ap);      //将x和ap分别插入到q->key[i + 1]和q->ptr[i + 1]
        if (q->keynum < m) finished = TRUE;     //插入完成
        else {                                  //分裂结点*q
            s = ceil((double)m / 2); x = q->key[s]; //s = ┌m/2┐
            if (!Split(q,s,&ap)) return ERROR;
            //将q->key[s + 1..m],q->ptr[s..m]和q->recptr[s + 1..m]移入新结点*ap
            q = q->parent;
            if (q) i = search(q,x);             //在双亲结点*q中查找x的插入位置
        }//else
    }//while
    if (!finished)          //T是空树（参数q初值为NULL）或者根结点已分裂为结点*q和*ap
        if (!NewRoot(T,q,x,ap)) return ERROR;   //生成含信息(*T,x,ap)的新根结点*T，原*T和ap为子树指针
    return OK;
}//InsertKey

void InsertRight(BTNode *q,int i,KeyType K,BTNode *ap){
    //将关键字K和指针ap分别插入到q->key[i + 1]和q->ptr[i + 1]
    int j;
    for (j = q->keynum; j > i; --j){
        q->key[j + 1] = q->key[j];
        q->ptr[j + 1] = q->ptr[j];
        q->recptr[j + 1] = q->recptr[j];
    }//for
    q->key[i + 1] = K; q->ptr[i + 1] = ap; q->recptr[i + 1] = NULL;
    if (ap) ap->parent = q;
    ++q->keynum;
}//InsertRight

void InsertLeft(BTNode *q,int i,BTNode *ap,KeyType K){
    //将指针ap和关键字K分别插入到q->ptr[i]和q->key[i + 1]
    int j;
    for (j = q->keynum; j > i; --j){
        q->key[j + 1] = q->key[j];
        q->ptr[j + 1] = q->ptr[j];
        q->recptr[j + 1] = q->recptr[j];
    }//for
    q->ptr[i + 1] = q->ptr[i];
    q->key[i + 1] = K; q->ptr[i] = ap; q->recptr[i + 1] = NULL;
    if (ap) ap->parent = q;
    ++q->keynum;
}//InsertLeft

Status Split(BTNode *q,int s,BTNode **ap){
    //将q->key[s + 1..m],q->ptr[s..m]和q->recptr[s + 1..m]移入新结点*ap
    int i;
    if (!(*ap = (BTNode*)malloc(sizeof(BTNode)))) exit(OVERFLOW);
    (*ap)->keynum = m - s; (*ap)->parent = q->parent;
    (*ap)->ptr[0] = q->ptr[s];
    for (i = 1; i <= m - s; ++i){
        (*ap)->key[i] = q->key[s + i];
        (*ap)->ptr[i] = q->ptr[s + i];
        (*ap)->recptr[i] = q->recptr[s + i];
    }//for
    for (i = 0; i <= m - s; ++i)
        if ((*ap)->ptr[i]) (*ap)->ptr[i]->parent = (*ap);   //★
    for (i = m - s + 1; i <= m; ++i){
        (*ap)->key[i] = INT_MAX;
        (*ap)->ptr[i] = (*ap)->recptr[i] = NULL;
    }//for
    for (i = s; i <= m; ++i){
        q->key[i] = INT_MAX;
        q->ptr[i] = q->recptr[i] = NULL;
    }//for
    q->keynum = s - 1;
    return OK;
}//Split

Status NewRoot(BTree *T,BTNode *q,KeyType K,BTNode *ap){
    //生成含信息(*T,x,ap)的新根结点*T，原*T和ap为子树指针
    //本算法完成后，指针*T指向新的根结点
    int i;
    BTree p;
    if (!(p = (BTree)malloc(sizeof(BTNode)))) exit(OVERFLOW);
    p->keynum = 1; p->parent = NULL;
    p->key[1] = K; p->ptr[0] = *T; p->ptr[1] = ap;
    p->recptr[1] = NULL;
    if (p->ptr[0]) p->ptr[0]->parent = p;
    if (p->ptr[1]) p->ptr[1]->parent = p;
    for (i = 2; i <= m; i++){
        p->key[i] = INT_MAX;
        p->ptr[i] = p->recptr[i] = NULL;
    }//for
    *T = p;
    return OK;
}//NewRoot

Status DeleteBTree(BTree *T,KeyType K){
    //若m阶B-树*T上存在包括关键字K的结点，删除该结点中的关键字K，
    //依情况调整B-树，并返回TRUE，否则FALSE。
    Result r;
    BTNode *q;
    r = SearchBTree(*T,K);
    if (r.tag == 1) {
        q = r.pt->ptr[r.i];
        if (q) {
            while (q->ptr[0]) q = q->ptr[0];    //q指向r.pt->ptr[i]所指子树中有最小关键字的结点
            r.pt->key[r.i] = q->key[1];         //用q中最小关键字q->key[1]替代r.pt->ptr[i]
            DeleteKey(T,q,1);                   //在相应结点中删去q->key[1]
        }//if
        else DeleteKey(T,r.pt,r.i);
        return TRUE;
    }//if
    else return FALSE;
}//DeleteBTree

void DeleteKey(BTree *T,BTNode *q,int i){
    //在m阶B-树*T上结点*q中删除第i个关键字，并依情况调整B-树
    BTNode *pa, *si;
    int s, j;
    int up, down;
    int LR; bool finished;
    pa = q->parent;
    s = ceil((double)m / 2);            // s = ┌m/2┐;
    if (!pa && q->keynum == 1)          //只有一个关键字的根结点
        { *T = q->ptr[0]; free(q); }
    else if (!pa || q->keynum >= s)     //*q结点中关键字数目不小于2（根结点）或者┌m/2┐
        DeleteRight(q,i);               //直接删除*q结点中的key[i]与ptr[i]
    else {
        LR = 0; finished = FALSE;
        j = search(pa,q->key[i]);       //pa->key[j] <= q->key[i] < pa->key[j + 1]
        if (j != pa->keynum){           //*q右兄弟结点存在
            LR = 1;
            si = pa->ptr[j + 1]; down = j + 1; up = 1;
            if (si->keynum > s - 1){    //右兄弟结点中的关键字数目大于┌m/2┐ - 1
                DeleteRight(q,i);
                Move(q,si,up,down,LR);
                //LR指示兄弟结点与*q相对位置，down指示双亲中下移关键字位置，up指示兄弟结点上移关键字位置
                finished = TRUE;
            }
        }//if (j != pa->keynum)
        if (j != 0){                    //*q左兄弟结点存在
            si = pa->ptr[j - 1]; down = j; up = si->keynum;
            if (si->keynum > s - 1){    //左兄弟结点中的关键字数目大于┌m/2┐ - 1
                DeleteRight(q,i);
                Move(q,si,up,down,LR);
                finished = TRUE;
            }
        }//else if (j != 0)
        if (!finished) {                //被删关键字所在结点和其相邻的兄弟结点中的关键字数目均等于┌m/2┐ - 1
            switch (LR) {
                case 1:                 //存在右兄弟，优先对右兄弟进行处理
                    si = pa->ptr[j + 1]; down = j + 1; break;
                case 0:
                    si = pa->ptr[j - 1]; down = j; break;
            }//switch
            DeleteRight(q,i);
            Merge(q,si,down,LR);
            //*q中剩余的关键字和指针，与双亲结点中关键字key[down]一起合并到L/R兄弟结点中
            if (pa->keynum < s - 1){    //合并后双亲结点中关键字数目小于┌m/2┐ - 1
                pa->key[++pa->keynum] = si->key[1];
                //构造一个待删除的虚拟关键字（如si->key[1]），插入到最右端依次类推进行相应处理★
                DeleteKey(T,pa,pa->keynum);
            }//if
        }//if (!finished)
    }//else
}//DeleteKey

void DeleteRight(BTNode *q,int i){
    //在结点*q中删除q->key[i]与q->ptr[i]
    int j;
    for (j = i; j <= q->keynum; ++j){
        q->key[j] = q->key[j + 1];
        q->ptr[j] = q->ptr[j + 1];
        q->recptr[j] = q->recptr[j + 1];
    }//for
    --q->keynum;
}//DeleteRight

void DeleteLeft(BTNode *q,int i){
    //在结点*q中删除q->key[i]与q->ptr[i - 1]
    int j;
    for (j = i; j <= q->keynum; ++j){
        q->key[j] = q->key[j + 1];
        q->ptr[j - 1] = q->ptr[j];
        q->recptr[j] = q->recptr[j + 1];
    }//for
    q->ptr[q->keynum] = NULL;
    --q->keynum;
}//DeleteLeft

void Move(BTNode *q,BTNode *si,int up,int down,int LR){
    //做关键字的“移动”操作，将*q兄弟结点中最小（或最大）关键字上移至双亲结点中，
    //而将双亲结点中小于（或大于）且仅靠该上移关键字的关键字下移至被删关键字所在结点*q中
    //up指示上移关键字的位置，down指示下移关键字位置，LR指示兄弟结点与*q相对位置
    KeyType k = si->key[up];
    switch (LR){
        case 0:
            InsertLeft(q,0,si->ptr[up],q->parent->key[down]);
            DeleteRight(si,up);
            break;
        case 1:
            InsertRight(q,q->keynum,q->parent->key[down],si->ptr[0]);
            DeleteLeft(si,up);
            break;
    }//switch
    q->parent->key[down] = k;
}//Move

void Merge(BTNode *q,BTNode *si,int down,int LR){
    //做结点的“合并”操作，将删去关键字后的结点*q中剩余的关键字和指针，
    //加上双亲结点中的关键字（仅比被删结点中最大关键字大或比最小关键字小）一起，
    //合并到其兄弟结点中，down指示双亲结点下移关键字位置,LR指示兄弟结点与*q相对位置
    int k;
    switch (LR){
        case 0:
            InsertRight(si,si->keynum,q->parent->key[down],q->ptr[0]);
            for (k = 1; k <= q->keynum; ++k)
                InsertRight(si,si->keynum,q->key[k],q->ptr[k]);
            DeleteRight(q->parent,down);
            break;
        case 1:
            InsertLeft(si,0,q->ptr[0],q->parent->key[down]);
            for (k = q->keynum; k >= 1; --k)
                InsertRight(si,0,q->key[k],q->ptr[k]);
            DeleteLeft(q->parent,down);
            break;
    }//switch
    free(q);
}//Merge

Status InorderTraverseBTree(BTree T,Status Visit(KeyType)){
    //中序遍历B-树T，得到一个关键字的有序序列
    int i;
    if (T){
        for (i = 0; i < T->keynum; ++i){
            if (!InorderTraverseBTree(T->ptr[i],Visit)) return ERROR;
            if (!Visit(T->key[i + 1])) return ERROR;
        }//for
        if (!InorderTraverseBTree(T->ptr[T->keynum],Visit)) return ERROR;
    }//if
    return OK;
}//InorderTraverseBTree

Status LevelOrderTraverseBTree(BTree T,Status Visit(KeyType)){
    //层序遍历B-树T，得到一个关键字的序列
    int i, j, lv, num, cnt;
    LinkQueue Q;
    BTNode *q;
    InitQueue(&Q);
    lv = num = cnt = 0;
    if (T) { EnQueue(&Q,T); num = 1; }
    while (!QueueEmpty(Q)){
        printf("Level %2d:",++lv);
        for (i = 1; i <= num; i++){
            DeQueue(&Q,&q);
            printf("(");
            for (j = 0; j < q->keynum; ++j){
                if (q->ptr[j]) { EnQueue(&Q,q->ptr[j]); ++cnt; }
                if (!Visit(q->key[j + 1])) return ERROR;
                if (j != q->keynum - 1) printf(",");
            }//for
            if (q->ptr[q->keynum]) { EnQueue(&Q,q->ptr[q->keynum]); ++cnt; }
            printf(")");
            if (i != num) printf(",");
        }//for
        printf("\n");
        num = cnt; cnt = 0;
    }//while
    return OK;
}//LevelOrderTraverseBTree

void CreateBTree(BTree *T){
    //输入关键字序列，建立一棵B-树
    int i, n;
    KeyType K;
    InitBTree(T);
    printf("Enter number of keywords:"); scanf("%d%*c",&n);
    printf("Enter all keyword values seperated by delimiter:");
    for (i = 1; i <= n; ++i){
        scanf("%d%*c",&K);
        InsertBTree(T,K);
    }//for
    printf("Succeeded!\n");
}//CreateBTree

#endif // !BTREE_H
