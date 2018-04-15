
#include <stdio.h>
#include "ChildTree.h"

Status PrintElement(TElemType e){
    //打印元素e的值
    printf("%c ",e);
    return OK;
}//PrintElement

int main(){
    //Operation;
    CTree T,c;
    int i,j;
    TElemType e;
    InitTree(&T);
    InitTree(&c);
    printf("Create tree T in level order(charater space means node is empty):\n");
    CreateTree(&T);                         //层序建立树T

    printf("LevelOrderTraverse_T:");
    LevelOrderTraverse(T,PrintElement);     //层序遍历

    printf("The depth of tree T is %d ,data of root is '%c'.\n",TreeDepth(T),Root(T));
    printf("Enter the order (start from 0 in level order)\n"
            "of the node you want to look up and the value want to Assign:");
    scanf("%d %c",&i,&e);
    getchar();      //吞掉回车
    printf("The value of the no.%d node is '%c',value of its parent is '%c',"
            "\nleft child is '%c',right sibling is '%c'.\n",
            i,Value(T,i),Parent(T,i),LeftChild(T,i),RightSibling(T,i));     //查询，赋值

    printf("Create tree c in level order(charater space means node is empty):\n");
    CreateTree(&c);                 //层序建立树c
    printf("LevelOrderTraverse_c:");
    LevelOrderTraverse(c,PrintElement);
    printf("Insert tree c as 2nd child of tree T...\n");
    InsertChild(&T,0,2,c);          //插入子树
    printf("LevelOrderTraverse_T:");
    LevelOrderTraverse(T,PrintElement);

    printf("Enter the order (start from 0 in level order) of the node \n"
            "whose child will be deleted, and the order of its child(from 1 on):");
    scanf("%d %d",&i,&j);
    DeleteChild(&T,i,j);            //删除子树
    printf("LevelOrderTraverse_T:");
    LevelOrderTraverse(T,PrintElement);

    printf("Insert '@' as 2nd child of T's root...\n");
    InsertNode(&T,0,2,'@');         //插入结点
    printf("LevelOrderTraverse_T:");
    LevelOrderTraverse(T,PrintElement);

    int clear = 1;
    ClearTree(&T);      //清空
    ClearTree(&c);
    for(i = 0;i < MAX_TREE_SIZE;++i){
        if(T.nodes[i].data != ' ' || c.nodes[i].data != ' '){
            clear = 0;
            break;
        }
    }
    clear? printf("Tree T and c is cleared!\n"):printf("Tree T or c is not cleared!\n");
    return 0;
}//孩子表示法（带双亲）树的建立/遍历/查询/插入/删除/清空