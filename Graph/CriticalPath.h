/** @workspace/Graph/CriticalPath.h
 *  This file includes a algorithme to get critical activities of a directed net.
 *  Also includes a algorithme realized by DFS.
 */

#ifndef CRITICALPATH_H
#define CRITICALPATH_H

#include <stdio.h>
#include "status.h"
typedef int InfoType;           //使用*info存储弧的权值
#include "ALGraph.h"
typedef int SElemType;
#include "../Stack/SqStack.h"

// - - - - - 需要使用的全局变量 - - - - -
int ve[MAX_VERTEX_NUM + 1];     //顶点事件最早发生时间（0号单元弃用）
int vl[MAX_VERTEX_NUM + 1];     //顶点事件最晚发生时间（0号单元弃用）

// - - - - - 需要调用的函数原型声明 - - - - -
void FindInDegree(ALGraph G,int indegree[]);
void DFS_CriticalPath(ALGraph G,int i);

// - - - - - 基本操作的算法描述 - - - - -
Status TopologicalOrder(ALGraph G,SqStack *T){
    //有向网G采用邻接表存储结构（边的权值存储于info中），求各顶点事件的最早发生时间ve（全局变量）。
    //T为拓扑序列顶点栈，S为零入度顶点栈。
    //若G无回路，则用栈T返回G的一个拓扑序列，且函数值为OK，否则为ERROR。
    int i, j;
    int count;
    ArcNode *p;
    SqStack S;
    int indegree[G.vexnum + 1];             //0号单元弃用
    FindInDegree(G,indegree);               //对各顶点求入度indegree[1..vexnum]
    InitStack(&S); InitStack(T);
    for (i = 1; i <= G.vexnum; ++i){
        if(indegree[i] == 0) Push(&S,i);    //建零入度顶点栈S
        ve[i] = 0;                          //初始化
    }
    count = 0;
    while (!StackEmpty(S)){
        Pop(&S,&i); Push(T,i);  ++count;    //i号顶点入T栈并计数
        for (p = G.vertices[i].firstarc; p; p = p->nextarc){
            j = p->adjvex;                  //对i号顶点的每个邻接点的入度减1
            if (--indegree[j] == 0) Push(&S,j);     //若入度减为0，则入栈
            if (ve[i] + *(p->info) > ve[j]) ve[j] = ve[i] + *(p->info);
        }//for  *(p->info) = dut(<i,j>)
    }//while
    if (count < G.vexnum) {     //有向网有回路
        printf("Loop exists! Cannot get a critical path!\n");
        return ERROR;
    }
    else return OK;
}//TopologicalOrder

Status CriticalPath(ALGraph G){
    //G为有向网，输出G的各项关键活动。
    int i, j;
    int ee, el, dut;
    char tag;
    ArcNode *p;
    SqStack T;
    if (!TopologicalOrder(G,&T)) return ERROR;
    for (i = 1; i <= G.vexnum; ++i) vl[i] = ve[G.vexnum];       //初始化顶点事件的最迟发生时间
    while (!StackEmpty(T))                  //按拓扑逆序求各顶点的vl值
        for (Pop(&T,&i), p = G.vertices[i].firstarc; p; p = p->nextarc){
            j = p->adjvex;  dut = *(p->info);       //dut<i,j>
            if (vl[j] - dut < vl[i]) vl[i] = vl[j] - dut;
        }//for
    for (i = 1; i <= G.vexnum; ++i)             //求ee，el和关键活动
        for (p = G.vertices[i].firstarc; p; p = p->nextarc){
            j = p->adjvex;  dut = *(p->info);
            ee = ve[i]; el = vl[j] - dut;
            tag = (ee == el) ? '*' : '\0';
            printf("'%c' -> '%c', dut = %2d, ee = %2d, el = %2d, %c\n",
                    G.vertices[i].data,G.vertices[j].data,dut,ee,el,tag);   //输出关键活动
        }//for
    return OK;
}//CriticalPath

Status CriticalPath_DFS(ALGraph G){
    //利用退出DFS函数的逆拓扑序列，输出有向网G的各项关键活动。
    int i, j;
    int ee, el, dut;
    char tag;
    ArcNode *p;
    SqStack T;
    if (!TopologicalOrder(G,&T)) return ERROR;      //拓扑排序也可用DFS来实现
    for (i = 1; i <= G.vexnum; ++i) vl[i] = ve[G.vexnum];       //初始化顶点事件的最迟发生时间
    for (i = 1; i <= G.vexnum; ++i) visited[i] = FALSE;         //访问标志数组初始化
    for (i = 1; i <= G.vexnum; ++i)
        if (!visited[i]) DFS_CriticalPath(G,i);     //此时网中无环，利用退出DFS的逆拓扑序列求vl
    for (i = 1; i <= G.vexnum; ++i)                 //求ee，el和关键活动
        for (p = G.vertices[i].firstarc; p; p = p->nextarc){
            j = p->adjvex;  dut = *(p->info);
            ee = ve[i]; el = vl[j] - dut;
            tag = (ee == el) ? '*' : '\0';
            printf("'%c' -> '%c', dut = %2d, ee = %2d, el = %2d, %c\n",
                    G.vertices[i].data,G.vertices[j].data,dut,ee,el,tag);   //输出关键活动
        }//for
    return OK;
}//CriticalPath_DFS

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

void DFS_CriticalPath(ALGraph G,int i){
    //从第i个顶点出发递归地深度优先遍历图G，并计算顶点i的vl值
    int j;
    int dut;
    ArcNode *p;
    visited[i] = TRUE;
    for (j = FirstAdjVex_i(G,i); j != 0; j = NextAdjVex_i(G,i,j))
        if(!visited[j]) DFS_CriticalPath(G,j);
    for (p = G.vertices[i].firstarc; p; p = p->nextarc){
        j = p->adjvex; dut = *(p->info);
        if (vl[j] - dut < vl[i]) vl[i] = vl[j] - dut;       //此时i的所有直接后继的vl值都已求出
    }
}//DFS_CriticalPath

#endif // !CRITICALPATH_H
