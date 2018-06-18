/** @workspace/Graph/ShortestPath.h
 *  This file includes Dijkstra algorithme and Floyd algorithme
 *  to get shortest pathes of a directed net.
 *  Also includes algorithmes to print out these pathes.
 */

#ifndef SHORTESTPATH_H
#define SHORTESTPATH_H

#include <stdio.h>
#include "status.h"
#include "MGraph.h"

// - - - - - 最短路径类型定义 - - - - -
typedef Status  PathMatrix;         //到各顶点的最短路径
typedef VRType  ShortPathTable;     //最短路径权值表

void ShortestPath_DIJ(MGraph G,int v0,PathMatrix P[][MAX_VERTAX_NUM + 1],ShortPathTable D[]){
    //用Dijkstra算法求有向网G的v0顶点到其余顶点v的最短路径P[v]及其带权长度D[v]。
    //若P[v][w]为TRUE，则w是从v0到v当前求得最短路径上的顶点。
    //final[v]为TRUE当且仅当v∈S，即已经求得从v0到v的最短路径。
    int v, w;
    int i, j;
    int min;
    Status final[MAX_VERTAX_NUM + 1];       //0号单元弃用
    for (v = 1; v <= G.vexnum; ++v){
        final[v] = FALSE;
        D[v] = G.arcs[v0][v].adj;
        for (w = 1; w <= G.vexnum; ++w) P[v][w] = FALSE;    //设空路径
        if(D[v] != INFINITY) {P[v][v0] = TRUE; P[v][v] = TRUE;}
    }//for
    D[v0] = 0; final[v0] = TRUE;            //初始化，v0顶点属于S集
    //开始主循环，每次求得v0到某个v顶点的最短路径，并加v到S集
    for (i = 1; i < G.vexnum; ++i){         //其余G.vexnum-1个顶点
        min = INFINITY;                     //当前所知离v0顶点的最近距离
        for (w = 1; w <= G.vexnum; ++w)
            if (!final[w] && D[w] < min) {v = w; min = D[w];}   //w顶点在V-S中且离v0顶点最近
        final[v] = TRUE;                    //离v0顶点最近的v加入S集
        for (w = 1; w <= G.vexnum; ++w)     //更新当前最短路径及距离
            if (!final[w] && min < INFINITY && G.arcs[v][w].adj < INFINITY      //注意C语言中整数的溢出★
                                        && (min + G.arcs[v][w].adj < D[w])){    //修改D[w]和P[w]，w∈V-S
                D[w] = G.arcs[v0][v].adj + G.arcs[v][w].adj;
                for (j = 1; j <= G.vexnum; ++j) P[w][j] = P[v][j];
                P[w][w] = TRUE;             //P[w] = P[v] + [w]
            }//if
    }//for
}//ShortestPath_DIJ

void ShortestPath_DIJK(MGraph G,PathMatrix P[][MAX_VERTAX_NUM + 1][MAX_VERTAX_NUM + 1],
                                                ShortPathTable D[][MAX_VERTAX_NUM + 1]){
    //用Dijkstra算法求有向网G的各对顶点v和w之间的最短路径P[v][w]及其带权长度D[v][w]。
    //若P[v][w][u]为TRUE，则u是从v到w当前求得最短路径上的顶点。
    //final[w]为TRUE当且仅当w∈S，即已经求得从v到w的最短路径。
    int v, w, u;
    int i, j;
    int min;
    Status final[MAX_VERTAX_NUM + 1];           //0号单元弃用
    for (v = 1; v <= G.vexnum; ++v)
        for (w = 1; w <= G.vexnum; ++w){
            D[v][w] = G.arcs[v][w].adj;
            for (u = 1; u <= G.vexnum; ++u) P[v][w][u] = FALSE;     //设空路径
                if (D[v][w] != INFINITY)
                    {P[v][w][v] = TRUE; P[v][w][w] = TRUE;}
        }//for
    for (v = 1; v <= G.vexnum; ++v){
        for (i = 1; i <= G.vexnum; ++i) final[i] = FALSE;
        D[v][v] = 0; final[v] = TRUE;           //初始化，v顶点属于S集
        //每次求得v到某个w顶点的最短路径，并加w到S集
        for (i = 1; i < G.vexnum; ++i){         //其余G.vexnum-1个顶点
            min = INFINITY;                     //当前所知离v顶点的最近距离
            for (w = 1; w <= G.vexnum; ++w)
                if (!final[w] && D[v][w] < min) {u = w; min = D[v][w];} //w顶点在V-S中且离v顶点更近
            final[u] = TRUE;                    //离v顶点最近的u加入S集
            for (w = 1; w <= G.vexnum; ++w)     //更新当前最短路径及距离
                if (!final[w] && min < INFINITY && G.arcs[u][w].adj < INFINITY  //注意C语言中整数的溢出★
                            && (min + G.arcs[u][w].adj < D[v][w])){     //修改D[v][w]和P[v][w]，w∈V-S
                    D[v][w] = min + G.arcs[u][w].adj;
                    for (j = 1; j <= G.vexnum; ++j) P[v][w][j] = P[v][u][j];
                    P[v][w][w] = TRUE;          //P[v][w] = P[v][u] + [w]
                }//if
        }//for
    }//for
}//ShortestPath_DIJK

void ShortestPath_FLOYD(MGraph G,PathMatrix P[][MAX_VERTAX_NUM + 1][MAX_VERTAX_NUM + 1],
                                                ShortPathTable D[][MAX_VERTAX_NUM + 1]){
    //用Floyd算法求有向网G中各对顶点v和w之间的最短路径P[v][w]及其带权长度D[v][w]。
    //若P[v][w][u]为TRUE，则u是从v到w当前求得最短路径上的顶点。
    int i, v, w, u;
    for (v = 1; v <= G.vexnum; ++v)     //各对结点之间初始已知路径及距离
        for (w = 1; w <= G.vexnum; ++w){
            D[v][w] = G.arcs[v][w].adj;
            for (u = 1; u <= G.vexnum; ++u) P[v][w][u] = FALSE;
            if (D[v][w] != INFINITY)    //从v到w有直接路径
                {P[v][w][v] = TRUE; P[v][w][w] = TRUE;}
        }//for
    for (u = 1; u <= G.vexnum; ++u)     //（D[v][w]：中间顶点序号不大于u★）
        for (v = 1; v <= G.vexnum; ++v)
            for (w = 1; w <= G.vexnum; ++w)
                if (D[v][u] != INFINITY && D[u][w] != INFINITY  //注意C语言中整数的溢出★
                                && D[v][u] + D[u][w] < D[v][w]){//从v经u到w的一条路径更短
                    D[v][w] = D[v][u] + D[u][w];
                    for (i = 1; i <= G.vexnum; ++i)
                        P[v][w][i] = P[v][u][i] || P[u][w][i];
                }//if
}//ShortestPath_FLOYD

void PrintPath_1(MGraph G,int v0,PathMatrix P[][MAX_VERTAX_NUM + 1],ShortPathTable D[]){
    //打印出v0到其它n-1个顶点的最短路径
    int i, j;
    int cnt;
    int count[G.vexnum + 1];        //存储从v0出发的各条路径上的顶点个数，0号单元弃用
    for (i = 1; i <= G.vexnum; ++i) count[i] = 0;
    for (i = 1; i <= G.vexnum; ++i)
        for (j = 1; j <= G.vexnum; ++j)
            if (P[i][j]) count[i]++;
    for (i = 1; i <= G.vexnum; ++i)
        if (v0 != i){               //从v0到其他顶点，排除自身
            printf("'%c'-->'%c':",G.vexs[v0],G.vexs[i]);
            cnt = 2;
            if(count[i]){           //从v0到i存在路径
                printf("'%c'->",G.vexs[v0]);
                for (j = 1; j <= G.vexnum && cnt < count[i]; ++j)
                //从v0到i的有向路径上，从v0到各顶点依次历经顶点个数为2，3...count[i]★
                    if (P[i][j] && count[j] == cnt)
                        { printf("'%c'->",G.vexs[j]); ++cnt; }
                printf("'%c', w = %d.\n",G.vexs[i],D[i]);
            }//if
            else printf("X\n");     //从v0到i不存在路径
        }//if
}//PrintPath_1

void PrintPath_2(MGraph G,PathMatrix P[][MAX_VERTAX_NUM + 1][MAX_VERTAX_NUM + 1],
                                            ShortPathTable D[][MAX_VERTAX_NUM + 1]){
    //打印出所有顶点间的最短路径
    int i, j, k;
    int cnt;
    int count[G.vexnum + 1][G.vexnum + 1];  //存储各条路径上的顶点个数
    for (i = 1; i <= G.vexnum; ++i)
        for (j = 1; j <= G.vexnum; ++j)
            count[i][j] = 0;
    for (i = 1; i <= G.vexnum; ++i)
        for (j = 1; j <= G.vexnum; ++j)
            for (k = 1; k <= G.vexnum; ++k)
                if (P[i][j][k]) count[i][j]++;
    for (i = 1; i <= G.vexnum; ++i){
        for (j = 1; j <= G.vexnum; ++j)
            if (i != j){                    //排除自身路径
                printf("'%c'-->'%c':",G.vexs[i],G.vexs[j]);
                cnt = 2;
                if(count[i][j]){            //从i到j存在路径
                    printf("'%c'->",G.vexs[i]);
                    for (k = 1; k <= G.vexnum && cnt < count[i][j]; ++k)
                    //从i到j的有向路径上，从i到各顶点依次历经顶点个数为2，3...count[i][j]★
                        if (P[i][j][k] && count[i][k] == cnt)
                            { printf("'%c'->",G.vexs[k]); ++cnt; }
                    printf("'%c', w = %d.\n",G.vexs[j],D[i][j]);
                }//if
                else printf("X\n");         //从i到j不存在路径
            }//if
        printf("\n");
    }//for
}//PrintPath_2

#endif // !SHORTESTPATH_H
