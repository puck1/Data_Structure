
#include <stdio.h>
#include <stdlib.h>
#include "status.h"	
	
	//- - - - - 线性表的单链表存储结构 - - - - - 
#define ElemType 	int	   //线性表存放的数据类型 
typedef struct LNode{
	ElemType 	  data;
	struct LNode *next;
}LNode, *LinkList; 
	//- - - - - - - - - - - - - - - - - - - - - 

Status GetElem_L(LinkList L,int i,ElemType *e){
	//L为带头节点的单链表的头指针。
	//当第i个元素存在时，其值赋给e并返回OK，否则返回ERROR
	LinkList p = L->next; int j = 1;	//初始化，p指向第一个结点，j为计数器★
	while(p && j<i){					//顺指针向后查找，直到p指向第i个元素或p为空 
		p = p->next; ++j; 
	}
	if(!p||j>i) return ERROR;			//第i个元素不存在 
	*e = p->data;						//取第i个元素 
	return OK; 
}//GetElem_L

Status ListInsert_L(LinkList *L,int i,ElemType e){
	//在带头节点的单链线性表L中第i个位置之前插入元素e
	LinkList p = *L; int j = 0;						//★(有可能在第一个位置前插入)
	while(p && j<i-1){
		p = p->next; ++j;							//寻找第i-1个结点 
	}
	if(!p||j>i-1) return ERROR;						//i小于1或者大于表长加1★
	LinkList s = (LinkList)malloc(sizeof(LNode));	//生成新结点 
	s->data = e; s->next = p->next;					//插入L中 
	p->next = s; 
	return OK;
} //ListInsert_L

Status ListDelete_L(LinkList *L,int i,ElemType *e){
	//在带头结点的单链线性表L中，删除第i个元素，并由e返回其值
	LinkList p = *L; int j = 0;
	while(p->next && j<i-1){	//寻找第i个结点，并令p指向其前趋 
		p = p->next; ++j;
	}
	if(!p->next||j>i-1) return ERROR;			//删除位置不合理 
	LinkList s = p->next; p->next = s->next;	//删除并释放结点 
	*e = s->data; free(s);
	return OK;
}//ListDelete_L 
 
void CreateList_L(LinkList *L,int n){	//(应传入双重指针，以在局部函数中修改指针值)★	
	//逆位序输入n个元素的值，建立带表头节点的单链线性表L。
	LinkList p; int i;	
	*L = (LinkList)malloc(sizeof(LNode));
	(*L)->next = NULL;		//先建立一个带头结点的单链表 
	for(i=n;i>0;--i){
		p = (LinkList)malloc(sizeof(LNode));	//生成新结点 
		printf("请输入第%d个元素的值:",i);
	 	scanf("%d",&p->data);					//输入元素值 
	 	p->next = (*L)->next; (*L)->next = p; 	//插入到表头 
	 }
	 printf("单链表建立完成!\n\n");
}//CreateList_L

void MergeList_L(LinkList *La,LinkList *Lb,LinkList *Lc){
	//已知单链线性表La和Lb的元素按值非递减排列。
	//归并La和Lb得到新的单链线性表Lc，Lc的元素也按值非递减排列。 
	LinkList pa = (*La)->next; LinkList pb = (*Lb)->next;
	LinkList pc = *Lc = *La;				//用La的头结点作为Lc的头结点 
	while(pa&&pb){
		if(pa->data<pb->data){
			pc->next = pa; pc = pa; pa = pa->next;
		}
		else{pc->next = pb; pc = pb; pb = pb->next;}
	}
	pc->next = pa?pa:pb;					//插入剩余段★ 
	free(*Lb);								//释放lb的头结点 
} //MergeList_L

int main(){
	//Operation;
	LinkList La,Lb,Lc;
	ElemType e,e1,e2,e3,e4,e5,e6;
	printf("请输入要建立的单链表La长："); 
	int n1,n2;  scanf("%d",&n1);
	CreateList_L(&La,n1);
	GetElem_L(La,1,&e1); GetElem_L(La,2,&e2); GetElem_L(La,3,&e3);
	printf("单链表La前三项数据分别为：%d %d %d\n",e1,e2,e3);
	ListInsert_L(&La,3,e2); 
	GetElem_L(La,1,&e1); GetElem_L(La,2,&e2); GetElem_L(La,3,&e3);
	printf("插入(复制第二项)后单链表La前三项数据分别为：%d %d %d\n",e1,e2,e3);
	ListDelete_L(&La,3,&e); printf("删除的La第三项数据为：%d,",e);
	GetElem_L(La,1,&e1); GetElem_L(La,2,&e2); GetElem_L(La,3,&e3);
 	printf("此后La前三项数据为：%d %d %d\n\n",e1,e2,e3);
 	printf("请输入要与La合并的单链表长："
 		"（Tip:输入的La与Lb应为非递减排序)"); scanf("%d",&n2);
	CreateList_L(&Lb,n2);
 	MergeList_L(&La,&Lb,&Lc);
 	GetElem_L(Lc,1,&e1); GetElem_L(Lc,2,&e2); GetElem_L(Lc,3,&e3);
 	GetElem_L(Lc,4,&e4); GetElem_L(Lc,5,&e5); GetElem_L(Lc,6,&e6);
 	printf("合并后所得Lc的前六项数据为：%d %d %d %d %d %d\n",e1,e2,e3,e4,e5,e6);
	return 0;
} //单链表的建立/取值/增删/合并 
