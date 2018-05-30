/**
 * This file is to test ExtendedLinkList.
 * This file includes creating,traverse,merge and some other basic operateions.
 */

typedef int ElemType;	//测试所用线性表数据类型暂定为int

#include <stdio.h>
#include "status.h"
#include "ExtendedLinkList.h"

Status visit(Link p){
	printf("%d ",p->data);
	return OK;
}//visit

int cmp(ElemType a,ElemType b){
	return a-b;
}//cmp

int main(){
	LinkList La,Lb,Lc;
	CreateList(&La);
	ListTraverse(La,&visit);
	printf("\n");
	CreateList(&Lb);
	MergeList_L(&La,&Lb,&Lc,&cmp);
	printf("\n归并La与Lb所得Lc");
	ListTraverse(Lc,&visit);
	return 0;
}//扩展的线性表的建立/遍历/归并
