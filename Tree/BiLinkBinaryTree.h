
#ifndef BILINKBINARYTREE_H
#define BILINKBINARYTREE_H

#include <stdio.h>
#include <stdlib.h>
#include "status.h"

// - - - - - 二叉树的二叉链表存储表示 - - - - -
typedef char TElemType;			//存储数据类型定为char
typedef struct BiTNode{
	TElemType data;
	struct BiTNode *lchild,*rchild;	//左右孩子指针
}BiTNode,*BiTree;

typedef BiTNode* SElemType;		//栈中存储树结点指针
#include "..\Stack\SqStack.h"

// - - - - - 基本操作的算法描述 - - - - -
Status InitBiTree(BiTree *T){
	//构造空二叉树T
	*T = NULL;
	return OK;
}//InitBiTree

Status DestroyBiTree(BiTree *T){
	//销毁二叉树T
	if(*T){
		if((*T)->lchild) DestroyBiTree(&(*T)->lchild);
		if((*T)->rchild) DestroyBiTree(&(*T)->rchild);
		free(*T); *T = NULL;
	}
	return OK;
	//销毁与清空操作相同
}//DestroyBiTree

Status CreateBiTree(BiTree *T){
	//按先序次序输入二叉树中结点的值（一个字符），空格字符表示空树，
	//构造二叉树T
	TElemType e = getchar();
	if(e == ' ') *T = NULL;
	else{
		*T = (BiTree)malloc(sizeof(BiTNode));
		if(!(*T)) exit(OVERFLOW);
		(*T)->data = e;						//生成根结点
		CreateBiTree(&(*T)->lchild);		//递归建立左子树
		CreateBiTree(&(*T)->rchild);		//递归建立右子树
	}
	return OK;
}//CreateBiTree

Status ClearBiTree(BiTree *T){
	//将二叉树T清为空树
	if(*T){		//T结点存在
		if((*T)->lchild) ClearBiTree(&(*T)->lchild);	//递归清空左子树
		if((*T)->rchild) ClearBiTree(&(*T)->rchild);	//递归清空右子树
		free(*T); *T = NULL;	//删除T结点
	}
	return OK;
}//ClearBiTree

Status BiTreeEmpty(BiTree T){
	//若二叉树T为空树，则返回TRUE，否则返回FALSE
	if(!T) return TRUE;
	else return FALSE;
}//BiTreeEmpty

int BiTreeDepth(BiTree T){	//★
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

TElemType Root(BiTree T){
	//若二叉树T的根存在，返回二叉树的根，否则返回"空"
	if(T) return T->data;
	else return ' ';
}//Root

TElemType Value(BiTNode *p){
	//p为二叉树T中某结点的位置，返回其值
	return p->data;
}//Value

Status Assign(BiTNode *p,TElemType e){
	//p为二叉树中某结点的位置，用e更新其值
	p->data = e;
	return OK;
}//Assign

TElemType Parent(BiTree T,BiTNode *p){
	//若p为二叉树中某非根节点的位置，则返回它的双亲，否则返回"空"
	if(p == T) return ' ';
	BiTree tmp[100];
	int i = 0,j = 0;
	tmp[j++] = T;
	while(i<j){		//层序遍历寻找双亲节点
		if(tmp[i]->lchild==p || tmp[i]->rchild==p)
			return tmp[i]->data;
		else{
			if(tmp[i]->lchild) tmp[j++] = tmp[i]->lchild;
			if(tmp[i]->rchild) tmp[j++] = tmp[i]->rchild;
			++i;
		}
	}
}//Parent

TElemType LeftChild(BiTNode *p){
	//返回p所指结点的左孩子，若p无左孩子，则返回"空"
	if(p->lchild) return p->lchild->data;
	else return ' ';
}//LeftChild

TElemType RightChild(BiTNode *p){
	//返回p所指结点的右孩子，若p无右孩子，则返回"空"
	if(p->rchild) return p->rchild->data;
	else return ' ';
}//RightChild

TElemType LeftSibling(BiTree T,BiTNode *p){
	//返回p所指结点的左兄弟，若p无左兄弟，则返回"空"
	BiTree tmp[100];
	int i = 0,j = 0;
	tmp[j++] = T;
	while(i<j){		//层序遍历寻找双亲节点
		if(tmp[i]->lchild==p || tmp[i]->rchild==p){
			if(tmp[i]->lchild==p || !tmp[i]->lchild)	//p无左兄弟
				return ' ';
			else return tmp[i]->lchild->data;
		}
		else{
			if(tmp[i]->lchild) tmp[j++] = tmp[i]->lchild;
			if(tmp[i]->rchild) tmp[j++] = tmp[i]->rchild;
			++i;
		}
	}
}//LeftSibling

TElemType RightSibling(BiTree T,BiTNode *p){
	//返回p所指结点的右兄弟，若p无右兄弟，则返回"空"
	BiTree tmp[100];
	int i = 0,j = 0;
	tmp[j++] = T;
	while(i<j){		//层序遍历寻找双亲节点
		if(tmp[i]->lchild==p || tmp[i]->rchild==p){	//p无右兄弟
			if(tmp[i]->rchild==p || !tmp[i]->rchild)
				return ' ';
			else return tmp[i]->rchild->data;
		}
		else{
			if(tmp[i]->lchild) tmp[j++] = tmp[i]->lchild;
			if(tmp[i]->rchild) tmp[j++] = tmp[i]->rchild;
			++i;
		}
	}
}//RightSibling

Status InsertChild(BiTNode *p,int LR,BiTree c){
	//根据LR为0或1，插入c为T中p所指节点的左或右子树。p所指节点的原有左
	//或右子树则成为c的右子树。
	switch(LR){
		case 0:
			c->rchild = p->lchild;
			p->lchild = c;
			break;
		case 1:
			c->rchild = p->rchild;
			p->rchild = c;
			break;
	}
	return OK;
}//InsertChild

Status DeleteChild(BiTNode *p,int LR){
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

Status PreOrderTraverse_Recur(BiTree T,Status (*Visit)(TElemType)){
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

Status PreOrderTraverse_NRecur(BiTree T,Status (*Visit)(TElemType)){
	//先序遍历T的非递归算法，对每个结点调用函数Visit一次且仅一次。
	//一旦Visit()失败，则操作失败。
	BiTree p = T;
	SqStack S;
	InitStack(&S);
	if(T) Push(&S,T);	//根指针进栈
	while(!StackEmpty(S)){
		Pop(&S,&p);
		if(!Visit(p->data)) return ERROR;	//访问节点
		if(p->rchild) Push(&S,p->rchild);	//右孩子进栈
		if(p->lchild) Push(&S,p->lchild);	//左孩子进栈
	}
	return OK;
}//PreOrderTraverse_NRecur

Status InOrderTraverse_Recur(BiTree T,Status (*Visit)(TElemType)){
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

Status InOrderTraverse_NRecur_0(BiTree T,Status (*Visit)(TElemType)){
	//中序遍历二叉树T的非递归算法0，对每个数据元素调用函数Visit
	//与非递归算法2思想一致
	BiTree p = T;
	SqStack S;
	InitStack(&S);
	if(T) Push(&S,T);			//根指针进栈
	while(!StackEmpty(S)){
		while(p && p->lchild){	//向左走到尽头
			Push(&S,p->lchild);
			p = p->lchild;
		}//while
		Pop(&S,&p);
		if(!Visit(p->data)) return ERROR;	//访问结点
		p = p->rchild;
		if(p) Push(&S,p);		//向右一步
	}//while
	return OK;
}//InOrderTraverse_NRecur_0

Status InOrderTraverse_NRecur_1(BiTree T,Status (*Visit)(TElemType)){
	//中序遍历二叉树T的非递归算法1，对每个数据元素调用函数Visit
	BiTree p;
	SqStack S;
	InitStack(&S); Push(&S,T);	//根指针进栈
	while(!StackEmpty(S)){
		while(GetTop(S,&p) && p) Push(&S,p->lchild);//向左走到尽头
		Pop(&S,&p);				//空指针退栈
		if(!StackEmpty(S)){		//访问结点，向右一步
			Pop(&S,&p);
			if(!Visit(p->data)) return ERROR;
			Push(&S,p->rchild);
		}//if
	}//while
	return OK;
}//InOrderTraverse_NRecur_1

Status InOrderTraverse_NRecur_2(BiTree T,Status (*Visit)(TElemType)){
	//中序遍历二叉树T的非递归算法2，对每个数据元素调用函数Visit
	BiTree p = T;
	SqStack S;
	InitStack(&S);
	while(p || !StackEmpty(S)){
		if(p){		//根指针进栈，遍历左子树
			Push(&S,p);
			p = p->lchild;
		}//if
		else{		//根指针退栈，访问根节点，遍历右子树
			Pop(&S,&p); if(!Visit(p->data)) return ERROR;
			p = p->rchild;
		}//else
	}//while
	return OK;
}//InOrderTraverse_NRecur_2

Status PostOrderTraverse_Recur(BiTree T,Status (*Visit)(TElemType)){
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

Status PostOrderTraverse_NRecur(BiTree T,Status (*Visit)(TElemType)){	//★
	//后序遍历T的非递归算法，对每个结点调用函数Visit一次且仅一次。
	//一旦Visit()失败，则操作失败。
	BiTNode *cur = T , *last = T;	//cur指示当前检查访问的结点，last指示上次访问的结点
	SqStack S;
	InitStack(&S);
	while(cur){
		Push(&S,cur);
		cur = cur->lchild;			//向左走到尽头
	}
	while(!StackEmpty(S)){
			Pop(&S, &cur);							//暂时取出结点
			if(!cur->rchild || cur->rchild==last){	//右孩子已经访问
				if(!Visit(cur->data)) return ERROR;	//访问结点
				last = cur;
			}
			else{									//右孩子未被访问
				Push(&S,cur);						//放回结点
				cur = cur->rchild;					//向右一步
				while(cur){
					Push(&S,cur);
					cur = cur->lchild;
				}
			}
	}
	return OK;
}//PostOrderTraverse_NRecur

Status LevelOrderTraverse(BiTree T,Status (*Visit)(TElemType)){	//★
	//层序遍历T，对每个结点调用函数Visit一次且仅一次。
	//一旦Visit()失败，则操作失败。
	BiTree tmp[100];		//暂存要访问的结点指针
	int i = 0,j = 0;
	if(T) tmp[j++] = T;		//存入根指针
	while(i<j){
		if(!(Visit(tmp[i]->data))) return ERROR;		//访问结点
		if(tmp[i]->lchild) tmp[j++] = tmp[i]->lchild;	//存入当前访问结点的左孩子
		if(tmp[i]->rchild) tmp[j++] = tmp[i]->rchild;	//存入当前访问结点的右孩子
		++i;
	}
	return OK;
}//LevelOrderTraverse

#endif
