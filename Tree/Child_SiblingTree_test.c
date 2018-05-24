
#include <stdio.h>
#include <synchapi.h>   //gcc中提供Sleep函数（以毫秒为单位）原型
#include "status.h"
#include "Child_SiblingTree.h"

Status PrintElement(TElemType e){
    //输出元素e的值
    printf("%c ",e);
	return OK;
}//PrintElement

int main(){
    //Operation; For test:RAD E  B CFG H K     EOF
    CSTree T,c;
    InitTree(&T); InitTree(&c); //初始化

    printf("Create tree T in preface order,character SPACE (' ') means node is empty:\n");
    CreateTree(&T);     //建立树T
    getchar();          //吞掉回车符★
    CSNode *p = T->firstchild->firstchild->nextsibling; //p指示树T中第一棵子树的第二个孩子

    printf("The depth of tree T is %d,data of root is '%c'.\n",TreeDepth(T),Root(T));
    printf("PreOrderTraverse_Recur_T:"); PreOrderTraverse_Recur(T,PrintElement);    //先序遍历
    printf("\n");

    printf("Data of 2nd child of root's 1st child is '%c',its parent is '%c',left child is '%c',"
    "right sibiling is '%c'.Let it be '@'.",Value(T,p),Parent(T,p),LeftChild(p),RightSibling(p));
    Assign(p,'@');      //查询与更改

    printf("Create tree T in preface order,character SPACE (' ') means node is empty:\n");
    CreateTree(&c);

    printf("Inserting c as 2nd child of T...");
    InsertChild(T,2,c); //插入子树
    Sleep(500); printf("Succeeded!\n");
    printf("PostOrderTraverse_Recur_T:"); PostOrderTraverse_Recur(T,PrintElement);  //后序遍历
    printf("\n");

    printf("Deleting 1st child of T...");
    DeleteChild(T,1);   //删除子树
    Sleep(500); printf("Succeeded!\n");
    printf("LevelOrderTraverse_T:"); LevelOrderTraverse(T,PrintElement);            //层序遍历
    printf("\n");

    ClearTree(&T);      //清空
    TreeEmpty(T)?printf("T is cleared!\n"):printf("T isnot cleared!\n");
}//孩子-兄弟表示法树的建立/查询/更改/插入/删除/遍历
