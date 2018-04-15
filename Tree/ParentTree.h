
#ifndef PARENTTREE_H
#define PARENTTREE_H

#include <stdio.h>
#include "status.h"

// - - - - - 树的双亲链表（静态链表）存储表示 - - - - -
// 树中结点始终按层序有序排列
typedef char TElemType; //存储数据类型定为char
#define MAX_TREE_SIZE 100
typedef struct PTNode{  //结点结构
    TElemType   data;
    int         parent; //双亲位置域
}PTNode;
typedef struct {        //树结构
    PTNode nodes[MAX_TREE_SIZE];
    int r,n;            //根的位置和结点数
}PTree;

// - - - - - 基本函数的算法描述 - - - - -
Status InitTree(PTree *T){
    //构造空树T
    int i;
    for(i=0;i<MAX_TREE_SIZE;++i){
        (*T).nodes[i].data = ' ';
        (*T).nodes[i].parent = -1;
    }
    (*T).n = 0;
    (*T).r = -1;
    return OK;
}//InitTree

Status DestroyTree(PTree *T){
    //销毁树T
    int i;
    for(i=0;i<MAX_TREE_SIZE;++i){
        (*T).nodes[i].parent = -1;
    }
    (*T).n = 0;
    (*T).r = -1;
    return OK;
}//DestroyTree

Status CreateTree(PTree *T){
	//按层序次序输入树中结点的值（一个字符），空格字符表示空树，构造树T
    TElemType e;
    printf("Enter the data of root:");
    scanf("%c",&e);
    if(e != ' '){
        int i = 1 , j = 0;
        (*T).nodes[0].data = e;
        (*T).r = 0;
        getchar();      //吞掉回车符
        while(j < i && j < MAX_TREE_SIZE){  //层序建立树T
            printf("Enter the data of '%c' 's children:",(*T).nodes[j].data);
            e = getchar();
            while(e != '\n' && e != ' '){
                (*T).nodes[i].data = e;
                (*T).nodes[i++].parent = j;
                e = getchar();
            }
            if(e == ' ') getchar();         //输入空格后仍需要吞掉一个回车符
            ++j;
        }
        (*T).n = j;     //树的结点数
    }
    printf("Succeeded!\n");
    return OK;
}//CreateTree

Status ClearTree(PTree *T){
    //将树T清为空树
    int i;
    for(i=0;i<MAX_TREE_SIZE;++i){
        (*T).nodes[i].data = ' ';
        (*T).nodes[i].parent = -1;
    }
    (*T).n = 0;
    (*T).r = -1;
    return OK;
    //清空树的操作与构造空树的操作相同
}//ClearTree

Status TreeEmpty(PTree T){
    //若树T为空树，则返回TRUE，否则FALSE
    if(T.n == 0) return TRUE;
    else return FALSE;
}//TreeEmpty

int TreeDepth(PTree T){
    //返回树T的深度
    int depth = 0;
    int i = T.n - 1;    //层序建立的最后存储的结点在最大层★
    if(T.n != 0){
        ++depth;
        while(T.nodes[i].parent != -1){
            ++depth;
            i = T.nodes[i].parent;  //向上层计算
        }
    }
    return depth;
}//TreeDepth

TElemType Root(PTree T){
    //返回T的根，若根不存在，返回“空”
    return T.nodes[T.r].data;
}//Root

TElemType Value(PTree T,int p){
    //返回层序遍历T得到的第p个结点的值，p从0开始
    return T.nodes[p].data;
}//Value

Status Assign(PTree *T,int p,TElemType value){
    //用value更新层序遍历T得到的的第p个结点的值，p从0开始
    if((*T).nodes[p].data == ' ') return ERROR;
    else{
        (*T).nodes[p].data = value;
        return OK;
    }
}//Assign

TElemType Parent(PTree T,int p){
    //p为层序遍历得到的结点序号（从0开始），
    //若p是T的非根结点，则返回它的双亲，否则返回“空”
    int i = T.nodes[p].parent;
    if(i == -1) return ' ';
    else return T.nodes[i].data;
}//Parent

TElemType LeftChild(PTree T,int p){
    //p为层序遍历得到的结点序号（从0开始），
    //若p是T的非叶子结点，则返回它的最左孩子，否则返回“空”
    int j = p + 1;
    while(j < T.n && T.nodes[j].parent != p){
        ++j;
    }
    if(j >= T.n) return ' ';
    else return T.nodes[j].data;
}//LeftChild

TElemType RightSibling(PTree T,int p){
    //p为层序遍历得到的结点序号（从0开始），
    //若p有右兄弟，则返回它的右兄弟，否则返回“空”
    int j = p + 1;
    if(T.nodes[j].parent != T.nodes[p].parent || j >= T.n)
        return ' ';
    else return T.nodes[j].data;
}//RightSibling

Status InsertNode(PTree *T,int p,int i,TElemType e){
    //p为层序遍历得到的结点序号（从0开始），插入元素e为p所指结点的第i棵子树的根结点
    if((*T).n + 1 > MAX_TREE_SIZE) return ERROR;
    int j = p + 1;
    int k = (*T).n - 1;
    while((*T).nodes[j].parent < p && (*T).nodes[j].parent != -1) ++j;  //寻找插入位置下标★
    j += i - 1;                                                         //注意是否越界
    while(k >= j){
        if((*T).nodes[k].parent >= j)       //父结点已经移动★
                (*T).nodes[k+1].parent = (*T).nodes[k].parent + 1;
        else
            (*T).nodes[k+1].parent = (*T).nodes[k].parent;
        (*T).nodes[k+1].data = (*T).nodes[k].data;
        --k;
    }
    (*T).nodes[j].data = e;
    (*T).nodes[j].parent = p;
    ++(*T).n;       //树的结点数+1
    return OK;
}//InsertNode

Status InsertChild(PTree *T,int p,int i,PTree c){
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
            /* 此处等同于：
             * j = tmpnode[c.nodes[k].parent] + 1;  //从父节点的位置+1开始
             * while((*T).nodes[j].parent < tmpnode[c.nodes[k].parent]) ++j;//寻找其第一个孩子
             * j += cnt - 1;
             */

            tmpnode[k] = j;           //存储在T中的插入位置
        }
    }
    return OK;
}//InsertChild

Status DeleteChild(PTree *T,int p,int i){
    //p为层序遍历得到的结点序号（从0开始），删除T中p所指结点的第i棵子树
    int k;
    int q = p + 1;
    while(q < (*T).n && (*T).nodes[q].parent != p) ++q; //寻找p的第一棵子树
    if(q == (*T).n) return ERROR;                       //p的子树不存在
    q += i - 1;
    if((*T).nodes[q].parent != p) return ERROR;         //p的第i棵子树不存在
    (*T).nodes[q].data = ' ';               //将要删除的结点都置为“空”
    while(q < (*T).n){                      //从第一个要删除的结点起开始层序遍历
        if((*T).nodes[q].data == ' '){      //是要删除的结点
            k = q;
            while(k + 1 < (*T).n){
                if((*T).nodes[k+1].parent > q)          //父结点已经移动
                    (*T).nodes[k].parent = (*T).nodes[k+1].parent - 1;
                else
                    (*T).nodes[k].parent = (*T).nodes[k+1].parent;

                if((*T).nodes[k+1].parent == q)
                    (*T).nodes[k].data = ' ';           //标记要删除结点的孩子
                else
                    (*T).nodes[k].data = (*T).nodes[k+1].data;
                ++k;
            }//while
            (*T).nodes[k].data = ' ';
			(*T).nodes[k].parent = -1;      //最后一个结点置空
            --(*T).n;                       //树的结点数减1
        }//if
        if((*T).nodes[q].data != ' ') ++q;  //若q为空则继续删除
    }//while
    return OK;
}//DeleteChild

Status LevelOrderTraverse(PTree T,Status (*visit)(TElemType)){
    //层序遍历树T，对T的每个结点调用函数visit()一次且至多一次
    //一旦visit()失败，则操作失败
    int i;
    for(i=T.r;i<T.n;++i)
        if(!visit(T.nodes[i].data)) return ERROR;
    printf("\n");
    return OK;
}//LevelOrderTraverse

#endif // !PARENTTREE_H