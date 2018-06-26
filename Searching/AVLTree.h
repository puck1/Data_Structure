/** @workspace/Searching/AVLTree.h
 *  This file includes a kind of abstract data structure
 *  BALANCED BINARY TREE or HEIGHT-BALANCED TREE,also called AVL TREE.
 *  AVL Tree is named after its inventors,G. M. Adelson-Velsky and E. M. Landis.
 *  An AVL Tree is an empty tree or a tree with such propeties:
 *  Its left subtree and right subtree are also AVL Tree and the absolute value of difference
 *  in depth between its left subtree and right one is not more than 1.
 */

#ifndef AVLTREE_H
#define AVLTREE_H

#include <stdio.h>
#include <stdlib.h>
#include "STypedef.h"
#include "../General/status.h"

// - - - - - 平衡二叉树结构类型定义 - - - - -
typedef struct BBSTNode{
    STElemType      data;
    int             bf;                 //结点的平衡因子Balance Factor
    struct BBSTNode *lchild, *rchild;   //左、右孩子指针
}BBSTNode, *BBSTree;
#define LH  +1      //左高
#define EH   0      //等高
#define RH  -1      //右高

// - - - - - 需要调用的函数原型声明 - - - - -
void R_Rotate(BBSTree *p);
void L_Rotate(BBSTree *p);
void LeftBalance(BBSTree *T);
void RightBalance(BBSTree *T);
void Delete(BBSTree *p,bool *shorter);

// - - - - - 基本操作的算法描述 - - - - -
void InitAVL(BBSTree *T){
    //构造一个空的平衡二叉树*T
    (*T) = NULL;
}//InitAVL

void DestroyAVL(BBSTree *T){
    //销毁平衡二叉树T
    if (*T){
        DestroyAVL(&((*T)->lchild));
        DestroyAVL(&((*T)->rchild));
        free(*T); *T = NULL;
    }//if
}//DestroyAVL

BBSTree SearchAVL(BBSTree T,KeyType key){
    //在根指针T所指平衡二叉树中递归地查找某关键字等于key的数据元素，
    //若查找成功，则返回指向该数据元素结点的指针，否则返回空指针
    if (!T || EQ(key,T->data.key)) return T;        //查找结束
    else if (LT(key,T->data.key)) return SearchAVL(T->lchild,key);
                                                    //在左子树中继续查找
    else return SearchAVL(T->rchild,key);           //在右子树中继续查找
}//SearchAVL

Status InsertAVL(BBSTree *T,STElemType e,bool *taller){
    //若在平衡的二叉排序树T中不存在和e有相同关键字的结点，则插入一个数据元素为e的新结点，
    //并返回TRUE，否则返回FALSE。若因插入而使二叉排序树失去平衡，则做平衡旋转处理，
    //布尔变量taller反映T长高与否
    if (!(*T)){ //插入新结点，树“长高”，置*taller为TRUE
        if (!((*T) = (BBSTree)malloc(sizeof(BBSTNode)))) exit(OVERFLOW);
        (*T)->lchild = (*T)->rchild = NULL;
        (*T)->data = e; (*T)->bf = EH; *taller = TRUE;
    }//if
    else {
        if (EQ(e.key,(*T)->data.key))               //树中已存在和e有相同关键字的结点
            { *taller = FALSE; return FALSE; }      //则不再插入
        else if (LT(e.key,(*T)->data.key)) {        //应继续在*T的左子树中进行搜索
            if (!InsertAVL(&((*T)->lchild),e,taller)) return FALSE;     //未插入
            if (*taller)                            //已插入到*T的左子树中且左子树“长高”
                switch ((*T)->bf) {                 //检查*T的平衡度
                    case LH:                        //原本左子树比右子树高，需要作左平衡处理
                        LeftBalance(T); *taller = FALSE; break;
                    case EH:                        //原本左、右子树等高，现因左子树增高而使树增高
                        (*T)->bf = LH; *taller = TRUE; break;
                    case RH:                        //原本右子树比左子树高，现左、右子树等高
                        (*T)->bf = EH; *taller = FALSE; break;
                }//switch ((*T)->bf)
        }//else if
        else {                                      //应继续在*T的右子树中进行搜索
            if (!InsertAVL(&((*T)->rchild),e,taller)) return FALSE;     //未插入
            if (*taller)                            //已插入到*T的右子树中且右子树“长高”
                switch ((*T)->bf) {                 //检查*T的平衡度
                    case LH:                        //原本左子树比右子树高，现左、右子树等高
                        (*T)->bf = EH; *taller = FALSE; break;
                    case EH:                        //原本、左右子树等高，现因右子树增高而使树增高
                        (*T)->bf = RH; *taller = TRUE; break;
                    case RH:                        //原本右子树比左子树高，需要作右平衡处理
                        RightBalance(T); *taller = FALSE; break;
                }//switch ((*T)->bf)
        }//else
    }//else
    return TRUE;
}//InsertAVL

void LeftBalance(BBSTree *T){
    //对以指针*T所指结点为根的二叉树作左平衡旋转处理，本算法结束时，指针T指向新的根结点
    BBSTree lc, rd;
    lc = (*T)->lchild;          //lc指向*T的左子树根结点
    switch (lc->bf){            //检查*T的左子树的平衡度，并作相应平衡处理
        case LH:                //新结点插入在*T的左孩子的左子树上，要作单右旋处理
            (*T)->bf = lc->bf = EH;
            R_Rotate(T); break;
        case RH:                //新结点插入在*T的左孩子的右子树上，要做双旋处理
            rd = lc->rchild;    //rd指向*T的左孩子的右子树根
            switch (rd->bf){    //修改*T及其左孩子的平衡因子
                case LH: (*T)->bf = RH; lc->bf = EH;    break;
                case EH: (*T)->bf = lc->bf = EH;        break;  //（此时rd为新插入结点）★
                case RH: (*T)->bf = EH; lc->bf = LH;    break;
            }//switch (rd->bf)
            rd->bf = EH;
            L_Rotate(&((*T)->lchild));      //对*T的左子树作左旋平衡处理
            R_Rotate(T); break;             //对*T作右旋平衡处理
    }//switch (lc->bf)
}//LeftBalance

void RightBalance(BBSTree *T){
    //对以指针*T所指结点为根的二叉树作右平衡旋转处理，本算法结束时，指针T指向新的根结点
    BBSTree rc, ld;
    rc = (*T)->rchild;          //rc指向*T的右子树根结点
    switch (rc->bf){            //检查*T的右子树的平衡度，并作相应平衡处理
        case LH:                //新结点插入在*T的右孩子的左子树上，要作双旋处理
            ld = rc->lchild;    //ld指向*T的右孩子的左子树根
            switch (ld->bf){    //修改*T及其右孩子的平衡因子
                case LH: (*T)->bf = EH; rc->bf = RH;    break;
                case EH: (*T)->bf = rc->bf = EH;        break;  //（此时rc为新插入结点）★
                case RH: (*T)->bf = LH; rc->bf = EH;    break;
            }//switch (ld->bf)
            ld->bf = EH;
            R_Rotate(&((*T)->rchild));      //对*T的右子树作右旋平衡处理
            L_Rotate(T); break;             //对*T作左旋平衡处理
        case RH:                //新结点插入在*T的右孩子的右子树上，要做单左旋处理
            (*T)->bf = rc->bf = EH;
            L_Rotate(T); break;
    }//switch (rc->bf)
}//RightBalance

void R_Rotate(BBSTree *p){
    //对以*p所指结点为根的二叉排序树作右旋处理，处理之后*p指向新的树根结点，
    //即旋转处理之前的左子树的根结点
    BBSTree lc;
    lc = (*p)->lchild;                  //lc指向的*p的左子树根节点
    (*p)->lchild = lc->rchild;          //lc的右子树挂接为*p的左子树
    lc->rchild = *p; *p = lc;           //*p指向新的根结点
}//R_Rotate

void L_Rotate(BBSTree *p){
    //对以*p所指结点为根的二叉排序树作左旋处理，处理之后*p指向新的树根结点，
    //即旋转处理之前的右子树的根结点
    BBSTree rc;
    rc = (*p)->rchild;                  //rc指向的*p的右子树根节点
    (*p)->rchild = rc->lchild;          //rc的左子树挂接为*p的右子树
    rc->lchild = *p; *p = rc;           //*p指向新的根结点
}//L_Rotate

Status DeleteAVL(BBSTree *T,STElemType e,bool *shorter){
    //若在平衡的二叉排序树T中存在和e有相同关键字的结点，则删除该结点，并返回TRUE，
    //否则返回FALSE。若因删除而使二叉排序树失去平衡，则做平衡旋转处理，
    //布尔变量taller反映T长高与否
    BBSTree lc, rc;
    if (!(*T)) return FALSE;
    else {
        if (EQ(e.key,(*T)->data.key))               //待删除结点
            Delete(T,shorter);
        else if (LT(e.key,(*T)->data.key)) {        //应继续在*T的左子树中进行搜索
            if (!DeleteAVL(&((*T)->lchild),e,shorter)) return FALSE;    //未删除
            if (*shorter)                           //已在*T的左子树中删除且左子树“变矮”
                switch ((*T)->bf) {                 //检查*T的平衡度
                    case LH:                        //原本左子树比右子树高，现左、右子树等高，树变矮
                        (*T)->bf = EH; *shorter = TRUE; break;
                    case EH:                        //原本左、右子树等高，现左子树变矮，树高度不变
                        (*T)->bf = RH; *shorter = FALSE; break;
                    case RH:                        //原本右子树比左子树高，需要作平衡处理
                        rc = (*T)->rchild;
                        switch (rc->bf){            //检查*T右子树平衡度
                            case EH:                //对*T作单左旋处理，树高度不变
                                (*T)->bf = RH; rc->bf = LH; L_Rotate(T);
                                *shorter = FALSE; break;
                            default:                //对*T作右平衡处理，树变矮
                                RightBalance(T); *shorter = TRUE; break;
                        }//switch (rc->bf)
                }//switch ((*T)->bf)
        }//else if
        else {                                      //应继续在*T的右子树中进行搜索
            if (!DeleteAVL(&((*T)->rchild),e,shorter)) return FALSE;     //未删除
            if (*shorter)                           //已在*T的右子树中删除且右子树“变矮”
                switch ((*T)->bf) {                 //检查*T的平衡度
                    case LH:                        //原本左子树比右子树高，需要作平衡处理
                        lc = (*T)->lchild;
                        switch (lc->bf){            //检查*T左子树平衡度
                            case EH:                //对*T作单右旋处理，树高度不变
                                (*T)->bf = LH; lc->bf = RH; R_Rotate(T);
                                *shorter = FALSE; break;
                                break;
                            default:                //对*T作左平衡处理，树变矮
                                LeftBalance(T); *shorter = TRUE; break;
                        }//switch (lc->bf)
                    case EH:                        //原本、左右子树等高，现右子树变矮，树高度不变
                        (*T)->bf = LH; *shorter = FALSE; break;
                    case RH:                        //原本右子树比左子树高，现左、右子树等高，树变矮
                        (*T)->bf = EH; *shorter = TRUE; break;
                }//switch ((*T)->bf)
        }//else
    }//else
    return TRUE;
}//DeleteAVL

void Delete(BBSTree *p,bool *shorter){
    //从平衡二叉树中删除结点*p，并重接它的左或右子树
    BBSTree q;
    STElemType tmp;
    if (!((*p)->rchild)) {          //右子树空则只需重接它的左子树
        q = *p; *p = (*p)->lchild; free(q); *shorter = TRUE;
    }//if
    else if (!((*p)->lchild)) {     //左子树空则只需重接它的右子树
        q = *p; *p = (*p)->rchild; free(q); *shorter = TRUE;
    }//else if
    else {      //左右子树均不空
        q = (*p)->lchild;
        while (q->rchild) q = q->rchild;    //转左，然后向右到尽头
        tmp = q->data;                      //q指向被删结点的“前驱”
        *shorter = FALSE; DeleteAVL(p,tmp,shorter); //AVL树中删除q结点（q结点左右子树至少一个为空）★
        (*p)->data = tmp;                   //q结点的值赋给*p结点
    }//else
}//Delete

Status InorderTraverseAVL(BBSTree T,Status Visit(STElemType)){
    //中序遍历平衡二叉树T，得到一个关键字的有序序列
    if (T){
        if (InorderTraverseAVL(T->lchild,Visit))
            if (Visit(T->data))
                if (InorderTraverseAVL(T->rchild,Visit))
                    return OK;
        return ERROR;
    }//if
    else return OK;
}//InorderTraverseAVL

void CreateAVL(BBSTree *T){
    //输入关键字序列，建立一颗平衡二叉树T
    int i, n;
    bool taller;
    STElemType e;
    InitAVL(T);
    printf("Enter number of elements:"); scanf("%d%*c",&n);
    printf("Enter all elements' keys seperated by delimiter:");
    for (i = 1; i <= n; ++i){
        taller = FALSE;
        scanf("%d%*c",&(e.key));
        InsertAVL(T,e,&taller);
    }//for
    printf("Succeeded!\n");
}//CreateAVL

#endif // !AVLTREE_H
