
#ifndef CHILD_SIBLINGTREE_H
#define CHILD_SIBLINGTREE_H

#include <stdio.h>
#include <stdlib.h>
#include "status.h"

// - - - - - 树的二叉链表（孩子-兄弟）存储表示 - - - - -
typedef char TElemType;
typedef struct CSNode{
    TElemType       data;
    struct CSNode   *firstchild,*nextsibling;
}CSNode, *CSTree;

// - - - - - 基本操作的算法描述 - - - - -
Status InitTree(CSTree *T){
	//构造空树T
    *T = NULL;
    return OK;
}//InitTree

Status DestroyTree(CSTree *T){
	//销毁树T
    if((*T)){
        if((*T)->firstchild)    DestroyTree(&(*T)->firstchild);
        if((*T)->nextsibling)   DestroyTree(&(*T)->nextsibling);
        free(*T); *T = NULL;
    }
    return OK;
}//DestroyTree

Status CreateTree(CSTree *T){
	//按先序次序输入树中结点的值（一个字符），空格字符表示空树，递归构造树T
    TElemType e = getchar();
    if(e == ' ') *T = NULL;
    else{
        *T = (CSTree)malloc(sizeof(CSNode));
        if(!(*T)) exit(OVERFLOW);
        (*T)->data = e;                     //建立根结点
        CreateTree(&(*T)->firstchild);      //建立第一棵子树
        CreateTree(&(*T)->nextsibling);     //建立其它子树
        }
    return OK;
}//CreateTree

Status ClearTree(CSTree *T){
    //将树T清为空树
    if(*T){
        if((*T)->firstchild)    ClearTree(&(*T)->firstchild);
        if((*T)->nextsibling)   ClearTree(&(*T)->nextsibling);
        free(*T); *T = NULL;
    }
    return OK;
}//ClearTree

Status TreeEmpty(CSTree T){
    //若树T为空树，则返回TRUE，否则FALSE
     return T==NULL?TRUE:FALSE;
}//TreeEmpty

int TreeDepth(CSTree T){
    //返回树T的深度
    if(T == NULL) return 0;
    else{
        int maxd = 0,d = 0;
        CSTree p = T->firstchild;
        maxd = TreeDepth(p);        //从第一棵子树开始计算
        while(p){
            d = TreeDepth(p->nextsibling);
            maxd = maxd>d?maxd:d;   //记录子树深度最大值
            p = p->nextsibling;
        }
        return maxd+1;
    }
}//TreeDepth

TElemType Root(CSTree T){
    //返回T的根，若根不存在，返回“空”
    if(T == NULL) return ' ';
    else return T->data;
}//Root

TElemType Value(CSTree T,CSNode *p){
    //p指示树T中某个结点，返回p的值
    if(p == NULL) return ' ';
    else return p->data;
}//Value

Status Assign(CSNode *p,TElemType value){
    //用value更新p所指结点的值
    if(p ==NULL) return ERROR;
    else{
        p->data = value;
        return OK;
    }
}//Assign

TElemType Parent(CSTree T,CSNode *p){
    //p指示树中某个节点，若p是T的非根结点，则返回它的双亲，否则返回“空”
    if(p == T) return ' ';
	CSTree tmp[100];
	int i = 0,j = 0;
    TElemType e;
	tmp[j++] = T;
	while(i < j){   //层序遍历寻找双亲节点
        while(tmp[i]){
            if(tmp[i]->firstchild == p) e = tmp[i]->data;
            else if(tmp[i]->nextsibling == p) e = Parent(T,tmp[i]); //找到左兄弟，递归寻找其双亲
            if(tmp[i]->firstchild) tmp[j++] = tmp[i]->firstchild;
            tmp[i] = tmp[i]->nextsibling;   //在层内从左往右走
        }
        ++i;        //进入下一层
	}
    return e;
}//Parent

TElemType LeftChild(CSNode *p){
    //p指示树中某个节点，若p是T的非叶子结点，则返回它的最左孩子，否则返回“空”
    if(p->firstchild == NULL) return ' ';
    else return p->firstchild->data;
}//LeftChild

TElemType RightSibling(CSNode *p){
    //p指示树中某个节点，若p有右兄弟，则返回它的右兄弟，否则返回“空”
    if(p->nextsibling == NULL) return ' ';
    else return (p->nextsibling->data);
}//RightSibling

Status InsertChild(CSNode *p,int i,CSTree c){
	//p指示树中某个节点，插入非空树c为p所指结点的第i棵子树
    CSNode *q = p->firstchild;
    if(i == 1){     //插入为第一棵子树
        c->nextsibling = q;
        p->firstchild = c;
    }
    else{           //寻找左兄弟结点
        for (int j = 1; j < i - 1; ++j) q = q->nextsibling;
        c->nextsibling = q->nextsibling;
        q->nextsibling = c;
    }
    return OK;
}//InsertChild

Status DeleteChild(CSNode *p,int i){
    //p指示树中某个节点，删除树中p所指结点的第i棵子树
    // if(!p) return ERROR;
    CSNode *q = p->firstchild,*r;
    // if(!q) return ERROR;
    if(i == 1){     //删除第一棵子树
        p->firstchild = q->nextsibling;
        if(q->firstchild) DestroyTree(&q->firstchild);  //摧毁其子树
        free(q); q = NULL;  //释放结点
    }
    else{           //寻找删除子树的左兄弟
        for (int j = 1; j < i - 1; ++j){
            q = q->nextsibling;
            // if(!q || !q->nextsibling || q->nextsibling->nextsibling)
            //     return ERROR;
        }
        r = q->nextsibling;
        q->nextsibling = r->nextsibling;
        if(r->firstchild) DestroyTree(&r->firstchild);
        free(r); r = NULL;
    }
    return OK;
}//DeleteChild

Status PreOrderTraverse_Recur(CSTree T,Status (*visit)(TElemType)){
	//先序遍历树T的递归算法，对每个结点调用函数Visit一次且仅一次。
	//一旦Visit()失败，则操作失败。
    if(T){
        if(visit(T->data))
            if(PreOrderTraverse_Recur(T->firstchild,visit))
                if(PreOrderTraverse_Recur(T->nextsibling,visit))
                    return OK;
        return ERROR;
    }
    else return OK;
}//PreOrderTraverse_Recur

Status PostOrderTraverse_Recur(CSTree T,Status (*visit)(TElemType)){
	//后序遍历树T的递归算法，对每个结点调用函数Visit一次且仅一次。
	//一旦Visit()失败，则操作失败。
    if(T){
        if(PostOrderTraverse_Recur(T->firstchild,visit))
            if(visit(T->data))     //相当于中序遍历二叉树结构★
                if(PostOrderTraverse_Recur(T->nextsibling,visit))
                    return OK;
        return ERROR;
    }
    else return OK;
}//PostOrderTraverse_Recur

Status LevelOrderTraverse(CSTree T,Status (*visit)(TElemType)){
    //层序遍历树T，对T的每个结点调用函数visit()一次且至多一次
    //一旦visit()失败，则操作失败
    int i = 0,j = 0;
    CSNode *tmp[100];   //暂存要访问的结点指针
    if(T) tmp[j++] = T; //存入根结点
    while(i < j){
        while(tmp[i]){
            if(!(visit(tmp[i]->data))) return ERROR;
            if(tmp[i]->firstchild) tmp[j++] = tmp[i]->firstchild;
            tmp[i] = tmp[i]->nextsibling;     //在层内从左往右走
        }
        ++i;            //进入下一层
    }
    return OK;
}//LevelOrderTraverse

#endif // !CHILDSIBLINGTREE_H