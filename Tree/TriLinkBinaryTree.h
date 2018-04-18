
#ifndef TRILINKBINARYTREE_H
#define TRILINKBINARYTREE_H

#include <stdio.h>
#include <stdlib.h>
#include "status.h"

// - - - - - 二叉树的三叉链表存储表示 - - - - -
typedef char TElemType;					//存储数据类型定为char
typedef struct TBiNode{
	TElemType data;
	struct TBiNode *parent,*lchild,*rchild;	//父节点及左右孩子指针
}TBiNode,*TBiTree;

// - - - - - 基本操作的算法描述 - - - - -
Status InitBiTree(TBiTree *T){
	//构造空二叉树T
	*T = NULL;
	return OK;
}//InitBiTree

Status DestroyBiTree(TBiTree *T){
	//销毁二叉树T
	if(*T){
		if((*T)->lchild) DestroyBiTree(&(*T)->lchild);
		if((*T)->rchild) DestroyBiTree(&(*T)->rchild);
		free(*T); *T = NULL;
	}
	return OK;
	//销毁与清空操作相同
}//DestroyBiTree

Status CreateBiTree(TBiTree *T){
	//按先序次序输入二叉树中结点的值（一个字符），空格字符表示空树，
	//构造二叉树T
	TElemType e = getchar();
	if(e == ' ')
		*T = NULL;
	else{
		*T = (TBiTree)malloc(sizeof(TBiNode));
		if(!(*T)) exit(OVERFLOW);
		(*T)->data = e;								//生成根结点
		(*T)->parent = NULL;						//父结点暂设为空
		CreateBiTree(&(*T)->lchild);				//递归建立左子树
		if((*T)->lchild) (*T)->lchild->parent = *T;	//若左子结点不空，设置其父节点
		CreateBiTree(&(*T)->rchild);				//递归建立右子树
		if((*T)->rchild) (*T)->rchild->parent = *T;	//若右子结点不空，设置其父节点
	}
	return OK;
}//CreateBiTree

Status ClearBiTree(TBiTree *T){
	//将二叉树T清为空树
	if(*T){		//T结点存在
		if((*T)->lchild) ClearBiTree(&(*T)->lchild);	//递归清空左子树
		if((*T)->rchild) ClearBiTree(&(*T)->rchild);	//递归清空右子树
		free(*T); *T = NULL;	//删除T结点
	}
	return OK;
}//ClearBiTree

Status BiTreeEmpty(TBiTree T){
	//若二叉树T为空树，则返回TRUE，否则返回FALSE
	if(!T) return TRUE;
	else return FALSE;
}//BiTreeEmpty

int BiTreeDepth(TBiTree T){
	//返回二叉树T的深度
	int ld,rd;
	if(T == NULL)
		return 0;
	else{
		ld = BiTreeDepth(T->lchild);	//递归返回左子树深度
		rd = BiTreeDepth(T->rchild);	//递归返回右子树深度
		return (ld>rd?ld:rd)+1;			//二叉树深度为最大子树深度+1
	}
}//BiTreeDepth

TElemType Root(TBiTree T){
	//若二叉树T的根存在，返回二叉树的根，否则返回"空"
	if(T) return T->data;
	else return ' ';
}//Root

TElemType Value(TBiNode *p){
	//p为二叉树T中某结点的位置，返回其值
	return p->data;
}//Value

Status Assign(TBiNode *p,TElemType e){
	//p为二叉树中某结点的位置，用e更新其值
	p->data = e;
	return OK;
}//Assign

TElemType Parent(TBiNode *p){
	//若p为二叉树中某非根节点的位置，则返回它的双亲，否则返回"空"
	if(p->parent) return p->parent->data;
	else return ' ';
}//Parent

TElemType LeftChild(TBiNode *p){
	//返回p所指结点的左孩子，若p无左孩子，则返回"空"
	if(p->lchild) return p->lchild->data;
	else return ' ';
}//LeftChild

TElemType RightChild(TBiNode *p){
	//返回p所指结点的右孩子，若p无右孩子，则返回"空"
	if(p->rchild) return p->rchild->data;
	else return ' ';
}//RightChild

TElemType LeftSibling(TBiTree T,TBiNode *p){
	//返回p所指结点的左兄弟，若p无左兄弟，则返回"空"
	if(!p->parent || !p->parent->lchild || p->parent->lchild==p)
		return ' ';
	else return p->parent->lchild->data;
}//LeftSibling

TElemType RightSibling(TBiTree T,TBiNode *p){
	//返回p所指结点的右兄弟，若p无右兄弟，则返回"空"
	if(!p->parent || !p->parent->rchild || p->parent->rchild==p)
		return ' ';
	else return p->parent->lchild->data;
}//RightSibling

Status InsertChild(TBiNode *p,int LR,TBiTree c){
	//根据LR为0或1，插入c为T中p所指节点的左或右子树。p所指节点的原有左
	//或右子树则成为c的右子树。
	switch(LR){
		case 0:
			c->rchild = p->lchild;
			p->lchild->parent = c;	//修改p所指结点左子树的父指针域
			p->lchild = c;
			c->parent = p;
			break;
		case 1:
			c->rchild = p->rchild;
			p->rchild->parent = c;	//修改p所指结点右子树的父指针域
			p->rchild = c;
			c->parent = p;
			break;
	}
	return OK;
}//InsertChild

Status DeleteChild(TBiNode *p,int LR){
	//根据LR为0或1，删除T中p所指结点的左或右子树。
	switch(LR){
		case 0:		//递归删除左子树结点
			if(p->lchild->lchild)
				DeleteChild(p->lchild,0);
			if(p->lchild->rchild)
				DeleteChild(p->lchild,1);
			free(p->lchild);
			p->lchild = NULL;
			break;
		case 1:		//递归删除右子树结点
			if(p->rchild->lchild)
				DeleteChild(p->rchild,0);
			if(p->rchild->rchild)
				DeleteChild(p->rchild,1);
			free(p->rchild);
			p->rchild = NULL;
			break;
	}
	return OK;
}//DeleteChild

Status PreOrderTraverse_Recur(TBiTree T,Status (*Visit)(TElemType)){
	//先序遍历T的递归算法，对每个结点调用函数Visit一次且仅一次。
	//一旦Visit()失败，则操作失败。
	if(T){
		if(Visit(T->data))
			if(PreOrderTraverse_Recur(T->lchild,Visit))
				if(PreOrderTraverse_Recur(T->rchild,Visit))
					return OK;
		return ERROR;
	}
	else return OK;
}//PreOrderTraverse_Recur

Status InOrderTraverse_Recur(TBiTree T,Status (*Visit)(TElemType)){
	//中序遍历T的递归算法，对每个结点调用函数Visit一次且仅一次。
	//一旦Visit()失败，则操作失败。
	if(T){
		if(InOrderTraverse_Recur(T->lchild,Visit))
			if(Visit(T->data))
				if(InOrderTraverse_Recur(T->rchild,Visit))
					return OK;
		return ERROR;
	}
	else return OK;
}//InOrderTraverse_Recur

Status PostOrderTraverse_Recur(TBiTree T,Status (*Visit)(TElemType)){
	//后序遍历T的递归算法，对每个结点调用函数Visit一次且仅一次。
	//一旦Visit()失败，则操作失败。
	if(T){
		if(PostOrderTraverse_Recur(T->lchild,Visit))
			if(PostOrderTraverse_Recur(T->rchild,Visit))
				if(Visit(T->data))
					return OK;
		return ERROR;
	}
	else return OK;
}//PostOrderTraverse_Recur

Status LevelOrderTraverse(TBiTree T,Status (*Visit)(TElemType)){
	//层序遍历T，对每个结点调用函数Visit一次且仅一次。
	//一旦Visit()失败，则操作失败。
	TBiTree tmp[100];		//暂存要访问的结点指针
	int i = 0,j = 0;
	if(T) tmp[j++] = T;		//存入根指针
	while(i < j){
		if(!(Visit(tmp[i]->data))) return ERROR;		//访问结点
		if(tmp[i]->lchild) tmp[j++] = tmp[i]->lchild;	//存入当前访问结点的左孩子
		if(tmp[i]->rchild) tmp[j++] = tmp[i]->rchild;	//存入当前访问结点的右孩子
		++i;
	}
	return OK;
}//LevelOrderTraverse

#endif
