
#include <stdio.h>
#include "status.h"
#include "SqBinaryTree.h"

Status PrintElement(TElemType e){
	//输出元素e的值
	printf("%c ",e);
	return OK;
}//PrintElement

int main(){
	//Opreation;
	//测试用例：表达式a+b*(c-d)-e/f，先序建立序列为:-+a  *b  -c  d  /e  f  EOF
	Position s = {2,1};		//第二层第一个结点
	Position p = {3,1};		//第三层第一个结点
	Position q = {3,2};		//第三层第二个结点
	SqBiTree T;
	InitBiTree(T);
	printf("Create SqBiTree T in preface order,character SPACE (' ') means node is empty:\n");
	CreateBiTree(T,0);

	printf("PreOrderTraverse_Recur:");	//先序遍历递归算法测试
	PreOrderTraverse_Recur(T,PrintElement,0); printf("\n");

	printf("InOrderTraverse_Recur:");	//中序遍历递归算法测试
	InOrderTraverse_Recur(T,PrintElement,0); printf("\n");

	printf("PostOrderTraverse_Recur:");	//后序遍历递归算法测试
	PostOrderTraverse_Recur(T,PrintElement,0); printf("\n");

	printf("LevelOrderTraverse:");		//层序遍历算法测试
	LevelOrderTraverse(T,PrintElement); printf("\n");

	printf("The depth of tree is %d .\n",BiTreeDepth(T,0));
	printf("Data_root is '%c' , data_p = '%c' , data_q = '%c'.\n",Root(T),Value(T,p),Value(T,q));
	Assign(T,p,'x');	printf("Let Data_p = 'x', data_p = '%c'.\n",Value(T,p));
	printf("p.parent is '%c' , q.parent is '%c'.\n",Parent(T,p),Parent(T,q));
	printf("s.leftchild is '%c' , s.rightchild is '%c'.\n",LeftChild(T,s),RightChild(T,s));
	printf("q.leftsibling is '%c' , p.rightsibling is '%c'.\n",LeftSibling(T,q),RightSibling(T,p));
	ClearBiTree(T);
	BiTreeEmpty(T)?printf("SqBiTree T is cleared!\n"):printf("SqBiTree T is not cleared!\n");
	return 0;
}//顺序结构二叉树的建立/遍历/替换/查询/遍历
