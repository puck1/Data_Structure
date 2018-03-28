/**
 * This file defines an extended list.
 * This extended list add an pointer to the tail of list and saves length of list.
 */ 
 
#ifndef EXTENDEDLINKLIST_H
#define EXTENDEDLINKLIST_H 
 
#include <stdio.h> 
#include <stdlib.h>
#include "status.h"

//- - - - - 扩展的线性单链表 - - - - -  
typedef struct LNode {	//结点类型 
	ElemType 	  	data;
	struct LNode 		*next;
}*Link, *Position; 

typedef struct {		//链表类型		
	Link head,tail;		//分别指向线性链表中的头结点和最后一个结点 
	int len;		//指示线性链表中数据元素的个数 
}LinkList;
//- - - - - - - - - - - - - - - - - - - - - 

Status MakeNode(Link *p,ElemType e){
	// 分配由p指向的值为e的结点，并返回OK；若分配失败，则返回ERROR
	if(!(*p = (Link)malloc(sizeof(struct LNode))))
		exit(OVERFLOW);
	(*p)->data = e; (*p)->next = NULL; 
	return OK;
}	// MakeNode

void FreeNode(Link *p){
	// 释放p所指结点
	free(*p);		//释放空间 
	(*p) = NULL;		//*p指针设为NULL，防止野指针出现★ 
}	// FreeNode

Status InitList(LinkList *L){
	// 构造一个空的线性链表L
	Link p;
	if(!(p = (Link)malloc(sizeof(struct LNode))))
		exit(OVERFLOW);
	p->next = NULL;
	L->head = L->tail = p; L->len = 0;
	return OK;
}	// InitList

Status DestroyList(LinkList *L){
	// 销毁线性链表L，L不再存在
	Link p = L->head; Link q;
	while(p){
		q = p->next;
		free(p); p = q;
	}
	L->head = L->tail = NULL; L->len = 0;
	L = NULL;
	return OK;
}	// 	DestroyList

Status ClearList(LinkList *L){
	// 将线性链表L重置为空表，并释放原链表的结点空间
	Link p = L->head->next; Link q;
	while(p){
		q = p->next;
		free(p); p = q;
	}
	L->head->next = NULL;
	L->tail = L->head;
	L->len = 0;
	return OK;
}	// ClearList

Status InsFirst(Link h,Link s){
	// 已知h指向线性链表的头结点，将s所指结点插入在第一个结点之前
	//（此函数未考虑链表的长度以及尾结点的变化，需自行更改） 
	s->next = h->next;
	h->next = s;
	return OK;
}	// InsFirst

Status DelFirst(Link h,Link *q){
	// 已知h指向线性链表的头结点，删除链表中的第一个结点并以q返回
	//（此函数未考虑链表的长度以及尾结点的变化，需自行更改）
	*q = h->next;
	if(!(*q)) return ERROR;
	h->next = (*q)->next;
	(*q)->next = NULL;		//返回结点后继设为NULL 
	return OK;
}

Status Append(LinkList *L,Link s){
	// 将指针s所指(彼此以指针相链)的一串结点链接在线性链表L的最后一个结点
	// 之后，并改变链表L的尾指针指向新的尾结点
	Link p = L->tail;
	p->next = s;			//插入s所指结点 
	int count = 0;
	while(p->next){
		p = p->next;		//找到新的尾结点 
		++count;
	}
	L->tail = p;			//更改尾指针 
	L->len += count;		//更改链表长度 
	return OK; 
}	// Append

Status Remove(LinkList *L,Link *q){
	// 删除线性链表L中的尾结点并以q返回，改变链表L的尾指针指向新的尾结点
	if(!L->len){
		*q = NULL;		//L中仅有头结点，返回NULL 
		return ERROR;
	} 
	*q = L->tail;
	Link p = L->head;
	while(p->next!=*q) p = p->next;	//寻找尾结点的前驱 
	p->next = NULL;
	L->tail = p; --L->len;		//更改尾结点及链表长
	return OK;
}	// Remove

Status InsBefore(LinkList *L,Link *p,Link s){
	// 已知p指向线性链表L中的一个结点，将s所指节点插入在p所指节点之前，
	// 并修改指针p指向新插入的结点
	Link q = L->head;
	while(q->next&&q->next!=*p)
		q = q->next;
	if(!q->next) return ERROR;	//p所指不在链表中 
	q->next = s; s->next = *p;	//插入节点 
	*p = s;				//指针p指向新插入结点 
	++L->len;			//修改链表长 
	return OK;
}	// InsBefore

Status InsAfter(LinkList *L,Link *p,Link s){
	// 已知p指向线性链表L中的一个结点，将s所指节点插入在p所指节点之后，
	// 并修改指针p指向新插入的结点
	Link q = L->head;
	while(q&&q!=*p) q = q->next;
	if(!q) return ERROR;			//p所指不在链表中
	s->next = (*p)->next; (*p)->next = s;	//插入结点 
	if(*p == L->tail) L->tail = s;		//若插入到尾结点之后，更改尾指针 
	++L->len;				//修改链表长 
	*p = s;					//修改p指针指向新插入结点 
	return OK;
}	// InsAfter

Status SetCurElem(Link p,ElemType e){
	// 已知p指向线性链表中的一个结点，用e更新p所指结点中数据元素的值
	p->data = e;
	return OK;
}	// SetCurELem

ElemType GetCurElem(Link p){
	// 已知p指向线性链表中的一个结点，返回p所指结点中数据元素的值
	return p->data;	
} 	// GetCurElem

Status ListEmpty(LinkList L){
	// 若线性链表L为空表，则返回TRUE，否则返回FALSE
	if(L.head->next) return FALSE;
	return TRUE; 
}	// ListEmpty

int ListLength(LinkList L){
	// 返回线性链表L中元素个数
	return L.len; 
}	// ListLength

Position GetHead(LinkList L){
	// 返回线性链表L中头结点的位置
	return L.head;
}	// GetHead

Position GetLast(LinkList L){
	// 返回线性链表L中最后一个结点的位置
	return L.tail;
}	// GetLast

Position PriorPos(LinkList L,Link p){
	// 已知p指向线性链表L中的一个结点，返回p所指结点的直接前驱的位置，
	// 若无前驱，则返回NULL
	Link q = L.head;
	while(q->next!=p) q = q->next;		//寻找p的前驱 
	if(q == L.head) return NULL;		//首结点无前驱 
	return q;
}	// PriorPos

Position NextPos(LinkList L,Link p){
	// 已知p指向线性链表L中的一个结点，返回p所指结点的直接后继的位置，
	// 若无后继，则返回NULL
	return p->next;
}	// NextPos

Status LocatePos(LinkList L,int i,Link *p){
	// 返回p指示线性链表L中第i个结点的位置并返回OK，i值不合法时返回ERROR
	Link q = L.head; int count = 0;
	while(q&&count<i){
		q = q->next; ++count;
	}
	if(!q||count>i) return ERROR;		//i值不合法 
	*p = q;
	return OK;
}	// LocatePos

Position LocateElem(LinkList L,ElemType e,Status(*compare)(ElemType,ElemType)){
	// 返回线性链表L中第1个与e满足函数compare()判定关系的元素的位置，
	// 若不存在这样的元素，则返回NULL
	Link p = L.head->next;
	while(p&&!(*compare)(p->data,e))
	p = p->next;
	return p;
}	// LocateElem

Status ListTraverse(LinkList L,Status(*visit)(Link)){
	// 依次对L的每个元素调用函数Visit()。一旦Visit()失败，则操作失败。
	Link p = L.head->next;
	printf("遍历Visit结果为：");
	while(p){
		if(!(*visit)(p))
		return ERROR;
		p = p->next;
	}
	printf("\n");
	return OK;
}	// ListTraverse

Status CreateList(LinkList *L){
	// 依次输入元素数据，从链表尾部开始创建一个新的带头结点的线性链表L
	InitList(L);
	int num,count;
	Link p = L->head;
	Link s; ElemType e;
	printf("请输入要建立链表的长度："); scanf("%d",&num);
	for(count=1;count<=num;count++){
		printf("请输入第%d个元素数据：",count);
		scanf("%d",&e);
		if(!(MakeNode(&s,e))) return ERROR;
		InsAfter(L,&p,s);
	}
	printf("单链表建立完成！\n");
	return OK;
}

Status ListInsert_L(LinkList *L,int i,ElemType e){
	// 在带头结点的单链线性表L中的第i个元素之前插入元素e
	Link p,q;
	if(!(LocatePos(*L,i-1,&p))) return ERROR;	//i值不合法 
	if(!MakeNode(&q,e)) return ERROR;		//结点存储分配失败 
	InsFirst(p,q);	//对于第i个结点开始的链表，第i-1个结点是它的头结点 
	++L->len;	//L表长+1 
	return OK;
}	// ListInsert_L 

Status MergeList_L(LinkList *La,LinkList *Lb,LinkList *Lc,
									int (*compare)(ElemType,ElemType)){
	// 已知单链线性表La和Lb中的元素按值非递减排列。
	// 归并La和Lb得到新的单链线性表Lc，Lc的元素也按值非递减排列。 
	if(!InitList(Lc)) return ERROR;		//存储空间分配失败 
	Link ha = GetHead(*La); Link hb = GetHead(*Lb);	//ha和hb分别指向La和Lb的头结点 
	Link pa = NextPos(*La,ha); Link pb = NextPos(*Lb,hb);	//pa和pb分别指向La和Lb中当前结点 
	ElemType a,b; Link q;
	while(pa&&pb){		//La和Lb均非空 
		a = GetCurElem(pa); b = GetCurElem(pb);		//a和b为两表中当前比较元素 
		if((*compare)(a,b)<=0){	// a<=b
			DelFirst(ha,&q); Append(Lc,q); pa = NextPos(*La,ha); }
		else{	// a>b
			DelFirst(hb,&q); Append(Lc,q); pb = NextPos(*Lb,hb); }
	}//while
	if(pa) Append(Lc,pa);			//链接La中剩余结点 
	else Append(Lc,pb);			//链接Lb中剩余结点 
	FreeNode(&ha); FreeNode(&hb);		//释放La和Lb的头结点 
	return OK;
}	// MergeList_L

#endif
