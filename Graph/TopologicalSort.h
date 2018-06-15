/** @workspace/Graph/TopologicalSort.h
 *  This file includes a algorithme to judge if
 *  there is a topological order of a directed graph.
 *  If there is one,print it.
 *  Also includes a algorithme realized by DFS.
 */

#ifndef TOPOLOGICALSORT_H
#define TOPOLOGICALSORT_H

#include <stdio.h>
#include "status.h"
#include "ALGraph.h"
typedef int SElemType;
#include "../Stack/SqStack.h"

// - - - - - 需要使用的全局变量 - - - - -
int count;
int finished[MAX_VERTEX_NUM + 1];       //记录完成搜索的顶点号，0号单元弃用
int path[MAX_VERTEX_NUM + 1];           //记录DFS连通分量中的所有顶点序列，0号单元弃用

// - - - - - 需要调用的函数原型声明 - - - - -
void FindInDegree(ALGraph G,int indegree[]);
Status DFS_Topo(ALGraph G,int i,int order);

// - - - - - 基本操作的算法描述 - - - - -
Status TopologicalSort(ALGraph G){
    //有向图G采用邻接表存储结构。
    //若G无回路，则输出G的顶点的一个拓扑序列并返回OK，否则ERROR
    int i, j;
    int count;
    ArcNode *p;
    SqStack S;
    int indegree[G.vexnum + 1];         //0号单元弃用
    FindInDegree(G,indegree);           //对各顶点求入度indegree[1..vexnum]
    InitStack(&S);
    for (i = 1; i <= G.vexnum; ++i)     //建零入度顶点栈S
        if (!indegree[i]) Push(&S,i);   //入度为0者进栈
    count = 0;                          //对输出顶点计数
    while (!StackEmpty(S)){
        Pop(&S,&i);
        printf("%c ",G.vertices[i].data); ++count;  //输出顶点并计数
        for (p = G.vertices[i].firstarc; p; p = p->nextarc){
            j = p->adjvex;              //对i号顶点的每个邻接点的入度减1
            if(!(--indegree[j])) Push(&S,j);        //若入度减为0，则入栈
        }//for
    }//while
    if (count < G.vexnum){              //该有向图有回路
        printf("\nLoop exists!\n");
        return ERROR;
    }
    printf("\nSucceeded!\n");
    return OK;
}//TopologicalSort

void FindInDegree(ALGraph G,int indegree[]){
    //对图G的各顶点求入度
    int i;
    ArcNode *p;
    for (i = 1; i <= G.vexnum; ++i)
        indegree[i] = 0;    //初始化
    for (i = 1; i <= G.vexnum; ++i)
        for (p = G.vertices[i].firstarc; p; p = p->nextarc)
            ++indegree[p->adjvex];
}//FindInDegree

Status TopologicalSort_DFS(ALGraph G){
    //利用深度优先遍历对有向图G进行拓扑排序
    int i;
    int order;
    count = 0;
    for (i = 1; i <= G.vexnum; ++i) visited[i] = FALSE; //访问标志数组初始化
    for (i = 1; i <= G.vexnum; ++i)
        if (!visited[i]){
            order = 1;
            path[order] = i;            //路径起始点
            if (!DFS_Topo(G,i,order))
                { printf("Loop exists!\n"); return ERROR; } //存在环路
        }//if
    for (i = G.vexnum; i >= 1; --i)     //按退出DFS函数的先后次序记录的序列为逆拓扑序列
        printf("%c ",G.vertices[finished[i]].data);
    printf("\nSucceeded!\n"); return OK;
}//TopologicalSort_DFS

Status DFS_Topo(ALGraph G,int i,int order){
    //从第i个顶点出发递归地深度优先遍历图G，将完成搜索的顶点号记录在辅助数组finished中
    //order指示顶点i在DFS连通分量中的路径次序编号★
    int j, k;
    visited[i] = TRUE;
    for (j = FirstAdjVex_i(G,i); j != 0; j = NextAdjVex_i(G,i,j)){
        for (k = order + 1; k <= G.vexnum; ++k) path[k] = 0;    //清理后面路径
        if(!visited[j]){            //加入路径，继续DFS
            path[order + 1] = j;
            if (!DFS_Topo(G,j,order + 1)) return ERROR;
        }
        else for (k = 1; k <= order; ++k)
            if (j == path[k]) return ERROR;     //j的邻接点在本次DFS的路径上,必存在环路
    }//for
    finished[++count] = i;          //按退出DFS_Topo函数的顺序将顶点连接起来
    return OK;
}//DFS_Topo

#endif // !TOPOLOGICALSORT_H
