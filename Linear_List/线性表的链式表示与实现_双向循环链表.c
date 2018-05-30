
#include <stdio.h>
#include <stdlib.h>
#include "status.h" 
	
//- - - - - 线性表的双向循环链表存储结构 - - - - - 
#define ElemType 	int	   //线性表存放的数据类型
typedef struct DuLNode{
	ElemType data;
	struct DuLNode *prior;
	struct DuLNode *next;
}DuLNode, *DuLinkList; 
//- - - - - - - - - - - - - - - - - - - - - -  

//双向链表ListLength、GetElem和LocateELem等仅需涉及一个方向指针，
//它们的算法描述和线性链表的操作相同 

void CreateList_DuL(DuLinkList *L,int n){	
	//逆位序输入n个元素的值，建立带表头节点的双向循环链表L。
	DuLinkList p; int i;	
	*L = (DuLinkList)malloc(sizeof(DuLNode));
	(*L)->next = *L;
	(*L)->prior = *L; 
	for(i=n;i>0;--i){
		p = (DuLinkList)malloc(sizeof(DuLNode));	//生成新结点 
		printf("请输入第%d个元素的值:",i);
	 	scanf("%d",&p->data);					//输入元素值 
	 	p->next = (*L)->next; (*L)->next = p; 	//插入到表头 
	 	p->next->prior = p; p->prior = *L;
	 }
	 printf("双向循环链表建立完成!\n\n");
}//CreateList_DUL

Status ListTraverse_DuL(DuLinkList L){
	//按顺序遍历双向循环链表，并输出各项的值
	 DuLinkList k = L->next;
	 int count = 1;
	 if (!k) return ERROR;
	 while(k!=L){
	 	printf("双向循环链表的第%d项为%d;\n",count,k->data);
	 	k = k->next;
	 	count++;
	 }
	 return OK;
}//ListTraverse_DuL

Status ListInsert_DuL(DuLinkList *L,int i,ElemType e){
	//在带头结点的双链循环线性表L中第i个位置之前插入元素e，
	//i的合法值为1<=i<=表长+1。 
	DuLinkList k = (*L)->next; int count = 1;
	while(k!=(*L)&&count<i){				//在L中确定插入位置 
		k = k->next; ++count;
	}
	if(count!=i) return ERROR;				//插入位置不合法 
	DuLinkList s;
	if(!(s = (DuLinkList)malloc(sizeof(DuLNode)))) return ERROR;
	s->data = e;
	s->prior = k->prior; k->prior->next = s;
	s->next = k; 		 k->prior = s;
	return OK;
}//ListInsrt_DuL
 
Status ListDelete_DuL(DuLinkList *L,int i,ElemType *e){
	//删除带头结点的双链循环线性表L的第i个元素，i的合法值为1<=i<=表长
	//用e返回其值
	DuLinkList k = (*L)->next; int count = 1;
	while(k!=(*L)&&count<i){			//在L中确定第i个元素的位置指针k 
		k = k->next; ++count;
	}
	if(k==*L||count>i) return ERROR;		//第i个元素不存在 
	*e = k->data;
	k->prior->next = k->next;
	k->next->prior = k->prior;
	free(k); return OK;
}//ListDelete_DuL 
 
int main(){
	//Operation;
	DuLinkList L;
	printf("请输入要建立的双向循环链表长度：");
	int n; scanf("%d",&n);
	CreateList_DuL(&L,n);
	ListTraverse_DuL(L);
	ListInsert_DuL(&L,1,3*L->next->data);
	printf("\nInserting...\n"); 	
	ListTraverse_DuL(L);
	ElemType e;
	ListDelete_DuL(&L,1,&e);
	printf("\nDeleting...\n"); 	
	ListTraverse_DuL(L);	
	return 0;
}//双向循环链表的建立/遍历/插入/删除 

