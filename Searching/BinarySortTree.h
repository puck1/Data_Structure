/** @workspace/Searching/BinarySortTree.h
 *  This file includes an abstract data structure BINARY SORT TREE.
 *  A binary sort tree is an empty tree or a tree with following propeties:
 *  (1)If its left child is not empty,all the values of nodes on its left subtree
 *     are smaller than its root node's;
 *  (2)If its right child is not empty,all the values of nodes on its right subtree
 *     are bigger than its root node's;
 *  (3)Its left subtree and right subtree are also binary sort trees.
 *  Also includes algorithmes of searching,inserting,deleting and traversing of BST.
 */

#ifndef BINARYSORTTREE_H
#define BINARYSORTTREE_H

#include "STypedef.h"
#include "../General/status.h"
typedef STElemType TElemType;
#include "../Tree/BiLinkBinaryTree.h"

// - - - - - 需要调用的函数原型声明 - - - - -
Status Delete(BiTree *p);

// - - - - - 基本操作的算法描述 - - - - -
void InitBST(BiTree *T){
    //构造一个空的二叉排序树T
    (*T) = NULL;
}//InitBST

void DestroyBST(BiTree *T){
    //销毁二叉排序树T
    if (*T){
        DestroyBST(&((*T)->lchild));
        DestroyBST(&((*T)->rchild));
        free(*T); *T = NULL;
    }//if
}//DestroyBST

BiTree SearchBST_1(BiTree T,KeyType key){
    //在根指针T所指二叉排序树中递归地查找某关键字等于key的数据元素，
    //若查找成功，则返回指向该数据元素结点的指针，否则返回空指针
    if (!T || EQ(key,T->data.key)) return T;        //查找结束
    else if (LT(key,T->data.key)) return SearchBST_1(T->lchild,key);
                                                    //在左子树中继续查找
    else return SearchBST_1(T->rchild,key);         //在右子树中继续查找
}//SearchBST_1

Status SearchBST_2(BiTree T,KeyType key,BiTree f,BiTree *p){
    //在根指针T所指二叉排序树中递归地查找其关键字等于key的数据元素，若查找成功，
    //则指针p指向该数据元素结点，并返回TRUE，否则指针p指向查找路径上访问的
    //最后一个结点并返回FALSE，指针f指向T的双亲，其初始调用值为NULL
    if (!T) {*p = f; return FALSE;}                                         //查找不成功
    else if (EQ(key,T->data.key)) {*p = T; return TRUE;}                    //查找成功
    else if (LT(key,T->data.key)) return SearchBST_2(T->lchild,key,T,p);    //在左子树中继续查找
    else return SearchBST_2(T->rchild,key,T,p);                             //在右子树中继续查找
}//SearchBST_2

Status InsertBST(BiTree *T,STElemType e){
    //当二叉排序树T中不存在关键字等于e.key的数据元素时，插入e并返回TRUE，
    //否则返回FALSE
    BiTree p, s;
    if (!SearchBST_2(*T,e.key,NULL,&p)){        //查找不成功
        if (!(s = (BiTree)malloc(sizeof(BiTNode)))) return ERROR;
        s->data = e; s->lchild = s->rchild = NULL;
        if (!p) *T = s;                         //被插结点s为新的根节点
        else if (LT(e.key,p->data.key)) p->lchild = s;  //被插结点s为左孩子
        else p->rchild = s;                     //被插结点s为右孩子
        return TRUE;
    }//if
    else return FALSE;                          //树中已有关键字相同的结点，不再插入
}//InsertBST

Status DeleteBST(BiTree *T,KeyType key){
    //若二叉排序树T中存在关键字等于key的数据元素，则删除该数据元素结点，
    //并返回TRUE；否则返回FALSE
    if (!(*T)) return FALSE;                               //不存在关键字等于key的数据元素
    else if (EQ(key,(*T)->data.key)) return Delete(T);  //找到关键字等于key的数据元素
    else if (LT(key,(*T)->data.key)) return DeleteBST(&((*T)->lchild),key);
    else return DeleteBST(&((*T)->rchild),key);
}//DeleteBST

Status Delete(BiTree *p){
    //从二叉排序树中删除结点p，并重接它的左或右子树
    BiTree q, s;
    if (!(*p)->rchild)          //右子树空则只需重接它的左子树
        { q = *p; *p = (*p)->lchild; free(q); }
    else if (!(*p)->lchild)     //只需重接它的右子树
        { q = *p; *p = (*p)->rchild; free(q); }
    else {      //左右子树均不空
        q = *p; s = (*p)->lchild;
        while (s->rchild) {q = s; s = s->rchild;}   //转左，然后向右到尽头
        (*p)->data = s->data;                       //s指向被删结点的“前驱”
        if (q != *p) q->rchild = s->lchild;         //重接q所指结点的右子树
        else q->lchild = s->lchild;                 //重接q所指结点的左子树
        free(s);
        /** Another method：
         *  s = (*p)->lchild;
         *  while (s->rchild) s = s->rchild;
         *  s->rchild = (*p)->rchild;
         *  q = *p; p = (*p)->lchild; free(q);
         */
    }
    return TRUE;
}//Delete

Status InorderTraverseBST(BiTree T,Status Visit(STElemType)){
    //中序遍历二叉排序树T，得到一个关键字的有序序列
    if (T){
        if (InorderTraverseBST(T->lchild,Visit))
            if (Visit(T->data))
                if (InorderTraverseBST(T->rchild,Visit))
                    return OK;
        return ERROR;
    }//if
    else return OK;
}//InorderTraverseBST

void CreateBST(BiTree *T){
    //输入关键字序列，建立一颗二叉排序树T
    int i, n;
    STElemType e;
    InitBST(T);
    printf("Enter number of elements:"); scanf("%d%*c",&n);
    printf("Enter all elements' keys seperated by delimiter:");
    for (i = 1; i <= n; ++i){
        scanf("%d%*c",&(e.key));
        InsertBST(T,e);
    }
    printf("Succeeded!\n");
}//CreateBST

#endif // !BINARYSORTTREE_H
