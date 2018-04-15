
#ifndef CHILDTREE_H
#define CHILDTREE_H

#include <stdio.h>
#include <stdlib.h>
#include "status.h"

// - - - - - 树的孩子链表（带双亲）存储表示 - - - - -
// 树中结点始终按层序有序排列
typedef char TElemType;		//存储数据类型定为char
#define MAX_TREE_SIZE 100	//树的最大节点数
typedef struct CTNode{		//孩子结点
	int child;
	struct CTNode *next;
}CTNode,*ChildPtr;
typedef struct {
	int parent;
	TElemType data;
	ChildPtr firstchild;	//孩子链表头指针
}CTBox;
typedef struct {
	CTBox nodes[MAX_TREE_SIZE];
	int n , r;				//结点数和根的位置；
}CTree;

// - - - - - 基本操作的算法描述 - - - - -
Status InitTree(CTree *T){
	//构造空树T
	int i;
	for(i=0;i<MAX_TREE_SIZE;++i){
		(*T).nodes[i].parent = -1;
		(*T).nodes[i].data = ' ';
		(*T).nodes[i].firstchild = NULL;
	}
	(*T).n = 0;
	(*T).r = -1;
	return OK;
}//InitTree

Status DestroyTree(CTree *T){
	//销毁树T
	int i;
	ChildPtr p,q;
	for(i=0;i<(*T).n;++i){
		(*T).nodes[i].parent = -1;
		(*T).nodes[i].data = ' ';
		p = (*T).nodes[i].firstchild;
		while (p){
			q = p->next;
			free(p);		//销毁孩子结点
			p = q;
		}
		(*T).nodes[i].firstchild = NULL;
	}
	(*T).n = 0;
	(*T).r = -1;
	return OK;
	//销毁树的操作与清空树的操作相同
}//DestroyTree

Status CreateTree(CTree *T){
	//按层序次序，依次输入树T各个结点的值，空格字符表示空结点，建立树T
	TElemType e;
	printf("Enter the data of root:");
	scanf("%c",&e);
	if(e != ' '){
		int i = 1 , j = 0;
		ChildPtr p = NULL , q = NULL;
		(*T).nodes[0].data = e;
		(*T).r = 0;
		getchar();		//吞掉回车符
		while (j < i && j < MAX_TREE_SIZE){				//层序建立树T
			printf("Enter the data of '%c' 's children:",(*T).nodes[j].data);
			e = getchar();
			while (e != ' ' && e != '\n'){
				p = (ChildPtr)malloc(sizeof(CTNode));	//在双亲的孩子链表中建立结点
				if(!p) exit(OVERFLOW);
				p->child = i;
				p->next = NULL;
				if(!(*T).nodes[j].firstchild)
					(*T).nodes[j].firstchild = p;
				else {
					q = (*T).nodes[j].firstchild;
					while (q->next) q = q->next;
					q->next = p;
				}
				(*T).nodes[i].data = e;					//在树中插入元素
				(*T).nodes[i++].parent = j;
				e = getchar();
			}
			if(e == ' ') getchar();     				//输入空格后仍需要吞掉一个回车符
			++j;
		}
		(*T).n = j;		//树的结点数
	}
	printf("Succeeded!\n");
	return OK;
}//CreateTree

Status ClearTree(CTree *T){
    //将树T清为空树
	int i;
	ChildPtr p,q;
	for(i=0;i<(*T).n;++i){
		(*T).nodes[i].data = ' ';
		(*T).nodes[i].parent = -1;
		p = (*T).nodes[i].firstchild;
		while (p){
			q = p->next;
			free(p);		//销毁孩子链表结点
			p = q;
		}
		(*T).nodes[i].firstchild = NULL;
	}
	(*T).n = 0;
	(*T).r = -1;
	return OK;
}//ClearTree

Status TreeEmpty(CTree T){
    //若树T为空树，则返回TRUE，否则FALSE
    if(T.n == 0) return TRUE;
    else return FALSE;
}//TreeEmpty

int TreeDepth(CTree T){
    //返回树T的深度
    int depth = 0;
    int i = T.n - 1;    //层序建立的最后存储的结点在最大层
    if(T.n != 0){
        ++depth;
        while(T.nodes[i].parent != -1){
            ++depth;
            i = T.nodes[i].parent;  //向上层计算
        }
    }
    return depth;
}//TreeDepth

TElemType Root(CTree T){
    //返回T的根，若根不存在，返回“空”
    return T.nodes[T.r].data;
}//Root

TElemType Value(CTree T,int p){
    //返回层序遍历T得到的第p个结点的值，p从0开始
    return T.nodes[p].data;
}//Value

Status Assign(CTree *T,int p,TElemType value){
    //用value更新层序遍历T得到的的第p个结点的值，p从0开始
    if((*T).nodes[p].data == ' ') return ERROR;
    else{
        (*T).nodes[p].data = value;
        return OK;
    }
}//Assign

TElemType Parent(CTree T,int p){
    //p为层序遍历得到的结点序号（从0开始），
    //若p是T的非根结点，则返回它的双亲，否则返回“空”
    int i = T.nodes[p].parent;
    if(i == -1) return ' ';
    else return T.nodes[i].data;
}//Parent

TElemType LeftChild(CTree T,int p){
    //p为层序遍历得到的结点序号（从0开始），
    //若p是T的非叶子结点，则返回它的最左孩子，否则返回“空”
	ChildPtr q = T.nodes[p].firstchild;
	if(q) return T.nodes[q->child].data;
	else  return ' ';
}//LeftChild

TElemType RightSibling(CTree T,int p){
    //p为层序遍历得到的结点序号（从0开始），
    //若p有右兄弟，则返回它的右兄弟，否则返回“空”
	ChildPtr chd;
	int par = T.nodes[p].parent;
	if(par == -1) return ' ';		//头结点无兄弟
	else chd = T.nodes[par].firstchild;
	while(chd->next){
		if(chd->child == p)
			return T.nodes[chd->next->child].data;
		else chd = chd->next;
	}
	return ' ';
}//RightSibling

Status InsertNode(CTree *T,int p,int i,TElemType e){
    //p为层序遍历得到的结点序号（从0开始），插入元素e为p所指结点的第i棵子树的根结点
    if((*T).n + 1 > MAX_TREE_SIZE) return ERROR;

	//寻找插入位置下标★
	//注意是否越界
    int j = p + 1;
    while((*T).nodes[j].parent < p && (*T).nodes[j].parent != -1) ++j;
    j += i - 1;

	//在树中插入元素，并移动后面的元素
    int k = (*T).n - 1;
	int par; ChildPtr chd;
    while(k >= j){
		par = (*T).nodes[k].parent;
		chd = (*T).nodes[par].firstchild;
		while(chd->child != k) chd = chd->next;
		++chd->child;					//更改父结点的孩子链表
        if((*T).nodes[k].parent >= j)	//父结点已经移动★
                (*T).nodes[k+1].parent = (*T).nodes[k].parent + 1;
        else
            (*T).nodes[k+1].parent = (*T).nodes[k].parent;
        (*T).nodes[k+1].data = (*T).nodes[k].data;
		(*T).nodes[k+1].firstchild = (*T).nodes[k].firstchild;
        --k;
    }
    (*T).nodes[j].data = e;
    (*T).nodes[j].parent = p;
	(*T).nodes[j].firstchild = NULL;
    ++(*T).n;		//树的结点数+1

	//在双亲结点的孩子链表中插入结点
	ChildPtr q = (*T).nodes[p].firstchild;
	ChildPtr s = (ChildPtr)malloc(sizeof(CTNode));	//建立孩子结点
	if(!s) exit(OVERFLOW);
	s->child = j;
	if(i == 1){		//作为最左孩子结点插入
		s->next = q;
		(*T).nodes[p].firstchild = s;
	}
	else{			//寻找左兄弟孩子结点
		for (int k = 1; k < i - 1; ++k) q = q->next;
		s->next = q->next;
		q->next = s;
	}
    return OK;
}//InsertNode

Status InsertChild(CTree *T,int p,int i,CTree c){
	//p为层序遍历得到的结点序号（从0开始），插入非空树c为p所指结点的第i棵子树
	if((*T).n + c.n > MAX_TREE_SIZE) return ERROR;
    int tmpnode[MAX_TREE_SIZE];         //暂存c的结点插入在T中的位置
    int q = 0 , j = p + 1;
    int cnt;                            //记录结点的孩子数目
    if(Root(c) != ' '){
        InsertNode(T,p,i,Root(c));      //插入c的根结点
        while((*T).nodes[j].parent < p && (*T).nodes[j].parent != -1) ++j;
        j += i - 1;                     //计算插入位置下标
        tmpnode[q++] = j;               //存储插入位置
        for (int k = 1; k < c.n; k++){
            if(c.nodes[k].parent != c.nodes[k-1].parent) cnt = 1;           //不同结点的孩子
            else ++cnt;
            InsertNode(T,tmpnode[c.nodes[k].parent],cnt,c.nodes[k].data);
            while((*T).nodes[j].parent < tmpnode[c.nodes[k].parent]) ++j;   //初始的j指示上一个结点存储位置
            if(cnt != 1) ++j;           //同一双亲的结点必为顺序存储
            tmpnode[k] = j;           	//存储在T中的插入位置
        }
    }
    return OK;
}

Status DeleteChild(CTree *T,int p,int i){
    //p为层序遍历得到的结点序号（从0开始），删除T中p所指结点的第i棵子树
	int j,k,q;
	int par;
	ChildPtr chd;
	ChildPtr s = (*T).nodes[p].firstchild;
	if(!s) return ERROR;						//p的子树不存在
	for (j = 1; j < i; ++j){
		if(!s->next) return ERROR;				//p的第i棵子树不存在
		else s = s->next;
	}
	q = s->child;
    (*T).nodes[q].data = ' ';                   //将要删除的结点都置为“空”

	//删除双亲的孩子链表中结点
	par = (*T).nodes[q].parent;
	ChildPtr priorc,nextc;
	priorc = nextc = (*T).nodes[par].firstchild;
	while (nextc->child != q){
		priorc = nextc;
		nextc = nextc->next;
	}
	if(nextc == priorc){
		(*T).nodes[par].firstchild = priorc->next;
		priorc = NULL;
	}
	else priorc->next = nextc->next;
	free(nextc); nextc = NULL;

    while(q < (*T).n){             	//从第一个要删除的结点起开始层序遍历
        if((*T).nodes[q].data == ' '){          //是要删除的结点

			//delete and free child list of deleted node
			priorc = nextc = (*T).nodes[q].firstchild;
			while(priorc){
				nextc = priorc->next;
				free(priorc);
				priorc = nextc;
			}
			(*T).nodes[q].firstchild = NULL;

            k = q;
            while(k + 1 < (*T).n){
				if((*T).nodes[k+1].parent == q){
					(*T).nodes[k].data = ' ';			//标记要删除结点的孩子
				}
				else{
					(*T).nodes[k].data = (*T).nodes[k+1].data;
					if((*T).nodes[k+1].parent > q)      //父结点已经移动
						(*T).nodes[k].parent = (*T).nodes[k+1].parent - 1;
					else
						(*T).nodes[k].parent = (*T).nodes[k+1].parent;
					if((*T).nodes[k].data != ' '){
						par = (*T).nodes[k].parent;
						chd = (*T).nodes[par].firstchild;
						while(chd->child != k + 1) chd = chd->next;
						--chd->child;					//更改父结点的孩子链表
					}
				}
				(*T).nodes[k].firstchild = (*T).nodes[k+1].firstchild;
				++k;
			}//while
			(*T).nodes[k].data = ' ';
			(*T).nodes[k].parent = -1;
			(*T).nodes[k].firstchild = NULL;			//最后一个结点置空
			--(*T).n;       							//树的结点数减1
		}//if
		if((*T).nodes[q].data != ' ') ++q;  			//若q为空则继续删除
	}//while
	return OK;
}//DeleteChild

Status LevelOrderTraverse(CTree T,Status (*visit)(TElemType)){
    //层序遍历树T，对T的每个结点调用函数visit()一次且至多一次
    //一旦visit()失败，则操作失败
    int i;
    for(i=T.r;i<T.n;++i)
        if(!visit(T.nodes[i].data)) return ERROR;
    printf("\n");
    return OK;
}//LevelOrderTraverse

#endif // !CHILDTREE_H