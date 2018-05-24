
#include <stdio.h>
#include "status.h"
#include "BiLinkBinaryTree.h"

Status PrintElement(TElemType e){
	//输出元素e的值
	printf("%c ",e);
	return OK;
}//PrintElement

int main(){
    //Operation;
    //测试用例：表达式a+b*(c-d)-e/f，先序建立序列为:-+a  *b  -c  d  /e  f  EOF
    BiTree T;
    InitBiTree(&T);
    printf("Create BiTree T in preface order,character SPACE (' ') means node is empty:\n");
    CreateBiTree(&T);
    BiTNode *s = T->lchild;
    BiTNode *p = s->lchild;
    BiTNode *q = s->rchild;
    printf("PreOrderTraverse_Recur:");      //先序遍历递归算法测试
	PreOrderTraverse_Recur(T,PrintElement); printf("\n");

    printf("PreOrderTraverse_NRecur:");     //先序遍历非递归算法测试
    PreOrderTraverse_NRecur(T, PrintElement); printf("\n");

    printf("InOrderTraverse_Recur:");       //中序遍历递归算法测试
	InOrderTraverse_Recur(T,PrintElement); printf("\n");

    printf("InOrderTraverse_NRecur0:");      //中序遍历非递归算法测试0
    InOrderTraverse_NRecur_0(T, PrintElement); printf("\n");

    printf("InOrderTraverse_NRecur1:");      //中序遍历非递归算法测试1
    InOrderTraverse_NRecur_1(T, PrintElement); printf("\n");

    printf("InOrderTraverse_NRecur2:");      //中序遍历非递归算法测试2
    InOrderTraverse_NRecur_2(T, PrintElement); printf("\n");

    printf("PostOrderTraverse_Recur:");     //后序遍历递归算法测试
	PostOrderTraverse_Recur(T,PrintElement); printf("\n");

    printf("PostOrderTraverse_NRecur:");     //后序遍历非递归算法测试
	PostOrderTraverse_NRecur(T,PrintElement); printf("\n");

	printf("LevelOrderTraverse:");          //层序遍历算法测试
	LevelOrderTraverse(T,PrintElement); printf("\n");

	printf("The depth of tree is %d .\n",BiTreeDepth(T));
	printf("Data_root is '%c' , data_p = '%c' , data_q = '%c'.\n",Root(T),Value(p),Value(q));
	Assign(p,'x');	printf("Let Data_p = 'x', data_p = '%c'.\n",Value(p));
	printf("p.parent is '%c' , q.parent is '%c'.\n",Parent(T,p),Parent(T,q));
	printf("s.leftchild is '%c' , s.rightchild is '%c'.\n",LeftChild(s),RightChild(s));
	printf("q.leftsibling is '%c' , p.rightsibling is '%c'.\n",LeftSibling(T,q),RightSibling(T,p));
	ClearBiTree(&T);
	BiTreeEmpty(T)?printf("SqBiTree T is cleared!\n"):printf("SqBiTree T is not cleared!\n");
    return 0;
}//二叉链表结构二叉树的建立/替换/查询/递归遍历/非递归遍历
