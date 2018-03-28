
#include <stdio.h>
#include <stdlib.h>
#include "status.h"	
	
//- - - - - 线性表的动态分配顺序存储结构 - - - - -
#define LIST_INIT_SIZE	100  //线性表存储空间的初始分配量  
#define LISTINCREMENT	10  //线性表存储空间的分配增量 
#define ElemType 	int	   //线性表存放的数据类型 
typedef struct{
	ElemType *elem;		//存储空间基址 
	int 	length;	 	//当前长度
	int  	listsize;	//当前分配的存储容量（以sizeof（ElemType）为单位） 
}SqList;
//- - - - - - - - - - - - - - - - - - - - - - - -

Status InitList_Sq(SqList *L){
	//构造一个空的线性表L
	L->elem = (ElemType *)malloc(LIST_INIT_SIZE*sizeof(ElemType));
	if(!L->elem) exit(OVERFLOW);
	L->length = 0;
	L->listsize = LIST_INIT_SIZE;
	return OK;
}//InitList_Sq

Status List_Insert_Sq(SqList *L,int i,ElemType e){
	//在顺序线性表L中第i个位置之前插入新的元素e;
	//i的合法值为1<=i<=ListLength_Sq(L)+1 
	if(i<1||i>L->length+1) return ERROR;	//i值不合法
	if(L->length>=L->listsize){		//当前存储空间已满，增加分配 
		ElemType *newbase;
		newbase = (ElemType *)realloc(L->elem,
						(L->listsize+LISTINCREMENT)*sizeof(ElemType));
	if(!newbase) exit(OVERFLOW);	//存储分配失败 
	L->elem = newbase;				//新基址 
	L->listsize += LISTINCREMENT;	//增加存储容量 
	}
	ElemType *q;
	q = &L->elem[i-1];				//q为插入位置 
	ElemType *p;
	for(p=&(L->elem[L->length-1]);p>=q;--p) *(p+1) = *p;					
									//插入位置及之后的元素右移★ 
	*q = e;			//插入e 
	++L->length;	//表长增1 
	return OK;
}//ListInsert_Sq

Status ListDelete_Sq(SqList *L,int i,ElemType *e){
	//在顺序线性表L中删除第i个元素，并用e返回其值
	//i的合法值为1<=i<=ListLength_Sq(L)
	if((i<1)||(i>L->length)) return ERROR;	//i值不合法
	ElemType *p;							 
	p = &(L->elem[i-1]);					//p为被删除元素的位置 
	*e = *p;								//被删除元素的值赋给e 
	ElemType *q=L->elem+L->length-1;		//表尾元素的位置★
	for(++p;p<=q;++p) *(p-1) = *p;			//被删除元素之后的元素左移★ 
	--L->length;							//表长减1 
	return OK;	 
}//ListDelete_Sq 

int LocateElem_Sq(SqList L,ElemType e,
					  Status(*compare)(ElemType,ElemType)){
	//在顺序线性表L中查找第1个值与e满足compare（）的元素的位序
	//若找到，则返回其在L中的位序，否则返回0
	int i = 1;					//i的初值为第1个元素的位序 
	ElemType *p = L.elem;		//p的初值为第1个元素的存储位置 
	while(i<=L.length&&!(*compare)(*p++,e)) ++i;	//★
	if(i<=L.length) return i;
	else return 0;				  	
}//LocateElem_Sq 

void MergeList_Sq(SqList La,SqList Lb,SqList *Lc){
	//已知顺序线性表La和Lb的元素按值非递减排列
	//归并La和Lb得到新的顺序线性表Lc，Lc的元素也按值非递减排列
	 ElemType *pa = La.elem; ElemType *pb = Lb.elem;
	 Lc->length = Lc->listsize = La.length + Lb.length;
	 ElemType *pc;
	 pc = Lc->elem = (ElemType *)malloc(Lc->listsize*sizeof(ElemType));
	 if(!Lc->elem) exit(OVERFLOW);	//存储分配失败 
	 ElemType *pa_last = La.elem + La.length - 1;
	 ElemType *pb_last = Lb.elem + Lb.length - 1;	 
	 while(pa<=pa_last&&pb<=pb_last){		//归并★ 
	 	if(*pa < *pb) *pc++ = *pa++;
	 	else *pc++ = *pb++;
	 }
	 while(pa<=pa_last) *pc++ = *pa++;		//插入La的剩余元素★
	 while(pb<=pb_last) *pc++ = *pb++;	 	//插入Lb的剩余元素★
}	//MegerList_Sq 

int main()
{
	SqList La,Lb,Lc; 
	InitList_Sq(&La);
	InitList_Sq(&Lb);
	InitList_Sq(&Lc);
	La.length = 5;
	Lb.length = 7;
	int i;
	printf("请依次输入表a的五个值，两个值之间用空格隔开：\n"); 	
	for(i=0;i<La.length;i++) scanf("%d",&La.elem[i]);
	printf("\n请依次输入表b的七个值,两个值之间用空格隔开：\n"); 
	for(i=0;i<Lb.length;i++) scanf("%d",&Lb.elem[i]);
	MergeList_Sq(La,Lb,&Lc);
	printf("\n归并得线性表c为：\n");
	for(i=0;i<Lc.length;i++) printf("%d ",Lc.elem[i]);
	return 0;
}//建立并归并 

