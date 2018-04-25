
#ifndef MGRAPH_H
#define MGRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "status.h"
#include "..\General\string_p.h"        //读取不定长字符串

// - - - - - 图的数组（邻接矩阵 Adjacency matrix）存储表示 - - - - -
typedef char VertexType;                //存储数据类型定为char
#define INFINITY INT_MAX                //最大值∞
#define MAX_VERTAX_NUM 20               //最大顶点个数
typedef int VRType;
typedef char* InfoType;
typedef enum {DG,DN,UDG,UDN} GraphKind; //{有向图，有向网，无向图，无向网}
typedef struct ArcCell{
    VRType      adj;    //VRType是顶点关系类型。对无权图，用1或0
                        //表示相邻否；对带权图，则为权值类型。
    InfoType    *info;  //该弧相关信息的指针
}ArcCell,   AdjMatrix[MAX_VERTAX_NUM + 1][MAX_VERTAX_NUM + 1];
typedef struct {
    VertexType  vexs[MAX_VERTAX_NUM + 1];   //顶点向量
    AdjMatrix   arcs;                       //邻接矩阵
    int         vexnum,arcnum;              //图的当前顶点数和弧数
    GraphKind   kind;                       //图的种类标志
}MGraph;

// - - - - - 需要调用的函数原型声明 - - - - -
int LocateVex(MGraph G,VertexType u);

// - - - - - 基本操作的算法描述 - - - - -
Status CreateDG(MGraph *DG){
    //采用数组（邻接矩阵）表示法，构造有向图DG
    int i,j,k;
    int IncInfo;
    VertexType v1,v2;
    printf("Enter number of DG's vertex,arc and determine "
            "whether to include information(0/1):");
    scanf("%d%*c%d%*c%d%*c",&(*DG).vexnum,&(*DG).arcnum,&IncInfo);  //IncInfo为0则各弧不包含其他信息
    printf("Enter the datas of all %d vertex(es) separated by delimitador:",(*DG).vexnum);
    for(i = 1;i <= (*DG).vexnum; ++i) scanf("%c%*c",&(*DG).vexs[i]);    //构造顶点向量
    for (i = 1; i <= (*DG).vexnum; ++i)         //初始化邻接矩阵
        for (j = 1; j <= (*DG).vexnum; ++j){
            (*DG).arcs[i][j].adj = 0;
            (*DG).arcs[i][j].info = NULL;
        }
    printf("Enter each arc's tail and head separated by delimitador:\n");
    for (k = 1; k <= (*DG).arcnum; ++k){        //构造邻接矩阵
        printf("No.%d:",k);
        scanf("%c%*c%c%*c",&v1,&v2);            //输入一条弧依附的顶点
        i = LocateVex(*DG,v1); j = LocateVex(*DG,v2);   //确定v1和v2在DG中位置
        (*DG).arcs[i][j].adj = 1;               //建立弧<v1,v2>
        if(IncInfo){                            //若弧含有相关信息，则输入
            printf("Enter information (No longer than 20 chars):");
            (*DG).arcs[i][j].info = (InfoType *)malloc(sizeof(InfoType));
            if(!(*DG).arcs[i][j].info) exit(OVERFLOW);
            *(*DG).arcs[i][j].info = getstr();  //input information
        }
    }//for
    printf("Succeeded!\n");
    return OK;
}//CreateDG

Status CreateDN(MGraph *DN){
    //采用数组（邻接矩阵）表示法，构造有向网DN
    int i,j,k;
    int IncInfo;
    int w;
    VertexType v1,v2;
    printf("Enter number of DN's vertex,arc and determine "
            "whether to include information(0/1):");
    scanf("%d%*c%d%*c%d%*c",&(*DN).vexnum,&(*DN).arcnum,&IncInfo);  //IncInfo为0则各弧不包含其他信息
    printf("Enter the datas of all %d vertex(es) separated by delimitador:",(*DN).vexnum);
    for(i = 1;i <= (*DN).vexnum; ++i) scanf("%c%*c",&(*DN).vexs[i]);    //构造顶点向量
    for (i = 1; i <= (*DN).vexnum; ++i)         //初始化邻接矩阵
        for (j = 1; j <= (*DN).vexnum; ++j){
            (*DN).arcs[i][j].adj = INFINITY;
            (*DN).arcs[i][j].info = NULL;
        }
    printf("Enter each arc's tail,head and weight separated by delimitador:\n");
    for (k = 1; k <= (*DN).arcnum; ++k){        //构造邻接矩阵
        printf("No.%d:",k);
        scanf("%c%*c%c%*c%d%*c",&v1,&v2,&w);            //输入一条弧依附的顶点
        i = LocateVex(*DN,v1); j = LocateVex(*DN,v2);   //确定v1和v2在DN中位置
        (*DN).arcs[i][j].adj = w;               //建立弧<v1,v2>
        if(IncInfo){                            //若弧含有相关信息，则输入
            printf("Enter information (No longer than 20 chars):");
            (*DN).arcs[i][j].info = (InfoType *)malloc(sizeof(InfoType));
            if(!(*DN).arcs[i][j].info) exit(OVERFLOW);
            *(*DN).arcs[i][j].info = getstr();  //input information
        }
    }//for
    printf("Succeeded!\n");
    return OK;
}//CreateDN

Status CreateUDG(MGraph *UDG){
    //采用数组（邻接矩阵）表示法，构造无向图UDG
    int i,j,k;
    int IncInfo;
    VertexType v1,v2;
    printf("Enter number of UDG's vertex,edge and determine "
            "whether to include information(0/1):");
    scanf("%d%*c%d%*c%d%*c",&(*UDG).vexnum,&(*UDG).arcnum,&IncInfo);    //IncInfo为0则各边不包含其他信息
    printf("Enter the datas of all %d vertex(es) separated by delimitador:",(*UDG).vexnum);
    for(i = 1;i <= (*UDG).vexnum; ++i) scanf("%c%*c",&(*UDG).vexs[i]);  //构造顶点向量
    for (i = 1; i <= (*UDG).vexnum; ++i)        //初始化邻接矩阵
        for (j = 1; j <= (*UDG).vexnum; ++j){
            (*UDG).arcs[i][j].adj = 0;
            (*UDG).arcs[i][j].info = NULL;
        }
    printf("Enter two vertexes of each edge separated by delimitador:\n");
    for (k = 1; k <= (*UDG).arcnum; ++k){       //构造邻接矩阵
        printf("No.%d:",k);
        scanf("%c%*c%c%*c",&v1,&v2);            //输入一条边依附的顶点
        i = LocateVex(*UDG,v1); j = LocateVex(*UDG,v2); //确定v1和v2在UDG中位置
        (*UDG).arcs[i][j].adj = 1;              //建立边<v1,v2>
        if(IncInfo){                            //若边含有相关信息，则输入
            printf("Enter information (No longer than 20 chars):");
            (*UDG).arcs[i][j].info = (InfoType *)malloc(sizeof(InfoType));
            if(!(*UDG).arcs[i][j].info) exit(OVERFLOW);
            *(*UDG).arcs[i][j].info = getstr();  //input information
        }
        (*UDG).arcs[j][i] = (*UDG).arcs[i][j];  //置<v1,v2>的对称边<v2,v1>
    }//for
    printf("Succeeded!\n");
    return OK;
}//CreateUDG

Status CreateUDN(MGraph *UDN){
    //采用数组（邻接矩阵）表示法，构造无向网UDN
    int i,j,k;
    int IncInfo;
    int w;
    VertexType v1,v2;
    printf("Enter number of UDN's vertex,edge and determine "
            "whether to include information(0/1):");
    scanf("%d%*c%d%*c%d%*c",&(*UDN).vexnum,&(*UDN).arcnum,&IncInfo);    //IncInfo为0则各边不包含其他信息
    printf("Enter the datas of all %d vertex(es) separated by delimitador:",(*UDN).vexnum);
    for(i = 1;i <= (*UDN).vexnum; ++i) scanf("%c%*c",&(*UDN).vexs[i]);  //构造顶点向量
    for (i = 1; i <= (*UDN).vexnum; ++i)        //初始化邻接矩阵
        for (j = 1; j <= (*UDN).vexnum; ++j){
            (*UDN).arcs[i][j].adj = INFINITY;
            (*UDN).arcs[i][j].info = NULL;
        }
    printf("Enter two vertexes and weight of each edge separated by delimitador:\n");
    for (k = 1; k <= (*UDN).arcnum; ++k){       //构造邻接矩阵
        printf("No.%d:",k);
        scanf("%c%*c%c%*c%d%*c",&v1,&v2,&w);    //输入一条边依附的顶点
        i = LocateVex(*UDN,v1); j = LocateVex(*UDN,v2); //确定v1和v2在UDN中位置
        (*UDN).arcs[i][j].adj = w;              //建立边<v1,v2>
        if(IncInfo){                            //若边含有相关信息，则输入
            printf("Enter information (No longer than 20 chars):");
            (*UDN).arcs[i][j].info = (InfoType *)malloc(sizeof(InfoType));
            if(!(*UDN).arcs[i][j].info) exit(OVERFLOW);
            *(*UDN).arcs[i][j].info = getstr();  //input information
        }
        (*UDN).arcs[j][i] = (*UDN).arcs[i][j];  //置<v1,v2>的对称边<v2,v1>
    }//for
    printf("Succeeded!\n");
    return OK;
}//CreateUDN

Status CreateGraph(MGraph *G){
    //采用数组（邻接矩阵）表示法，构造图G
    printf("Enter the kind of Graph G (0->DG/1->DN/2->UDG/3->UDN):");
    scanf("%d",&(*G).kind);
    switch ((*G).kind){
        case  DG: return CreateDG(G);
        case  DN: return CreateDN(G);
        case UDG: return CreateUDG(G);
        case UDN: return CreateUDN(G);
        default: return ERROR;
    }
}//CreateGraph

Status DestroyGraph(MGraph *G){
    //销毁图G
    int i,j;
    for (i = 1; i <= (*G).vexnum; ++i)
        for (j = 1; j <= (*G).vexnum; ++j)
            if((*G).arcs[i][j].info){
                free((*G).arcs[i][j].info);
                (*G).arcs[i][j].info = NULL;
                if((*G).kind == UDG || (*G).kind == UDN)
                    (*G).arcs[j][i].info = NULL;    //无向
            }
    (*G).arcnum = (*G).vexnum = 0;
    return OK;
}//DestroyGraph

int LocateVex(MGraph G,VertexType u){
    //u和图G中顶点有相同特征，若图G中存在顶点u，则返回该顶点在图中位置，否则返回“空”
    for (int i = 1; i <= G.vexnum; ++i)
        if(G.vexs[i] == u) return i;
    return 0;
}//LocateVex

VertexType GetVex(MGraph G,int i){
    //i是图G中某个顶点的位置，返回i的值
    if(i < 1 || i > G.vexnum) return ' ';
    else return G.vexs[i];
}//GetVex

Status PutVex(MGraph *G,VertexType v,VertexType value){
    //v是图G中某个顶点，对v赋值valve
    int i = LocateVex(*G,v);
    if(i == 0) return ERROR;
    else{
        (*G).vexs[i] = value;
        return OK;
    }
}//PutVex

VertexType FirstAdjVex(MGraph G,VertexType v){
    //v是图G中某个顶点，返回v的第一个邻接顶点，若顶点在G中没有邻接顶点，则返回“空”
    int i = LocateVex(G,v);
    int j; int n;
    if(G.kind == DG || G.kind == UDG) n = 0;
    else n = INFINITY;
    if(i != 0){
       for (j = 1; j <= G.arcnum; ++j)
            if(G.arcs[i][j].adj != n)
                return G.vexs[j];
    }
    return ' ';     //空格表示“空”
}//FirstAdjVex

VertexType NextAdjVex(MGraph G,VertexType v,VertexType w){
    //v是图G中某个顶点，w是v的邻接顶点，返回v的（相对于w的）下一个邻接顶点；
    //若顶点在G中没有邻接顶点，则返回“空”
    int i = LocateVex(G,v);
    int j = LocateVex(G,w);
    int n;
    if(G.kind == DG || G.kind == UDG) n = 0;
    else n = INFINITY;
    if(i && j && G.arcs[i][j].adj != n){
        for (j = j + 1; j <= G.vexnum; ++j)
            if(G.arcs[i][j].adj != n)
                return G.vexs[j];
    }
    return ' ';
}//NextAdjVex

Status InsertVex(MGraph *G,VertexType v){
    //v和图G中顶点有相同特征，在图G中增添新顶点v
    int i = (*G).vexnum + 1 , j;
    int n;
    if(i > MAX_VERTAX_NUM) return ERROR;
    (*G).vexs[i] = v;
    if((*G).kind == DG || (*G).kind == UDG) n = 0;
    else n = INFINITY;
    for(j = 1; j <= i; ++j){
        (*G).arcs[i][j].adj = n;
        (*G).arcs[i][j].info = NULL;
        (*G).arcs[j][i].adj = n;
        (*G).arcs[j][i].info = NULL;
    }
    ++(*G).vexnum;
    return OK;
}//InsertVex

Status DeleteVex(MGraph *G,VertexType v){
    //v和图G中顶点有相同特征，在图G中删除顶点v及其相关的弧
    int i = LocateVex(*G,v) , j , k;
    int n;
    if(i == 0) return ERROR;
    if((*G).kind == DG || (*G).kind == UDG) n = 0;
    else n = INFINITY;
    for (j = 1; j <= (*G).vexnum; ++j){             //计算新的弧数及释放弧信息空间
        if((*G).arcs[i][j].adj != n)
            --(*G).arcnum;
        if((*G).arcs[i][j].info)
            free((*G).arcs[i][j].info);
        if((*G).kind == DG || (*G).kind == DN){     //有向
            if((*G).arcs[j][i].adj != n)
                --(*G).arcnum;
            if((*G).arcs[j][i].info)
                free((*G).arcs[j][i].info);
        }//if
    }//for
    for (k = i + 1; k <= (*G).vexnum; ++k){
        (*G).vexs[k-1] = (*G).vexs[k];              //第i号以后的顶点前移
        for (j = 1; j <= (*G).vexnum; ++j)
            (*G).arcs[k-1][j] = (*G).arcs[k][j];    //邻接矩阵中第i行以后上移
    }
    for (k = i + 1; k <= (*G).vexnum; ++k)
        for (j = 1; j <= (*G).vexnum - 1; ++j)
            (*G).arcs[j][k-1] = (*G).arcs[j][k];    //邻接矩阵中第i列以后左移
    --(*G).vexnum;
    return OK;
}//DeleteVex

Status InsertArc(MGraph *G,VertexType v,VertexType w,...){
    //v和w是图G中两个顶点，在G中增添弧<v,w>，若G是无向的，则还增添对称弧<w,v>
    int i = LocateVex(*G,v);
    int j = LocateVex(*G,w);
    int weight = 1;
    // char info[21];
    if(!i || !j) return ERROR;
    va_list ap;
    va_start(ap,w);
    if((*G).kind == DN || (*G).kind == UDN) weight = va_arg(ap,int);    //网
    char *info = va_arg(ap,InfoType);     //弧信息
    va_end(ap);
    (*G).arcs[i][j].adj = weight;
    if(info){
         (*G).arcs[i][j].info = (InfoType *)malloc(sizeof(InfoType));
         if(!(*G).arcs[i][j].info) exit(OVERFLOW);
         *(*G).arcs[i][j].info = info;
    }
    if((*G).kind == UDG || (*G).kind == UDN) (*G).arcs[j][i] = (*G).arcs[i][j];
    ++(*G).arcnum;
    return OK;
}//InsertArc

Status DeleteArc(MGraph *G,VertexType v,VertexType w){
    //v和w是图G中两个顶点，在G中删除弧<v,w>，若G是无向的，则还删除对称弧<w,v>
    int i = LocateVex(*G,v);
    int j = LocateVex(*G,w);
    int n;
    if((*G).kind == DG || (*G).kind == UDG) n = 0;
    else n = INFINITY;
    if(!i || !j) return ERROR;
    (*G).arcs[i][j].adj = n;
    if((*G).arcs[i][j].info){       //清除弧信息
        free((*G).arcs[i][j].info);
        (*G).arcs[i][j].info = NULL;
    }
    if((*G).kind == UDG || (*G).kind == UDN)    //无向
        (*G).arcs[j][i] = (*G).arcs[i][j];
    --(*G).arcnum;                  //弧数减一
    return OK;
}//DeleteArc

Status DFSTraverse(MGraph G,Status (*Visit)(VertexType)){
    //Visit是顶点的应用函数，对图G进行深度优先遍历，在遍历过程中
    //对每个顶点调用函数Visit一次且仅一次。一旦Visit()失败，则操作失败

}//DFSTraverse

Status BFSTraverse(MGraph G,Status (*Visit)(VertexType)){
    //Visit是顶点的应用函数，对图G进行广度优先遍历，在遍历过程中
    //对每个顶点调用函数Visit一次且仅一次。一旦Visit()失败，则操作失败

}//DFSTraverse

#endif // !MGRAPH_H
