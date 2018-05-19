
#ifndef OLGRAPH_H
#define OLGRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "status.h"
#include "../General/string_p.h"        //读取不定长字符串

// - - - - - 有向图的十字链表(Orthogonal List)存储表示 - - - - -
typedef char    VertexType;             //存储数据类型定为char
typedef char*   InfoType;
#define MAX_VERTEX_NUM  20
typedef struct ArcBox{
    int             tailvex,headvex;    //该弧的尾和头结点的位置
    struct ArcBox   *hlink,*tlink;      //分别为弧头相同和弧尾相同的弧的链域
    InfoType        *info;              //该弧相关信息的指针
}ArcBox;
typedef struct {
    VertexType  data;
    ArcBox      *firstin,*firstout;     //分别指向该顶点第一条入弧和出弧
}VexNode;
typedef struct {
    VexNode xlist[MAX_VERTEX_NUM + 1];  //表头向量,0号单元弃用
    int     vexnum,arcnum;              //有向图的当前顶点数和弧数
}OLGraph;

// - - - - - 需要调用的函数原型声明 - - - - -
int LocateVex(OLGraph G,VertexType u);

// - - - - - 基本操作的算法描述 - - - - -
Status CreateDG(OLGraph *DG){
    //采用十字链表存储表示，构造有向图DG
    int i,j,k;
    int IncInfo;
    ArcBox *p;
    VertexType v1,v2;
    printf("Enter number of DG's vertex,arc and determine "
            "whether to include information(0/1):");
    scanf("%d%*c%d%*c%d%*c",&(*DG).vexnum,&(*DG).arcnum,&IncInfo);  //IncInfo为0则各弧不包含其他信息
    printf("Enter the datas of all %d vertex(es) separated by delimitador:",(*DG).vexnum);
    for (k = 1; k <= (*DG).vexnum; ++k){        //构造表头向量
        scanf("%c%*c",&(*DG).xlist[k].data);    //输入顶点值
        (*DG).xlist[k].firstin = (*DG).xlist[k].firstout = NULL;    //初始化指针
    }
    printf("Enter each arc's tail and head separated by delimitador:\n");
    for (k = 1; k <= (*DG).arcnum; ++k){        //输入各弧并构造十字链表
        printf("No.%d:",k);
        scanf("%c%*c%c%*c",&v1,&v2);            //输入一条弧的始点和终点
        i = LocateVex(*DG,v1); j = LocateVex(*DG,v2);   //确定v1和v2在DG中位置
        if(!i || !j) return ERROR;
        p = (ArcBox *)malloc(sizeof(ArcBox));
        if(!p) exit(OVERFLOW);
        p->tailvex = i; p->headvex = j;
        p->hlink = (*DG).xlist[j].firstin; p->tlink = (*DG).xlist[i].firstout;
        (*DG).xlist[j].firstin = (*DG).xlist[i].firstout = p;   //完成在入弧和出弧链头的插入★
        if(IncInfo){                            //若弧含有相关信息，则输入
            printf("Enter information (No longer than 20 chars):");
            p->info = (InfoType *)malloc(sizeof(InfoType));
            if(!p->info) exit(OVERFLOW);
            *(p->info) = getstr();             //input information
        }
        else p->info = NULL;
    }//for
    printf("Succeeded!\n");
    return OK;
}//CreateDG

Status DestroyGraph(OLGraph *G){
    //销毁图G
    int i;
    ArcBox *p,*q;
    for (i = 1; i <= (*G).vexnum; ++i){
        p = (*G).xlist[i].firstin;
        while (p){      //释放所有以顶点i为头的弧结点
            q = p->hlink;
            if(p->info) free(p->info);  //清空弧信息
            free(p); p = q;
        }
    }//for
    (*G).vexnum = (*G).arcnum = 0;
    return OK;
}//DestroyGraph

int LocateVex(OLGraph G,VertexType u){
    //u和图G中顶点有相同特征，若图G中存在顶点u，则返回该顶点在图中位置，否则返回“空”
    for (int i = 1; i <= G.vexnum; ++i)
        if(G.xlist[i].data == u) return i;
    return 0;
}//LocateVex

VertexType GetVex(OLGraph G,int i){
    //i是图G中某个顶点的位置，返回i的值
    if(i < 1 || i > G.vexnum) return ' ';
    else return G.xlist[i].data;
}//GetVex

Status PutVex(OLGraph *G,VertexType v,VertexType value){
    //v是图G中某个顶点，对v赋值valve
    int i = LocateVex(*G,v);
    if(i == 0) return ERROR;
    else{
        (*G).xlist[i].data = value;
        return OK;
    }
}//PutVex

VertexType FirstAdjVex(OLGraph G,VertexType v){
    //v是图G中某个顶点，返回v的第一个邻接顶点(以出弧连接)，若顶点在G中没有邻接顶点，则返回“空”
    ArcBox *p;
    int i = LocateVex(G,v);
    if(i != 0){
        p = G.xlist[i].firstout;
        if(p) return G.xlist[p->headvex].data;
    }
    return ' ';
}//FirstAdjVex

VertexType NextAdjVex(OLGraph G,VertexType v,VertexType w){
    //v是图G中某个顶点，w是v的邻接顶点(以出弧连接)，返回v的（相对于w的）下一个邻接顶点；
    //若顶点在G中没有邻接顶点，则返回“空”
    ArcBox *p;
    int i = LocateVex(G,v);
    int j = LocateVex(G,w);
    if(i && j){
        p = G.xlist[i].firstout;
        while (p && p->tlink){
            if(p->headvex == j) return G.xlist[p->tlink->headvex].data;
            else p = p->tlink;
        }//while
    }//if
    return ' ';
}//NextAdjVex

Status InsertVex(OLGraph *G,VertexType v){
    //v和图G中顶点有相同特征，在图G中增添新顶点v
    int i = (*G).vexnum + 1;
    if(i > MAX_VERTEX_NUM) return ERROR;
    (*G).xlist[i].data = v;
    (*G).xlist[i].firstin = (*G).xlist[i].firstout = NULL;
    ++(*G).vexnum;
    return OK;
}//InsertVex

Status DeleteVex(OLGraph *G,VertexType v){
    //v和图G中顶点有相同特征，在图G中删除顶点v及其相关的弧
    int i = LocateVex(*G,v);
    int j;
    ArcBox *p,*q;
    if(i == 0) return ERROR;
    for (j = 1; j <= (*G).vexnum; ++j){
        if(j != i){
            p = q = (*G).xlist[j].firstout;
            while (q){
                if(q->headvex == i){    //删除以顶点v为头的弧
                    if(q->info) free(q->info);
                    if(q == (*G).xlist[j].firstout){
                        (*G).xlist[j].firstout = q->tlink;
                        free(q); q = (*G).xlist[j].firstout;
                    }
                    else {
                        p->tlink = q->tlink;
                        free(q); q = p->tlink;
                    }
                    --(*G).arcnum;
                }
                else {
                    if (q->headvex > i) --q->headvex;   //弧的顶点已变动
                    if (q->tailvex > i) --q->tailvex;
                    p = q; q = q->tlink;
                }
            }//while

            p = q = (*G).xlist[j].firstin;
            while (q){
                if(q->tailvex == i){    //删除以顶点v为尾的弧
                    if(q->info) free(q->info);
                    if(q == (*G).xlist[j].firstin){
                        (*G).xlist[j].firstin = q->hlink;
                        free(q); break;
                    }
                    else {
                        p->hlink = q->hlink;
                        free(q); break;
                    }
                    --(*G).arcnum;
                }
                else {p = q; q = q->hlink;}
            }//while
        }//if
    }//for
    for (j = i + 1; j <= (*G).vexnum; ++j)  //顶点上移
        (*G).xlist[j - 1] = (*G).xlist[j];
    --(*G).vexnum;      //顶点个数减1
    return OK;
}//DeleteVex

Status InsertArc(OLGraph *G,VertexType v,VertexType w,int IncInfo,...){
    //v和w是图G中两个顶点，在G中增添弧<v,w>，IncInfo指示弧中是否包含信息
    int i = LocateVex(*G,v);
    int j = LocateVex(*G,w);
    ArcBox *p;
    if(!i || !j) return ERROR;

    va_list ap;
    va_start(ap,IncInfo);
    char *info = va_arg(ap,char*);  //弧信息
    va_end(ap);

    if(!(p = (ArcBox *)malloc(sizeof(ArcBox)))) exit(OVERFLOW);
    p->tailvex = i; p->headvex = j;
    p->hlink = (*G).xlist[j].firstin; p->tlink = (*G).xlist[i].firstout;
    if(IncInfo){
        p->info = (InfoType *)malloc(sizeof(InfoType));
        if(!p->info) exit(OVERFLOW);
        *(p->info) = info;
    }
    else p->info = NULL;
    (*G).xlist[j].firstin = (*G).xlist[i].firstout = p; //完成在入弧和出弧链头的插入
    ++(*G).arcnum;  //弧数加1

    return OK;
}//InsertArc

Status DeleteArc(OLGraph *G,VertexType v,VertexType w){
    //v和w是图G中两个顶点，在G中删除弧<v,w>
    int i = LocateVex(*G,v);
    int j = LocateVex(*G,w);
    ArcBox *p,*q;
    if(!i || !j) return ERROR;

    p = q = (*G).xlist[i].firstout;
    while (q){
        if(q->headvex == j){
            if(q == (*G).xlist[i].firstout)
                (*G).xlist[i].firstout = q->tlink;
            else p->tlink = q->tlink;
            break;
        }
        else {p = q; q = q->tlink;}
    }//while
    if(!q) return ERROR;

    p = (*G).xlist[j].firstin;
    if(p == q) (*G).xlist[j].firstin = q->hlink;
    else{
        while (p && p->hlink != q) p = p->hlink;
        if(!p) return ERROR;
        p->hlink = q->hlink;
    }

    if(q->info) free(q->info);
    free(q); --(*G).arcnum;     //释放弧结点，弧数减1
    return OK;
}//DeleteArc

Status DFSTraverse(OLGraph G,Status (*Visit)(VertexType)){
    //Visit是顶点的应用函数，对图G进行深度优先遍历，在遍历过程中
    //对每个顶点调用函数Visit一次且仅一次。一旦Visit()失败，则操作失败

}//DFSTraverse

Status BFSTraverse(OLGraph G,Status (*Visit)(VertexType)){
    //Visit是顶点的应用函数，对图G进行广度优先遍历，在遍历过程中
    //对每个顶点调用函数Visit一次且仅一次。一旦Visit()失败，则操作失败

}//DFSTraverse

#endif // !OLGRAPH_H
