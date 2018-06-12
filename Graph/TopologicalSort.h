
#ifndef TOPOLOGICALSORT_H
#define TOPOLOGICALSORT_H

#include <stdio.h>
#include "status.h"
#include "ALGraph.h"
typedef int SElemType;
#include "../Stack/SqStack.h"

// - - - - - 需要调用的函数原型声明 - - - - -
void FindInDegree(ALGraph G,int indegree[]);

// - - - - - 基本操作的算法描述 - - - - -
Status TopologicalSort(ALGraph G){
    //有向图G采用邻接表存储结构。
    //若G无回路，则输出G的顶点的一个拓扑序列并返回OK，否则ERROR
    int i, j;
    int count;
    ArcNode *p;
    SqStack S;
    int indegree[G.vexnum + 1];         //0号单元弃用
    FindInDegree(G,indegree);           //对各顶点求入度indegree[0..vexnum-1]
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

#endif // !TOPOLOGICALSORT_H
