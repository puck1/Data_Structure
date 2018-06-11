/** @workspace/Graph/UDGSpanningTree.h
 *  This file includes algorithmes to get a spanning tree
 *  or spanning forest of an undirected graph.
 */

#ifndef UDGSpanningTree_H
#define UDGSpanningTree_H

#include <stdlib.h>
#include "status.h"
#include "ALGraph.h"
#include "..\Tree\Child_SiblingTree.h"

// - - - - - 需要调用的函数原型声明 - - - - -
void DFSTree(ALGraph G,int i,CSTree *T);

// - - - - - 基本操作的算法描述 - - - - -
void DFSForest(ALGraph G,CSTree *T){
    //建立无向图G(以邻接表存储)的深度优先生成森林的(最左)孩子(右)兄弟链表T。
    int i;
    CSNode *p, *q;
    InitTree(T);
    for (i = 1; i <= G.vexnum; ++i) visited[i] = FALSE;
    for (i = 1; i <= G.vexnum; ++i)
        if(!visited[i]){                            //第i顶点为新的生成树的根节点
            p = (CSNode *)malloc(sizeof(CSNode));   //分配根节点
            p->data = GetVex(G,i);
            p->firstchild = p->nextsibling = NULL;
            if(!(*T)) *T = p;                       //是第一棵生成树的根（T的根）
            else q->nextsibling = p;            //是其他生成树的根（前一棵的根的“兄弟”）★
            q = p;                                  //q指示当前建立好的生成树的根
            DFSTree(G,i,&p);                        //建立以p为根的生成树
        }
}//DFSForest

void DFSTree(ALGraph G,int i,CSTree *T){
    //从第i个顶点出发深度优先遍历图G，建立以T为根的生成树
    int j;
    bool first;
    CSNode *p ,*q;
    visited[i] = TRUE; first = TRUE;
    for (j = FirstAdjVex_i(G,i); j != 0; j = NextAdjVex_i(G,i,j))
        if(!visited[j]){
            p = (CSNode *)malloc(sizeof(CSNode));       //分配孩子结点
            p->data = GetVex(G,j);
            p->firstchild = p->nextsibling = NULL;
            if(first){                                  //j是v的第一个未被访问的邻接顶点
                (*T)->firstchild = p; first = FALSE;    //是根的左孩子结点
            }
            else q->nextsibling = p;                    //是上一邻接顶点的右兄弟结点
            q = p;
            DFSTree(G,j,&q);    //从第j个顶点出发深度优先遍历图G，建立子生成树q
        }//if
}//DFSTree

void BFSForest(ALGraph G,CSTree *T){
    //建立无向图G(以邻接表存储)的广度优先生成森林的(最左)孩子(右)兄弟链表T。
    int i, j;
    QElemType k;
    LinkQueue Q; InitQueue(&Q);         //置空的辅助队列Q

    bool first;
    CSNode *p, *q, *r;
    CSNode *tptr[MAX_VERTEX_NUM];       //记录图中顶点序号对应的树结点指针★
    InitTree(T);

    for (i = 1; i <= G.vexnum; ++i) visited[i] = FALSE;
    for (i = 1; i <= G.vexnum; ++i)
        if(!visited[i]){                //第i顶点为新的生成树的根节点
            p = (CSNode *)malloc(sizeof(CSNode));
            p->data = GetVex(G,i);
            p->firstchild = p->nextsibling = NULL;
            if(!(*T)) *T = p;           //是第一棵生成树的根（T的根）
            else q->nextsibling = p;    //是其他生成树的根（前一棵的根的“兄弟”）
            q = p;                      //q指示当前建立好的生成树的根
            visited[i] = TRUE; tptr[i] = p;
            EnQueue(&Q,i);              //i入队列
            while (!QueueEmpty(Q)){
                DeQueue(&Q,&k);         //队头元素出队并置为k
                first = TRUE;
                for (j = FirstAdjVex_i(G,k); j != 0; j = NextAdjVex_i(G,k,j))
                    if(!visited[j]){
                        p = (CSNode *)malloc(sizeof(CSNode));   //分配孩子结点
                        p->data = GetVex(G,j);
                        p->firstchild = p->nextsibling = NULL;
                        if(first){
                            tptr[k]->firstchild = p; first = FALSE;     //是根的左孩子结点
                        }
                        else r->nextsibling = p;                //是上一邻接顶点的右兄弟结点
                        r = p;
                        visited[j] = TRUE; tptr[j] = p;
                        EnQueue(&Q,j);                          //j入队列
                    }//if
            }//while
        }//if
    DestroyQueue(&Q);
}//BFSForest

#endif // !UDGSpanningTree_H
