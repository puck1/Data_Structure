
#ifndef AMLGRAPH_H
#define AMLGRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "status.h"
#include "..\General\string_p.h"    //读取不定长字符串

// - - - - - 无向图的邻接多重表(Adjacency Multilist)存储表示 - - - - -
typedef char    VertexType;         //存储数据类型定为char
typedef char*   InfoType;
#define MAX_VERTEX_NUM  20
typedef enum {unvisited, visited} VisitIf;
typedef struct EBox {
    VisitIf     mark;               //访问标记
    int         ivex, jvex;         //该边依附的两个顶点的位置
    struct EBox *ilink, *jlink;     //分别指向依附这两个顶点的下一条边
    InfoType    *info;              //该边指针信息
}EBox;
typedef struct VexBox {
    VertexType  data;
    struct EBox *firstedge;         //指向第一条依附该顶点的边
}VexBox;
typedef struct {
    VexBox  adjmulist[MAX_VERTEX_NUM + 1];
    int     vexnum, edgenum;        //无向图的当前顶点数和边数
}AMLGraph;

// - - - - - 需要调用的函数原型声明 - - - - -
int LocateVex(AMLGraph G,VertexType u);
Status DeleteArc(AMLGraph *G,VertexType v,VertexType w);
void MarkAsUnvisited(AMLGraph *G);

// - - - - - 基本操作的算法描述 - - - - -
Status CreateUDG(AMLGraph *UDG){
    //采用邻接多重表表示法，构造无向图UDG
    int i,j,k;
    int IncInfo;
    EBox *p;
    VertexType v1,v2;
    printf("Enter number of UDG's vertex,edge and determine "
            "whether to include information(0/1):");
    scanf("%d%*c%d%*c%d%*c",&(*UDG).vexnum,&(*UDG).edgenum,&IncInfo);  //IncInfo为0则各弧不包含其他信息
    printf("Enter the datas of all %d vertex(es) separated by delimitador:",(*UDG).vexnum);
    for (k = 1; k <= (*UDG).vexnum; ++k){        //建立顶点结点
        scanf("%c%*c",&(*UDG).adjmulist[k].data);
        (*UDG).adjmulist[k].firstedge = NULL;
    }
    printf("Enter two vertexes of each edge separated by delimitador:\n");
    for (k = 1; k <= (*UDG).edgenum; ++k){      //建立边结点
        printf("No.%d:",k);
        scanf("%c%*c%c%*c",&v1,&v2);            //输入一条边依附的顶点
        i = LocateVex(*UDG,v1); j = LocateVex(*UDG,v2); //确定v1和v2在UDG中位置
        if(!i || !j) return ERROR;
        if(!(p = (EBox *)malloc(sizeof(EBox)))) exit(OVERFLOW);
        p->mark = unvisited; p->ivex = i; p->jvex = j;
        p->ilink = (*UDG).adjmulist[i].firstedge;
        p->jlink = (*UDG).adjmulist[j].firstedge;
        (*UDG).adjmulist[i].firstedge = (*UDG).adjmulist[j].firstedge = p;
                                                //插入为顶点的第一条边
        if(IncInfo){                            //若边含有相关信息，则输入
            printf("Enter information (No longer than 20 chars):");
            p->info = (InfoType *)malloc(sizeof(InfoType));
            if(!p->info) exit(OVERFLOW);
            *(p->info) = getstr();              //input information
        }
        else p->info = NULL;
    }//for
    printf("Succeeded!\n");
    return OK;
}//CreateUDG

Status DestroyGraph(AMLGraph *G){
    //销毁图G
    int i;
    EBox *p, *q;
    for (i = 1; i <= (*G).vexnum; ++i){
        p = q = (*G).adjmulist[i].firstedge;
        while (p){      //释放所有以i为顶点的边结点
            q = p->ilink;
            if(!DeleteArc(G, (*G).adjmulist[p->ivex].data,
            (*G).adjmulist[p->jvex].data)) return ERROR;    //★
            p = q;
        }//while
    }//for
    (*G).vexnum = 0;
    return OK;
}//DestroyGraph

int LocateVex(AMLGraph G,VertexType u){
    //u和图G中顶点有相同特征，若图G中存在顶点u，则返回该顶点在图中位置，否则返回“空”
    for (int i = 1; i <= G.vexnum; ++i)
        if(G.adjmulist[i].data == u) return i;
    return 0;
}//LocateVex

VertexType GetVex(AMLGraph G,int i){
    //i是图G中某个顶点的位置，返回i的值
    if(i < 1 || i > G.vexnum) return ' ';
    else return G.adjmulist[i].data;
}//GetVex

Status PutVex(AMLGraph *G,VertexType v,VertexType value){
    //v是图G中某个顶点，对v赋值valve
    int i = LocateVex(*G,v);
    if(i == 0) return ERROR;
    else{
        (*G).adjmulist[i].data = value;
        return OK;
    }
}//PutVex

VertexType FirstAdjVex(AMLGraph G,VertexType v){
    //v是图G中某个顶点，返回v的第一个邻接顶点，若顶点在G中没有邻接顶点，则返回“空”
    EBox *p;
    int k = LocateVex(G,v);
    if(k != 0){
        p = G.adjmulist[k].firstedge;
        if(p){
            if(k == p->ivex)
                return G.adjmulist[p->jvex].data;
            else if (k == p->jvex)
                return G.adjmulist[p->ivex].data;   //★
        }
    }
    return ' ';
}//FirstAdjVex

VertexType NextAdjVex(AMLGraph G,VertexType v,VertexType w){
    //v是图G中某个顶点，w是v的邻接顶点，返回v的（相对于w的）下一个邻接顶点；
    //若顶点在G中没有邻接顶点，则返回“空”
    EBox *p;
    int k1 = LocateVex(G,v);
    int k2 = LocateVex(G,w);
    if(k1 && k2){
        p = G.adjmulist[k1].firstedge;
        while (p){      //寻找边<v，w>的邻接边★
            if(p->ivex == k1){
                if(p->jvex == k2) {p = p->ilink; break;}
                else p = p->ilink;
            }
            else if(p->jvex == k1){
                if (p->ivex == k2) {p = p->jlink; break;}
                else p = p->jlink;
            }
        }//while
        if(p && p->ivex == k1) return G.adjmulist[p->jvex].data;
        if(p && p->jvex == k1) return G.adjmulist[p->ivex].data;
    }//if
    return ' ';
}//NextAdjVex

Status InsertVex(AMLGraph *G,VertexType v){
    //v和图G中顶点有相同特征，在图G中增添新顶点v
    int i = (*G).vexnum + 1;
    if(i > MAX_VERTEX_NUM) return ERROR;
    (*G).adjmulist[i].data = v;
    (*G).adjmulist[i].firstedge = NULL;
    ++(*G).vexnum;
    return OK;
}//InsertVex

Status DeleteVex(AMLGraph *G,VertexType v){
    //v和图G中顶点有相同特征，在图G中删除顶点v及其相关的弧
    int k1;
    int k2 = LocateVex(*G,v);
    EBox *p, *q;
    if(k2 == 0) return ERROR;

    MarkAsUnvisited(G);
    for (k1 = 1; k1 <= (*G).vexnum; ++k1){
        if(k1 != k2){
            p = q = (*G).adjmulist[k1].firstedge;
            while (q){
                if(q->ivex == k1){      //k1作为边的i顶点
                    if(q->jvex == k2){  //q为需要删除的边
                        if(q->info) free(q->info);
                        if(q == (*G).adjmulist[k1].firstedge){
                            (*G).adjmulist[k1].firstedge = q->ilink;
                            free(q); q = (*G).adjmulist[k1].firstedge;
                        }
                        else if(p->ivex == k1){
                            p->ilink = q->ilink;
                            free(q); q = p->ilink;
                        }
                        else{   //p->jvex == k1
                            p->jlink = q->ilink;
                            free(q); q = p->jlink;
                        }
                        --(*G).edgenum;
                    }//q->jvex == k2
                    else {p = q; q = q->ilink;}
                }//if(q->ivex == k1)

                else if(q->jvex == k1){     //k1作为边的j顶点
                    if(q->ivex == k2){
                        if(q == (*G).adjmulist[k1].firstedge){
                            (*G).adjmulist[k1].firstedge = q->jlink;
                            free(q); q = (*G).adjmulist[k1].firstedge;
                        }
                        else if(p->ivex == k1){
                            p->ilink = q->jlink;
                            free(q); q = p->ilink;
                        }
                        else{   //p->jvex == k1
                            p->jlink = q->jlink;
                            free(q); q = p->jlink;
                        }
                        --(*G).edgenum;
                    }//q->ivex == k2
                    else { p = q; q = q->jlink;}
                }//else if(q->jvex == k1)

            }//while
        }//if(k1 != k2)
    }//for

    for (k1 = 1; k1 <= (*G).vexnum; ++k1){      //更改顶点已经移动的边结点
        if(k1 != k2){
            p = (*G).adjmulist[k1].firstedge;
            while (p){
                if(p->mark == unvisited){
                    if(p->ivex > k2) --p->ivex;
                    if(p->jvex > k2) --p->jvex;
                    p->mark = visited;
                }
                p = p->ilink;
            }//while
        }//if
    }//for

    for(k1 = k2 + 1; k1 <= (*G).vexnum; ++k1)
        (*G).adjmulist[k1 - 1] = (*G).adjmulist[k1];    //顶点上移
    --(*G).vexnum;      //顶点个数减1
    MarkAsUnvisited(G);
    return OK;
}//DeleteVex

Status InsertArc(AMLGraph *G,VertexType v,VertexType w,int IncInfo,...){
    //v和w是图G中两个顶点，在G中增添弧<v,w>，若G是无向的，则还增添对称弧<w,v>
    //IncInfo指示弧中是否包含信息
    int i = LocateVex(*G,v);
    int j = LocateVex(*G,w);
    EBox *p;
    if(!i || !j) return ERROR;

    va_list ap;
    va_start(ap,IncInfo);
    InfoType info = va_arg(ap,InfoType);    //边信息
    va_end(ap);

    if(!(p = (EBox *)malloc(sizeof(EBox)))) exit(OVERFLOW);
    p->mark = unvisited; p->ivex = i; p->jvex = j;
    p->ilink = (*G).adjmulist[i].firstedge;
    p->jlink = (*G).adjmulist[j].firstedge;
    if(IncInfo){
        p->info = (InfoType *)malloc(sizeof(InfoType));
        if(!(p->info)) exit(OVERFLOW);
        *(p->info) = info;
    }
    else p->info = NULL;
    (*G).adjmulist[i].firstedge = (*G).adjmulist[j].firstedge = p;  //插入为顶点的第一条边
    ++(*G).edgenum;  //边数加1

    return OK;
}//InsertArc

Status DeleteArc(AMLGraph *G,VertexType v,VertexType w){
    //v和w是图G中两个顶点，在G中删除弧<v,w>
    int k1 = LocateVex(*G,v);
    int k2 = LocateVex(*G,w);
    EBox *p, *q, *r;
    if(!k1 || !k2) return ERROR;

    p = q = (*G).adjmulist[k1].firstedge;
    while (q){
        if(q->ivex == k1){
            if(q->jvex == k2){
                if(q == (*G).adjmulist[k1].firstedge)
                    (*G).adjmulist[k1].firstedge = q->ilink;
                else if(p->ivex == k1) p->ilink = q->ilink;
                else p->jlink = q->ilink;
                break;
            }
            else {p = q; q = q->ilink;}
        }//if(q->ivex == k1)

        else if(q->jvex == k1){
            if(q->ivex == k2){
                if(q == (*G).adjmulist[k1].firstedge)
                    (*G).adjmulist[k2].firstedge = q->jlink;
                else if(p->ivex == k1) p->ilink = q->jlink;
                else p->jlink = q->jlink;
                break;
            }
            else {p = q; q = q->jlink;}
        }//else if(q->jvex == k1)
    }//while

    p = r = (*G).adjmulist[k2].firstedge;
    while(r){
        if(r->ivex == k2){
            if(r == q){
                if(r == (*G).adjmulist[k2].firstedge)
                    (*G).adjmulist[k2].firstedge = r->ilink;
                else if (p->ivex == k2) p->ilink = r->ilink;
                else p->jlink = r->ilink;
                break;
            }
            else {p = r; r = r->ilink;}
        }//if(r->jvex == k2)

        else if(r->jvex == k2){
            if(r == q){
                if(r == (*G).adjmulist[k2].firstedge)
                    (*G).adjmulist[k2].firstedge = r->jlink;
                else if(p->ivex == k2) p->ilink = r->jlink;
                else p->jlink = r->jlink;
                break;
            }
            else {p = r; r = r->jlink;}
        }//else if(r->jvex == k2)
    }//while

    if(q->info) free(q->info);
    free(q); --(*G).edgenum;    //释放边结点，边数减1
    return OK;
}//DeleteArc

void MarkAsUnvisited(AMLGraph *G){
    //将邻接多重表表示的图G中边的访问标记全部置为unvisited
    int i;
    EBox *p;
    for (i = 1; i <= (*G).vexnum; ++i){
        p = (*G).adjmulist[i].firstedge;
        while (p){
            if(p->mark == visited) p->mark = unvisited;
            p = p->ilink;
        }//while
    }//for
}//MarkAsUnvisited

Status DFSTraverse(AMLGraph G,Status (*Visit)(VertexType)){
    //Visit是顶点的应用函数，对图G进行深度优先遍历，在遍历过程中
    //对每个顶点调用函数Visit一次且仅一次。一旦Visit()失败，则操作失败

}//DFSTraverse

Status BFSTraverse(AMLGraph G,Status (*Visit)(VertexType)){
    //Visit是顶点的应用函数，对图G进行广度优先遍历，在遍历过程中
    //对每个顶点调用函数Visit一次且仅一次。一旦Visit()失败，则操作失败

}//DFSTraverse

#endif // !AMLGRAPH_H
