
#ifndef ALGRAPH_H
#define ALGRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "status.h"
#include "..\General\string_p.h"    //读取不定长字符串

// - - - - - 图的邻接表(Adjacency List)存储表示 - - - - -
typedef char    VertexType;         //存储数据类型定为char
typedef char*   InfoType;
typedef enum {DG,UDG} GraphKind;    //{有向图，无向图}
#define MAX_VERTEX_NUM  20
typedef struct ArcNode{
    int              adjvex;        //该弧所指向的顶点的位置
    struct ArcNode  *nextarc;       //指向下一条弧的指针
    InfoType        *info;          //该弧相关信息的指针
}ArcNode;                           //表结点
typedef struct VNode{
    VertexType   data;              //顶点信息
    ArcNode     *firstarc;          //指向第一条依附该顶点的弧的指针
}VNode,AdjList[MAX_VERTEX_NUM + 1]; //头结点,0号单元弃用
typedef struct {
    AdjList     vertices;
    int         vexnum,arcnum;      //图的当前顶点数和弧数
    GraphKind   kind;               //图的种类标志
}ALGraph;

// - - - - - 需要调用的函数原型声明 - - - - -
int LocateVex(ALGraph G,VertexType u);

// - - - - - 基本操作的算法描述 - - - - -
Status CreateDG(ALGraph *DG){
    //采用邻接表表示法，构造有向图DG
    int i,j,k;
    int IncInfo;
    ArcNode *a1,*a2;
    VertexType v1,v2;
    printf("Enter number of DG's vertex,arc and determine "
            "whether to include information(0/1):");
    scanf("%d%*c%d%*c%d%*c",&(*DG).vexnum,&(*DG).arcnum,&IncInfo);  //IncInfo为0则各弧不包含其他信息
    printf("Enter the datas of all %d vertex(es) separated by delimitador:",(*DG).vexnum);
    for (k = 1; k <= (*DG).vexnum; ++k){        //建立头结点
        scanf("%c%*c",&(*DG).vertices[k].data);
        (*DG).vertices[k].firstarc = NULL;
    }
    printf("Enter each arc's tail and head separated by delimitador:\n");
    for (k = 1; k <= (*DG).arcnum; ++k){        //建立表结点
        printf("No.%d:",k);
        scanf("%c%*c%c%*c",&v1,&v2);            //输入一条弧依附的顶点
        i = LocateVex(*DG,v1); j = LocateVex(*DG,v2);   //确定v1和v2在DG中位置
        if(!i || !j) return ERROR;
        a1 = (*DG).vertices[i].firstarc;
        while (a1 && a1->nextarc) a1 = a1->nextarc;
        a2 = (ArcNode *)malloc(sizeof(ArcNode));        //建立弧<v1,v2>
        if(!a2) exit(OVERFLOW);
        a2->adjvex = j; a2->nextarc = NULL;
        if(IncInfo){                            //若弧含有相关信息，则输入
            printf("Enter information (No longer than 20 chars):");
            a2->info = (InfoType *)malloc(sizeof(InfoType));
            if(!a2->info) exit(OVERFLOW);
            *(a2->info) = getstr();             //input information
        }
        else a2->info = NULL;
        if(a1) a1->nextarc = a2;
        else (*DG).vertices[i].firstarc = a2;   //是第一条弧
    }//for
    printf("Succeeded!\n");
    return OK;
}//CreateDG

Status CreateUDG(ALGraph *UDG){
    //采用邻接表表示法，构造无向图UDG
    int i,j,k;
    int IncInfo;
    ArcNode *a1,*a2,*a3;
    VertexType v1,v2;
    printf("Enter number of UDG's vertex,arc and determine "
            "whether to include information(0/1):");
    scanf("%d%*c%d%*c%d%*c",&(*UDG).vexnum,&(*UDG).arcnum,&IncInfo);  //IncInfo为0则各弧不包含其他信息
    printf("Enter the datas of all %d vertex(es) separated by delimitador:",(*UDG).vexnum);
    for (k = 1; k <= (*UDG).vexnum; ++k){        //建立头结点
        scanf("%c%*c",&(*UDG).vertices[k].data);
        (*UDG).vertices[k].firstarc = NULL;
    }
    printf("Enter each arc's tail and head separated by delimitador:\n");
    for (k = 1; k <= (*UDG).arcnum; ++k){       //建立表结点
        printf("No.%d:",k);
        scanf("%c%*c%c%*c",&v1,&v2);            //输入一条边依附的顶点
        i = LocateVex(*UDG,v1); j = LocateVex(*UDG,v2); //确定v1和v2在UDG中位置
        if(!i || !j) return ERROR;
        a1 = (*UDG).vertices[i].firstarc;
        while (a1 && a1->nextarc) a1 = a1->nextarc;
        a2 = (ArcNode *)malloc(sizeof(ArcNode));        //建立边<v1,v2>
        if(!a2) exit(OVERFLOW);
        a2->adjvex = j; a2->nextarc = NULL;
        if(IncInfo){                            //若边含有相关信息，则输入
            printf("Enter information (No longer than 20 chars):");
            a2->info = (InfoType *)malloc(sizeof(InfoType));
            if(!a2->info) exit(OVERFLOW);
            *(a2->info) = getstr();             //input information
        }
        else a2->info = NULL;
        if(a1) a1->nextarc = a2;
        else (*UDG).vertices[i].firstarc = a2;      //是第一条边

        a1 = (*UDG).vertices[j].firstarc;
        while (a1 && a1->nextarc) a1 = a1->nextarc;
        a3 = (ArcNode *)malloc(sizeof(ArcNode));    //建立边<v2,v1>
        if(!a3) exit(OVERFLOW);
        a3->adjvex = i; a3->nextarc = NULL; a3->info = a2->info;
        if(a1) a1->nextarc = a3;
        else (*UDG).vertices[j].firstarc = a3;      //是第一条边
    }//for
    printf("Succeeded!\n");
    return OK;
}//CreateUDG

Status CreateGraph(ALGraph *G){
    //采用邻接表表示法，构造图G
    printf("Enter the kind of graph G (0->DG/1->UDG):");
    scanf("%d",&(*G).kind);
    switch ((*G).kind){
        case  DG: return CreateDG(G);
        case UDG: return CreateUDG(G);
        default:  return ERROR;
    }
}//CreateGraph

Status DestroyGraph(ALGraph *G){
    //销毁图G
    int i;
    ArcNode *p,*q,*r;
    for (i = 1; i <= (*G).vexnum; ++i){
        p = (*G).vertices[i].firstarc;
        while (p){
            q = p->nextarc;
            if(p->info){        //清空弧信息
                free(p->info);
                if((*G).kind == UDG){   //无向图清除对称信息防止出错
                    r = (*G).vertices[p->adjvex].firstarc;
                    while (r->adjvex != i) r = r->nextarc;
                    r->info = NULL;
                }//if
            }//if
            free(p);
            p = q;
        }//while
    }//for
    (*G).vexnum = (*G).arcnum = 0;
    return OK;
}//DestroyGraph

int LocateVex(ALGraph G,VertexType u){
    //u和图G中顶点有相同特征，若图G中存在顶点u，则返回该顶点在图中位置，否则返回“空”
    for (int i = 1; i <= G.vexnum; ++i)
        if(G.vertices[i].data == u) return i;
    return 0;
}//LocateVex

VertexType GetVex(ALGraph G,int i){
    //i是图G中某个顶点的位置，返回i的值
    if(i < 1 || i > G.vexnum) return ' ';
    else return G.vertices[i].data;
}//GetVex

Status PutVex(ALGraph *G,VertexType v,VertexType value){
    //v是图G中某个顶点，对v赋值valve
    int i = LocateVex(*G,v);
    if(i == 0) return ERROR;
    else{
        (*G).vertices[i].data = value;
        return OK;
    }
}//PutVex

VertexType FirstAdjVex(ALGraph G,VertexType v){
    //v是图G中某个顶点，返回v的第一个邻接顶点，若顶点在G中没有邻接顶点，则返回“空”
    ArcNode *p;
    int i = LocateVex(G,v);
    if(i != 0){
        p = G.vertices[i].firstarc;
        if(p) return G.vertices[p->adjvex].data;
    }
    return ' ';
}//FirstAdjVex

VertexType NextAdjVex(ALGraph G,VertexType v,VertexType w){
    //v是图G中某个顶点，w是v的邻接顶点，返回v的（相对于w的）下一个邻接顶点；
    //若顶点在G中没有邻接顶点，则返回“空”
    ArcNode *p;
    int i = LocateVex(G,v);
    int j = LocateVex(G,w);
    if(i && j){
        p = G.vertices[i].firstarc;
        while (p && p->adjvex != j) p = p->nextarc;
        if(p && p->nextarc)
            return G.vertices[p->nextarc->adjvex].data;
    }
    return ' ';
}//NextAdjVex

Status InsertVex(ALGraph *G,VertexType v){
    //v和图G中顶点有相同特征，在图G中增添新顶点v
    int i = (*G).vexnum + 1;
    if(i > MAX_VERTEX_NUM) return ERROR;
    (*G).vertices[i].data = v;
    (*G).vertices[i].firstarc = NULL;
    ++(*G).vexnum;
    return OK;
}//InsertVex

Status DeleteVex(ALGraph *G,VertexType v){
    //v和图G中顶点有相同特征，在图G中删除顶点v及其相关的弧
    int i = LocateVex(*G,v);
    int j;
    ArcNode *p,*q;
    if(i == 0) return ERROR;
    for (j = 1; j <= (*G).vexnum; ++j){
        p = q = (*G).vertices[j].firstarc;
        while (q){
            if(q->adjvex == i){     //删除以顶点v为头的弧
                if(q->info) free(q->info);
                if(q == (*G).vertices[j].firstarc){
                    (*G).vertices[j].firstarc = q->nextarc;
                    free(q); q = (*G).vertices[j].firstarc;
                }
                else {
                    p->nextarc = q->nextarc;
                    free(q); q = p->nextarc;
                }
                --(*G).arcnum; //弧数减1
            }//if
            else {
                if (q->adjvex > i) --q->adjvex;     //弧的顶点已变动
                p = q; q = q->nextarc;
            }//else
        }//while
    }//for
    p = (*G).vertices[i].firstarc;
    while (p){      //删除以顶点v为尾的弧
        q = p->nextarc;
        if((*G).kind == DG){
            if(p->info) free(p->info);
            --(*G).arcnum;
        }
        free(p); p = q;
    }
    for (j = i + 1; j <= (*G).vexnum; ++j)  //顶点上移
        (*G).vertices[j - 1] = (*G).vertices[j];
    --(*G).vexnum;      //顶点个数减1
    return OK;
}//DeleteVex

Status InsertArc(ALGraph *G,VertexType v,VertexType w,int IncInfo,...){
    //v和w是图G中两个顶点，在G中增添弧<v,w>，若G是无向的，则还增添对称弧<w,v>
    //IncInfo指示弧中是否包含信息
    int i = LocateVex(*G,v);
    int j = LocateVex(*G,w);
    ArcNode *p,*q,*r;
    if(!i || !j) return ERROR;

    va_list ap;
    va_start(ap,IncInfo);
    char *info = va_arg(ap,InfoType);     //弧信息
    va_end(ap);

    p = (*G).vertices[i].firstarc;
    while (p && p->nextarc) p = p->nextarc;
    q = (ArcNode *)malloc(sizeof(ArcNode));
    if(!q) exit(OVERFLOW);
    q->adjvex = j; q->nextarc = NULL;
    if(IncInfo){
        q->info = (InfoType *)malloc(sizeof(InfoType));
        if(!q->info) exit(OVERFLOW);
        *(q->info) = info;
    }
    else q->info = NULL;
    if(p) p->nextarc = q;
    else (*G).vertices[i].firstarc = q;

    if((*G).kind == UDG){   //无向图
        p = (*G).vertices[j].firstarc;
        while (p && p->nextarc) p = p->nextarc;
        r = (ArcNode *)malloc(sizeof(ArcNode));
        if(!r) exit(OVERFLOW);
        r->adjvex = i; r->nextarc = NULL; r->info = q->info;
        if(p) p->nextarc = r;
        else (*G).vertices[j].firstarc = r;
    }
    ++(*G).arcnum;      //弧数加1
    return OK;
}//InsertArc

Status DeleteArc(ALGraph *G,VertexType v,VertexType w){
    //v和w是图G中两个顶点，在G中删除弧<v,w>，若G是无向的，则还删除对称弧<w,v>
    int i = LocateVex(*G,v);
    int j = LocateVex(*G,w);
    ArcNode *p,*q;
    if(!i || !j) return ERROR;

    p = q = (*G).vertices[i].firstarc;
    while (q && q->adjvex != j) {p = q; q = q->nextarc;}
    if(!q) return ERROR;
    if(q->info) free(q->info);
    if(p == (*G).vertices[i].firstarc)
        (*G).vertices[i].firstarc = q->nextarc;
    else p->nextarc = q->nextarc;
    free(q);

    if((*G).kind == UDG){       //无向图
        p = q = (*G).vertices[j].firstarc;
        while (q && q->adjvex != i) {p = q; q = q->nextarc;}
        if(!q) return ERROR;
        if(p == (*G).vertices[j].firstarc)
            (*G).vertices[j].firstarc = q->nextarc;
        else p->nextarc = q->nextarc;
        free(q);
    }
    --(*G).arcnum;      //弧数减1
    return OK;
}//DeleteArc

Status DFSTraverse(ALGraph G,Status (*Visit)(VertexType)){
    //Visit是顶点的应用函数，对图G进行深度优先遍历，在遍历过程中
    //对每个顶点调用函数Visit一次且仅一次。一旦Visit()失败，则操作失败

}//DFSTraverse

Status BFSTraverse(ALGraph G,Status (*Visit)(VertexType)){
    //Visit是顶点的应用函数，对图G进行广度优先遍历，在遍历过程中
    //对每个顶点调用函数Visit一次且仅一次。一旦Visit()失败，则操作失败

}//DFSTraverse

#endif // !ALGRAPH_H
