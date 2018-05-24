
#include <stdio.h>
#include "status.h"
#include "TriLinkBinaryTree.h"

Status PrintElement(TElemType e){
	//输出元素e的值
	printf("%c ",e);
	return OK;
}//PrintElement

int main(){
    //Operation;
    TBiTree T;
    InitBiTree(&T);
    printf("Create TBiTree T in preface order,character SPACE (' ') means node is empty:\n");
    CreateBiTree(&T);
    TBiNode *s = T->lchild;
    TBiNode *p = s->lchild;
    TBiNode *q = s->rchild;
    printf("PreOrderTraverse_Recur:");      //先序遍历递归算法测试
	PreOrderTraverse_Recur(T,PrintElement); printf("\n");

    printf("InOrderTraverse_Recur:");       //中序遍历递归算法测试
	InOrderTraverse_Recur(T,PrintElement); printf("\n");

    printf("PostOrderTraverse_Recur:");     //后序遍历递归算法测试
	PostOrderTraverse_Recur(T,PrintElement); printf("\n");

	printf("LevelOrderTraverse:");          //层序遍历算法测试
	LevelOrderTraverse(T,PrintElement); printf("\n");

	printf("The depth of tree is %d .\n",BiTreeDepth(T));
	printf("Data_root is '%c' , data_p = '%c' , data_q = '%c'.\n",Root(T),Value(p),Value(q));
	Assign(p,'x');	printf("Let Data_p = 'x', data_p = '%c'.\n",Value(p));
	printf("p.parent is '%c' , q.parent is '%c'.\n",Parent(p),Parent(q));
	printf("s.leftchild is '%c' , s.rightchild is '%c'.\n",LeftChild(s),RightChild(s));
	printf("q.leftsibling is '%c' , p.rightsibling is '%c'.\n",LeftSibling(T,q),RightSibling(T,p));
	ClearBiTree(&T);
	BiTreeEmpty(T)?printf("TBiTree T is cleared!\n"):printf("TBiTree T is not cleared!\n");
    return 0;
}//三叉链表结构二叉树的建立/替换/查询/递归遍历/非递归遍历
