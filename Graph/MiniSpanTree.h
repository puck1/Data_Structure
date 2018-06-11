/** @workspace/Graph/MiniSpanTree.h
 *  This file includes Prim algorithme and Kruskal algorithme
 *  to get a minimum cost spanning tree of an undirected net.
 */

#ifndef MINISPANTREE_H
#define MINISPANTREE_H

#include <stdio.h>
#include "MGraph.h"
typedef VertexType SetElemType;
#include "../Tree/MFSet.h"

// - - - - - Prim算法需要使用的结构定义 - - - - -
typedef struct {
    VertexType  adjvex;             //存储该边依附的在U中的顶点
    VRType      lowcost;            //存储该边上的权
}Edge;                              //辅助数组类型

// - - - - - 需要调用的函数原型声明 - - - - -
int minimum_Prim(Edge closedge[],int n);
void minimum_Kruskal(AdjMatrix arcs,int n,int *i,int *j);

// - - - - - 基本操作的算法描述 - - - - -
void MiniSpanTree_Prim(MGraph G,VertexType u){
    //用Prim算法从顶点u出发构造网G的最小生成树T，输出T的各条边
    //辅助数组closedge记录从顶点集U到V-U的代价最小的边
    Edge closedge[MAX_VERTAX_NUM + 1];  //0号单元弃用
    int i = LocateVex(G,u);
    int j, k;
    for (j = 1; j <= G.vexnum; ++j)     //辅助数组初始化
        if(j != i){
            closedge[j].adjvex = u;
            closedge[j].lowcost = G.arcs[i][j].adj;
        }
    closedge[i].lowcost = 0;            //初始，U = {u}
    for (j = 1; j < G.vexnum; ++j){     //选择其余G.vexnum-1个顶点
        i = minimum_Prim(closedge,G.vexnum);    //求出T的下一个顶点：第i顶点
        // 此时closedge[i].lowcost =
        //          MIN{ closedge[Vi].lowcost | closedge[Vi].lowcost > 0, Vi ∈ V-U}
        printf("'%c'--'%c',cost = %d;\n",closedge[i].adjvex,G.vexs[i],closedge[i].lowcost);
                                        //输出生成树的边及其权值
        closedge[i].lowcost = 0;        //第i顶点并入U集
        for (k = 1; k <= G.vexnum; ++k)
            if(G.arcs[i][k].adj < closedge[k].lowcost){     //新顶点并入U后重新选择最小边
                closedge[k].adjvex = G.vexs[i];
                closedge[k].lowcost = G.arcs[i][k].adj;
            }//for
    }//for
}//MiniSpanTree_Prim

int minimum_Prim(Edge closedge[],int n){
    //选择从顶点集U到V-U的代价最小的边，输出其在V-U中顶点的序号
    int i, j;
    int min = INT_MAX;      //★
    for (i = 1; i <= n; ++i)
        if(closedge[i].lowcost != 0 && closedge[i].lowcost < min)
            { min = closedge[i].lowcost; j = i;}
    return j;
}//minimum

void MiniSpanTree_Kruskal(MGraph G){
    //用Kruskal算法构造无向网G的最小生成树T，输出T的各条边
    //辅助数组es记录网中各边的顶点及其权值
    int i, j, x, y;
    MFSet S;
    AdjMatrix tmp;
    Initial_MFSet(&S,G.vexnum,G.vexs);      //初始化并查集
    for (i = 1; i <= G.vexnum; ++i)         //存储邻接矩阵中所有不重复的边
        for (j = 1; j <= G.vexnum; ++j)
            if(G.arcs[i][j].adj != INFINITY){
                tmp[i][j].adj = G.arcs[i][j].adj;
                tmp[j][i].adj = INFINITY;
            }
            else tmp[i][j].adj = INFINITY;
    while (SubSetCount(S) > 1){         //直至T中所有顶点都在同一连通分量上
        minimum_Kruskal(tmp,G.vexnum,&i,&j);    //求代价最小的边
        x = Find_MFSet(S,i);
        y = Find_MFSet(S,j);
        if(x != y) {                    //边的顶点不在同一连通分量上
            Merge_MFSet(&S,x,y);        //合并连通分量
            printf("'%c'--'%c',cost = %d;\n",G.vexs[j],G.vexs[i],tmp[i][j].adj);
        }                               //输出生成树的边及其权值
        tmp[i][j].adj = INFINITY;       //标记已经查找过的边
    }
}//MiniSpanTree_Kruskal

void minimum_Kruskal(AdjMatrix arcs,int n,int *i,int *j){
    //从含有n个顶点的邻接矩阵arcs中选择代价最小的边，输出其两顶点序号
    int a, b;
    int min = INT_MAX;
    for (a = 1; a <= n; ++a)
        for (b = 1; b <= n; ++b)
            if(arcs[a][b].adj < min)
                { min = arcs[a][b].adj; *i = a; *j = b;}
}//minimum

#endif // !MINISPANTREE_H
