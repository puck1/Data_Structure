/** @workspace/Graph/SCC.h
 *  This file includes Kosaraju algorithme to get
 *  strongly connected compontents of a directed graph.
 */

#ifndef SCC_H
#define SCC_H

#include <stdio.h>
#include "OLGraph.h"

// - - - - - 需要使用的全局变量 - - - - -
int count;
int finished[MAX_VERTEX_NUM + 1];       //记录完成搜索的顶点号

// - - - - - 需要调用的函数原型声明 - - - - -
void InverseOLGraph(OLGraph *G);
void DFSTraverse_SCC_1(OLGraph G);
void DFSTraverse_SCC_2(OLGraph G);
void DFS_SCC_1(OLGraph G,int i);
void DFS_SCC_2(OLGraph G,int i);

// - - - - - 基本操作的算法描述 - - - - -
void InverseOLGraph(OLGraph *G){
    //逆置有向图G
    int i, j;
    ArcBox *p, *q;
    for (i = 1; i <= (*G).vexnum; ++i){
        for (p = (*G).xlist[i].firstin; p; p = q){      //将每个入弧结点各项逆转
            j = p->headvex;
            p->headvex = p->tailvex; p->tailvex = j;
            q = p->hlink;
            p->hlink = p->tlink; p->tlink = q;
        }
        q = (*G).xlist[i].firstin;
        (*G).xlist[i].firstin = (*G).xlist[i].firstout; //顶点的入弧域与出弧域互换
        (*G).xlist[i].firstout = q;
    }
}//InverseOLGraph

void DFSTraverse_SCC_1(OLGraph G){
    //对图G进行深度优先遍历
    int i;
    count = 0;
    for (i = 1; i <= G.vexnum; ++i) visited[i] = FALSE; //访问标志数组初始化
    for (i = 1; i <= G.vexnum; ++i)
        if(!visited[i]) DFS_SCC_1(G,i);
}//DFSTraverse_SCC_1

void DFS_SCC_1(OLGraph G,int i){
    //从第i个顶点出发递归地深度优先遍历图G，将完成搜索的顶点号记录在辅助数组finished中
    int j;
    visited[i] = TRUE;
    for (j = FirstAdjVex_i(G,i); j != 0; j = NextAdjVex_i(G,i,j))
        if(!visited[j]) DFS_SCC_1(G,j);
    finished[++count] = i;          //按退出DFS_SCC_1函数的顺序将顶点连接起来★
}//DFS_SCC_1

void DFSTraverse_SCC_2(OLGraph G){
    //对图G进行深度优先遍历，得到其全部强连通分量
    int i;
    for (i = 1; i <= G.vexnum; ++i) visited[i] = FALSE; //访问标志数组初始化
    for (i = G.vexnum; i >= 1; --i)
        if(!visited[finished[i]]) {DFS_SCC_2(G,finished[i]); printf("\n");}
                                    //每次调用DFS_SCC_2可以得到一个强连通分量★
}//DFSTraverse_SCC_2

void DFS_SCC_2(OLGraph G,int i){
    //从第i个顶点出发递归地深度优先遍历图G，得到一个强连通分量
    int j;
    visited[i] = TRUE; printf("'%c' ",G.xlist[i].data);
    for (j = FirstAdjVex_i(G,i); j != 0; j = NextAdjVex_i(G,i,j))
        if(!visited[j]) DFS_SCC_2(G,j);
}//DFS_SCC_2

void SCC_Kosaraju(OLGraph G){
    //求有向图强连通分量的Kosaraju算法
    DFSTraverse_SCC_1(G);
    InverseOLGraph(&G);
    printf("Strongly connected components of directed graph G:\n");
    DFSTraverse_SCC_2(G);
}//SCC_Kosaraju

#endif // !SCC_H
