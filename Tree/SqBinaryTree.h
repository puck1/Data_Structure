
#ifndef SQBINARYTREE_H
#define SQBINARYTREE_H

#include <stdio.h>
#include <math.h>
#include "status.h"

// - - - - - 二叉树的顺序存储表示 - - - - -
//一般适用于完全二叉树
#define		TElemType		char			//顺序二叉树存储数据类型定为char型
#define 	MAX_TREE_SIZE 	100				//二叉树的最大结点数
typedef TElemType SqBiTree[MAX_TREE_SIZE];	//0号单元存储根节点

typedef struct {
	int level;		//结点所在层次（从0开始）
	int order;		//结点在层次的序号（从0开始）
}Position;			//完全二叉树中结点位置

// - - - - - 基本操作的算法描述 - - - - -
Status InitBiTree(SqBiTree T){
	//构造空二叉树T
	int i;
	for(i = 0; i < MAX_TREE_SIZE; ++i)
		T[i] = ' ';
	return OK;
}//InitBiTree

Status CreateBiTree(SqBiTree T,int i){	//★
	//从固定的结点开始，按先序次序输入二叉树中结点的值（一个字符），
	//空格字符表示空树，构造二叉树T（在此之前必须先构造空二叉树）
	TElemType e = getchar();
	if(e != ' '){
		T[i] = e;						//输入根结点数据
		if(2 * i + 1 < MAX_TREE_SIZE)	//递归构造左子树
			CreateBiTree(T,2 * i + 1);
		if(2 * i + 2 < MAX_TREE_SIZE)	//递归构造右子树
			CreateBiTree(T,2 * i + 2);
	}
	return OK;
}//CreateBiTree

Status ClearBiTree(SqBiTree T){
	//将二叉树T清为空树
	int i;
	for(i = 0; i < MAX_TREE_SIZE; ++i)
		T[i] = ' ';
	return OK;
	//清空与构造空二叉树操作相同
}//ClearBiTree

Status BiTreeEmpty(SqBiTree T){
	//若二叉树T为空树，则返回TRUE，否则返回FALSE
	if(T[0] == ' ') return TRUE;
	else return FALSE;
}//BiTreeEmpty

int BiTreeDepth(SqBiTree T,int i){
	//返回以某一结点为根的二叉树T的深度
	int ld,rd;
	if(T[i] == ' ')
		return 0;
	else{
		ld = BiTreeDepth(T,2 * i + 1);		//递归求左子树深度
		rd = BiTreeDepth(T,2 * i + 2);		//递归求右子树深度
		return (ld > rd ? ld : rd) + 1;		//深度为最大子树深度+1
	}
}//BiTreeDepth

TElemType Root(SqBiTree T){
	//若二叉树T的根存在，返回二叉树的根，否则返回"空"
	return T[0];
}//Root

TElemType Value(SqBiTree T,Position p){
	//p为二叉树T中某结点的位置，返回其值
	return T[(int)pow(2,p.level - 1) + p.order - 2];
}//Value

Status Assign(SqBiTree T,Position p,TElemType e){
	//p为二叉树中某结点的位置，用e更新其值
	T[(int)pow(2,p.level - 1) + p.order - 2] = e;
	return OK;
}//Assign

TElemType Parent(SqBiTree T,Position p){
	//若p为二叉树中某非根节点的位置，则返回它的双亲，否则返回"空"
	int i = pow(2,p.level - 1) + p.order - 2;		//该结点的数组下标
	if(i == 0) return ' ';
	else if(i % 2 == 1) return T[i / 2];
	else return T[i / 2 - 1];
}//Parent

TElemType LeftChild(SqBiTree T,Position p){
	//返回p所指结点的左孩子，若p无左孩子，则返回"空"
	int i = pow(2,p.level - 1) + p.order - 2;		//该结点的数组下标
	if(2 * i + 1 >= MAX_TREE_SIZE) return ' ';		//孩子节点下标越界
	else return T[2 * i + 1];
}//LeftChild

TElemType RightChild(SqBiTree T,Position p){
	//返回p所指结点的右孩子，若p无右孩子，则返回"空"
	int i = pow(2,p.level - 1) + p.order - 2;		//该结点的数组下标
	if(2 * i + 2 >= MAX_TREE_SIZE) return ' ';		//孩子节点下标越界
	else return T[2 * i + 2];
}//RightChild

TElemType LeftSibling(SqBiTree T,Position p){
	//返回p所指结点的左兄弟，若p无左兄弟，则返回"空"
	if(p.order % 2 == 1) return ' ';	//p是左孩子
	else return T[(int)pow(2,p.level - 1) + p.order - 3];
}//LeftSibling

TElemType RightSibling(SqBiTree T,Position p){
	//返回p所指结点的右兄弟，若p无右兄弟，则返回"空"
	if(p.order % 2 == 0) return ' ';
	else return T[(int)pow(2,p.level - 1) + p.order - 1];
}//RightSibling

Status InsertChild(SqBiTree T,Position p,int LR,SqBiTree c){
	//根据LR为0或1，插入c为T中p所指节点的左或右子树。p所指节点的原有左
	//或右子树则成为c的右子树。
	//*顺序结构难以实现。
}//InsertChild

Status DeleteChild(SqBiTree T,Position p,int LR){
	//根据LR为0或1，删除T中p所指结点的左或右子树。
	//*顺序结构难以实现。
}//DeleteChild

Status PreOrderTraverse_Recur(SqBiTree T,Status (*Visit)(TElemType),int i){
	//先序遍历T的递归算法，对每个结点调用函数Visit一次且仅一次。一旦Visit()失败，则操作失败。
	int ret = 1;
	if(T[i] != ' '){
		if(!Visit (T[i]))
			ret = 0;
		if(2 * i + 1 < MAX_TREE_SIZE)		//左孩子结点未越界
			if(!PreOrderTraverse_Recur(T,Visit,2 * i + 1))
				ret = 0;
		if(2 * i + 2 < MAX_TREE_SIZE)		//右孩子结点未越界
			if(!PreOrderTraverse_Recur(T,Visit,2 * i + 2))
				ret = 0;
	}
	return ret;
}//PreOrderTraverse_Recur

Status InOrderTraverse_Recur(SqBiTree T,Status (*Visit)(TElemType),int i){
	//中序遍历T的递归算法，对每个结点调用函数Visit一次且仅一次。一旦Visit()失败，则操作失败。
	int ret = 1;
	if(T[i] != ' '){
		if(2 * i + 1 < MAX_TREE_SIZE)		//左孩子结点未越界
			if(!InOrderTraverse_Recur(T,Visit,2 * i + 1))
				ret = 0;
		if(!Visit (T[i]))
			ret = 0;
		if(2 * i + 2 < MAX_TREE_SIZE)		//右孩子结点未越界
			if(!InOrderTraverse_Recur(T,Visit,2 * i + 2))
				ret = 0;
	}
	return ret;
}//InOrderTraverse_Recur

Status PostOrderTraverse_Recur(SqBiTree T,Status (*Visit)(TElemType),int i){
	//后序遍历T的递归算法，对每个结点调用函数Visit一次且仅一次。一旦Visit()失败，则操作失败。
	int ret = 1;
	if(T[i] != ' '){
		if(2 * i + 1 < MAX_TREE_SIZE)		//左孩子结点未越界
			if(!PostOrderTraverse_Recur(T,Visit,2 * i + 1))
				ret = 0;
		if(2 * i + 2 < MAX_TREE_SIZE)		//右孩子结点未越界
			if(!PostOrderTraverse_Recur(T,Visit,2 * i + 2))
				ret = 0;
		if(!Visit (T[i]))
			ret = 0;
	}
	return ret;
}//PostOrderTraverse_Recur

Status LevelOrderTraverse(SqBiTree T,Status (*Visit)(TElemType)){
	//层序遍历T，对每个结点调用函数Visit一次且仅一次。一旦Visit()失败，则操作失败。
	int i;
	for(i = 0; i < MAX_TREE_SIZE; ++i){
		if(T[i] != ' ')
			if(!Visit (T[i])) return ERROR;
	}
	return OK;
}//LevelOrderTraverse

#endif
