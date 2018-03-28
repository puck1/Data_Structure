/**
 * This file defines a stack structure to storage operators.
 */
  
#ifndef OPTRSTACK_H
#define OPTRSTACK_H 

#include <stdio.h>
#include <stdlib.h>
#include "status.h"

	//- - - - - 运算数栈的顺序存储表示 - - - - - 
#define STACK_INIT_SIZE 100	//存储空间初始分配量 
#define STACKINCREMENT 10	//存储空间分配增量
typedef struct{
	OperatorType *base;		//在栈构造之前和销毁之后，base的值为NULL 
	OperatorType *top;		//栈顶指针 
	int stacksize;			//当前已分配的存储空间，以元素为单位 
}OPTRStack;
	//- - - - - - - - - - - - - - - - - - - 

	//- - - - - 基本操作的算法描述 - - - - - 
Status InitStack_OPTR(OPTRStack *S){
	//构造一个空栈S
	S->base = (OperatorType *)malloc(STACK_INIT_SIZE * sizeof(OperatorType));
	if(!S->base) exit(OVERFLOW);	//存储分配失败 
	S->top = S->base;
	S->stacksize = STACK_INIT_SIZE;
	return OK; 
}//InitStack_OPTR

Status DestroyStack_OPTR(OPTRStack *S){
	//销毁栈S，S不再存在
	free(S->base);
	S->base = S->top = NULL;
	S->stacksize = 0;
	S = NULL;
	return OK;
}//DestroyStack_OPTR

Status ClearStack_OPTR(OPTRStack *S){
	//把S置为空栈
	OperatorType *p = S->top;
	OperatorType Se;
	while(p != S->base){
		Pop_OPTR(S,&Se);
		p = S->top;
	}
	return OK;
}//ClearStack_OPTR

Status StackEmpty_OPTR(OPTRStack S){
	//若栈S为空栈，则返回TRUE，否则返回FALSE
	if(S.base == S.top) return TRUE;
	return FALSE;
}//StackEmpty_OPTR

int StackLength_OPTR(OPTRStack S){
	//返回S的元素个数，即栈的长度
	return S.top - S.base;
}//StackLength_OPTR

Status GetTop_OPTR(OPTRStack S,OperatorType *e){
	//若栈不空，则用e返回S的栈顶元素，并返回OK；否则返回ERROR
	if(S.base == S.top) return ERROR;
	*e = *(S.top-1); return OK;
}//GetTop_OPTR

Status Push_OPTR(OPTRStack *S,OperatorType e){
	//插入元素e为新的栈顶元素
	if((S->top)-(S->base) >= S->stacksize){	//栈满，追加存储空间 
		S->base = (OperatorType *)realloc(S->base,
					(S->stacksize + STACKINCREMENT) * sizeof(OperatorType));
		if(!S->base) exit(OVERFLOW);	//存储分配失败 
		S->stacksize += STACKINCREMENT;
	}
	*S->top++ = e;
	return OK; 
}//Push_OPTR

Status Pop_OPTR(OPTRStack *S,OperatorType *e){
	//若栈不空，则删除S的栈顶元素，用e返回其值，并返回OK；否则返回ERROR
	if(S->top == S->base) return ERROR;
	*e = *--S->top; return OK;
}//Pop_OPTR

Status StackTraverse_OPTR(OPTRStack S,Status (*visit)(OperatorType *p)){
	//从栈底到栈顶依次对栈中每个元素调用函数visit()。一旦visit()失败，则操作失败
	OperatorType *p = S.top;
	while(p != S.base){
		if(!(*visit)(p-1)) return ERROR;
		p--;
	}
	return OK;
}//StackTraverse_OPTR 
	
#endif
 
