
#ifndef THREADEDBINARYTREE_H
#define THREADEDBINARYTREE_H

#include <stdio.h>
#include <stdlib.h>
#include "status.h"

#define NeedPostOrder 0     //后序访问开关

// - - - - - 二叉树的二叉线索存储表示 - - - - -
/* 为方便起见，在二叉树的线索链表上也添加一个头结点，并令其lchild域的指针指向二叉树的根结点，
 * 其rchild域的指针指向遍历时访问的最后一个结点；
 * 反之，令二叉树遍历序列中的第一个结点的lchild域指针和最后一个结点rchild域的指针（如果为Thread）均指向头结点；
 * 好比为二叉树建立了一个双向线索链表，既可从第一个结点起顺后继进行遍历，也可从最后一个结点起顺前驱进行遍历。
 */
typedef char TElemType;                     //二叉树存储数据类型定为char
typedef enum {Link,Thread} PointerTag;      //Link == 0:指针，Thread == 1:线索
typedef struct BiThrNode{
    TElemType   data;
    struct BiThrNode *lchild,*rchild;       //左右孩子指针
    PointerTag  LTag,RTag;                  //左右标志
#if NeedPostOrder
    struct BiThrNode *parent;               //需要后序访问时，建立带父指针三叉链表
#endif // NeedPostOrder
}BiThrNode,*BiThrTree;

// - - - - - 一些需要调用的函数原型声明 - - - - -
Status BuildTreeBody(BiThrTree *T);
void DeThreadTreeBody(BiThrTree T);
Status DestroyTreeBody(BiThrTree *T);
void PreThreading(BiThrTree T,BiThrTree *pre);
void InThreading(BiThrTree T,BiThrTree *pre);
void PostThreading(BiThrTree T,BiThrTree *pre);

// - - - - - 基本操作的算法描述 - - - - -
Status InitBiThrTree(BiThrTree *Thrt){
	//构造空线索二叉树Thrt，Thrt指向其头结点
    *Thrt = (BiThrTree)malloc(sizeof(BiThrNode)); //建立头结点
    if(!(*Thrt)) exit(OVERFLOW);
    (*Thrt)->data = '$';
    (*Thrt)->LTag = Link;       //左孩子标志为指针
    (*Thrt)->RTag = Thread;     //右孩子标志为线索
    (*Thrt)->lchild = (*Thrt)->rchild = *Thrt;       //左右孩子指针回指
    return OK;
}//InitBiThrTree

Status DestroyBiThrTree(BiThrTree *Thrt){
	//销毁线索二叉树Thrt
    BiThrTree T = (*Thrt)->lchild;
    if(T != *Thrt){
        if(!DestroyTreeBody(&T))   //摧毁二叉树树体
            return ERROR;
    }
    free(*Thrt); *Thrt = NULL;     //销毁头结点
    return OK;
}//DestroyBiThrTree

Status CreateBiThrTree(BiThrTree *Thrt){
	//按先序次序构造带头结点的线索二叉树Thrt，并建立左右孩子标志，此时二叉树未线索化
    if(!BuildTreeBody(&(*Thrt)->lchild)) return ERROR;  //建立树体
    if(!(*Thrt)->lchild) (*Thrt)->lchild = *Thrt;       //若树体空，则左指针回指
    return OK;
}//CreateBiThrTree

Status ClearBiThrTree(BiThrTree Thrt){
	//将线索二叉树Thrt清为空树，Thrt指向其头结点
    BiThrTree T = Thrt->lchild;
    if(T != Thrt){
        if(!DestroyTreeBody(&T)) return ERROR;  //摧毁二叉树树体
        Thrt->lchild = Thrt->rchild = Thrt;     //头结点左右孩子指针回指
    }
    return OK;
}//ClearBiThrTree

Status BuildTreeBody(BiThrTree *T){
	//按先序次序输入二叉树中结点的值（一个字符），空格字符表示空树，
	//递归构造线索二叉树的树体T，并建立左右孩子标志
	TElemType e = getchar();
	if(e == ' ') *T = NULL;                 //树体为空
	else{
		*T = (BiThrTree)malloc(sizeof(BiThrNode));
		if(!(*T)) exit(OVERFLOW);
		(*T)->data = e;						//生成根结点
		BuildTreeBody(&(*T)->lchild);		//递归建立左子树
        if((*T)->lchild){
            (*T)->LTag = Link;
            #if NeedPostOrder
            (*T)->lchild->parent = *T;      //若为三叉链表，建立父指针域
            #endif
        }
        else (*T)->LTag = Thread;           //建立左孩子标志
		BuildTreeBody(&(*T)->rchild);		//递归建立右子树
        if((*T)->rchild) {
            (*T)->RTag = Link;
            #if NeedPostOrder
            (*T)->rchild->parent = *T;      //若为三叉链表，建立父指针域
            #endif
        }
        else (*T)->RTag = Thread;           //建立右孩子标志
	}
	return OK;
}//BuildTreeBody

void DeThreadTreeBody(BiThrTree T){
    //树体递归去线索化
    if(T->LTag == Thread) T->lchild = NULL;
    else DeThreadTreeBody(T->lchild);
    if(T->RTag == Thread) T->rchild = NULL;
    else DeThreadTreeBody(T->rchild);
}//DeThreadTreeBody

Status DestroyTreeBody(BiThrTree *T){
    //递归摧毁树体
    if((*T)->LTag == Link) DestroyTreeBody(&(*T)->lchild); //递归摧毁左孩子
    if((*T)->RTag == Link) DestroyTreeBody(&(*T)->rchild); //递归摧毁右孩子
    free(*T); *T = NULL;
    return OK;
}//DestroyTreeBody

void PreOrderThreading(BiThrTree Thrt){
    //前序遍历二叉树Thrt，并将其前序线索化，Thrt指向其头结点
    BiThrNode *pre = Thrt , *T = Thrt ->lchild;
    if(T != Thrt){
        PreThreading(T,&pre);
        pre->rchild = Thrt;     //最后一个结点线索化
        Thrt->rchild = pre;     //头结点右线索指向最后一个结点
    }
}//PreOrderThreading

void PreThreading(BiThrTree T,BiThrTree *pre){
    if(T){
        if(T->LTag == Thread) T->lchild = *pre;             //前驱线索
        if((*pre)->RTag == Thread) (*pre)->rchild = T;      //后继线索
        *pre = T;                       //保持pre指向T的前驱
        if(T->LTag != Thread) PreThreading(T->lchild,pre);  //左子树线索化
        if(T->RTag != Thread) PreThreading(T->rchild,pre);  //右子树线索化
    }
}//PreThreading

void InOrderThreading(BiThrTree Thrt){
    //中序遍历二叉树Thrt，并将其中序线索化，Thrt指向其头结点
    BiThrNode *pre = Thrt , *T = Thrt->lchild;
    if(T != Thrt){              //树体不为空
        InThreading(T,&pre);    //中序遍历进行中序线索化
        pre->rchild = Thrt;     //最后一个结点线索化
        Thrt->rchild = pre;     //头结点右线索指向最后一个结点
    }
}//InOrderThreading

void InThreading(BiThrTree T,BiThrTree *pre){
	if(T){
		InThreading(T->lchild,pre);     //左子树线索化
        if(T->LTag == Thread) T->lchild = *pre;         //前驱线索
        if((*pre)->RTag == Thread) (*pre)->rchild = T;  //后继线索
		*pre = T;                       //保持pre指向T的前驱
		InThreading(T->rchild,pre);     //右子树线索化
	}
}//InThreading

/* void LevelOrderDeThreading(BiThrTree Thrt){
    //利用层序遍历，将线索二叉树Thrt去线索化
    BiThrNode *tmp[100];            //暂存要访问的结点指针
    int i = 0,j = 0;
    BiThrTree T = Thrt->lchild;
    if(T != Thrt) tmp[j++] = T;     //存入根指针
    while(i<j){
        if(tmp[i]->LTag == Thread) tmp[i]->lchild = NULL;   //左孩子去线索化
        else tmp[j++] = tmp[i]->lchild;     //寄存左孩子以待访问
        if(tmp[i]->RTag == Thread) tmp[i]->rchild = NULL;   //右孩子去线索化
        else tmp[j++] = tmp[i]->rchild;     //寄存右孩子以待访问
        ++i;
    }
    Thrt->rchild = Thrt;        //头结点右孩子域回指
}//LevelOrderDeThreading */

void DeThreading(BiThrTree Thrt){
    //将线索二叉树Thrt去线索化，Thrt指向其头结点
    BiThrTree T = Thrt->lchild;
    if(T != Thrt){
        DeThreadTreeBody(T);
        Thrt->rchild = Thrt;    //头结点右孩子域回指
    }
}//DeThreading

Status PreOrderTraverse_Thr(BiThrTree Thrt,Status (*Visit)(TElemType)){
    //Thrt指向头结点，头结点的左链lchild指向根节点，采用双向线索链表为存储结构。
    //前序遍历二叉线索树Thrt的的非递归算法，对每个数据元素调用函数Visit。
    BiThrTree T = Thrt->lchild;     //T指向根节点
    while(T != Thrt){               //空树或遍历结束时，T==Thrt
        if(!Visit(T->data)) return ERROR;       //访问结点
        if(T->LTag != Thread) T = T->lchild;    //左子树不空，向左一步
        else T = T->rchild;                     //左子树为空，向右一步
    }
    return OK;
}//PreOrderTraverse_Thr

Status InOrderTraverse_Thr(BiThrTree Thrt,Status (*Visit)(TElemType)){
    //Thrt指向头结点，头结点的左链lchild指向根节点，采用双向线索链表为存储结构。
    //中序遍历二叉线索树Thrt的的非递归算法，对每个数据元素调用函数Visit。
    BiThrTree T = Thrt->lchild;     //T指向根节点
    while (T != Thrt){              //空树或遍历结束时，T==Thrt
        while(T->LTag != Thread) T = T->lchild;         //向左走到尽头
        if(!Visit(T->data)) return ERROR;               //访问左子树为空的结点
        while(T->RTag==Thread && T->rchild!=Thrt){      //★
            T = T->rchild;
            if(!Visit(T->data)) return ERROR;           //访问后继结点
        }
        T = T->rchild;
    }
    return OK;
}//InOrderTraverse_Thr

#if NeedPostOrder
void PostOrderThreading(BiThrTree Thrt){
    //后序遍历二叉树Thrt，并将其后序线索化，Thrt指向其头结点
    BiThrNode *pre = Thrt , *T = Thrt ->lchild;
    if(T != Thrt){
        PostThreading(T,&pre);
        if(pre->RTag == Thread)     //后序遍历中最后一个结点为头结点
            pre->rchild = Thrt;     //若头结点RTag为线索，则将其线索化
        Thrt->rchild = pre;         //头结点右线索指向最后一个结点
    }
}//PostOrderThreading

void PostThreading(BiThrTree T,BiThrTree *pre){
    if(T){
        PostThreading(T->lchild,pre);   //左子树线索化
        PostThreading(T->rchild,pre);   //右子树线索化
        if(T->LTag == Thread) T->lchild = *pre;         //前驱线索
        if((*pre)->RTag == Thread) (*pre)->rchild = T;  //后继线索
        *pre = T;                       //保持pre指向T的前驱
    }
}//PostThreading

Status PostOrderTraverse_Thr(BiThrTree Thrt,Status (*Visit)(TElemType)){
    //Thrt指向头结点，头结点的左链lchild指向根节点，采用双向线索链表为存储结构。
    //后序遍历二叉线索树Thrt的的非递归算法，对每个数据元素调用函数Visit。
    //后序线索化树上找后继时须知道结点双亲，采用带标识域的三叉链表做存储结构。
    BiThrTree T = Thrt->lchild;     //T指向根节点
    while(1){                       //寻找遍历起点★
        while(T->LTag == Link) T = T->lchild;
        if(T->RTag == Link) T = T->rchild;
        else break;
    }
    while (T != Thrt->lchild){
        while(T==T->parent->lchild && T->parent->RTag==Link){
        //若结点为其双亲的左孩子，且其双亲有右孩子，则后继为双亲右子树上后序遍历列出的第一个结点
            if(!Visit(T->data)) return ERROR;
            if(T->RTag == Thread) T = T->rchild;    //若线索能直接指出后继
            else{
                T = T->parent->rchild;
                while(1){                           //否则后继为右子树遍历起点
                    while(T->LTag == Link) T = T->lchild;
                    if(T->RTag == Link) T = T->rchild;
                    else break;
                }
            }
        }
        while(T==T->parent->rchild || T->parent->RTag==Thread){
        //若结点为双亲的右孩子或是其双亲的左孩子且其双亲无右子树，则后继为双亲结点
            if(!Visit(T->data)) return ERROR;
            T = T->parent;                  //后继为双亲结点
            if(T == Thrt->lchild) break;    //访问到根节点则跳出
        }
    }
    if(!Visit(T->data)) return ERROR;       //访问根结点
    return OK;
}//PostOrderTraverse_Thr
#endif // NeedPostOrder

#endif // !THREADEDBINARYTREE_H
