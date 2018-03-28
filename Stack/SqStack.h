
#ifndef SQSTACK_H
#define SQSTACK_H 

#include <stdio.h>
#include <stdlib.h>
#include "status.h"

	//- - - - - 栈的顺序存储表示 - - - - - 
#define STACK_INIT_SIZE 100	//存储空间初始分配量 
#define STACKINCREMENT 10	//存储空间分配增量
typedef struct{
	SElemType *base;	//在栈构造之前和销毁之后，base的值为NULL 
	SElemType *top;		//栈顶指针 
	int stacksize;		//当前已分配的存储空间，以元素为单位 
}SqStack;
	//- - - - - - - - - - - - - - - - - - - 

	//- - - - - 基本操作的算法描述 - - - - - 
Status InitStack(SqStack *S){
	//构造一个空栈S
	S->base = (SElemType *)malloc(STACK_INIT_SIZE * sizeof(SElemType));
	if(!S->base) exit(OVERFLOW);	//存储分配失败 
	S->top = S->base;
	S->stacksize = STACK_INIT_SIZE;
	return OK; 
}//InitStack

Status DestroyStack(SqStack *S){
	//销毁栈S，S不再存在
	free(S->base);
	S->base = S->top = NULL;
	S->stacksize = 0;
	S = NULL;
	return OK;
}//DestroyStack

Status ClearStack(SqStack *S){
	//把S置为空栈
	SElemType *p = S->top;
	SElemType Se;
	while(p != S->base){
		Pop(S,&Se);
		p = S->top;
	}
	return OK;
}//ClearStack

Status StackEmpty(SqStack S){
	//若栈S为空栈，则返回TRUE，否则返回FALSE
	if(S.base == S.top) return TRUE;
	return FALSE;
}//StackEmpty

int StackLength(SqStack S){
	//返回S的元素个数，即栈的长度
	return S.top - S.base;
}//StackLength

Status GetTop(SqStack S,SElemType *e){
	//若栈不空，则用e返回S的栈顶元素，并返回OK；否则返回ERROR
	if(S.base == S.top) return ERROR;
	*e = *(S.top-1); return OK;
}//GetTop

Status Push(SqStack *S,SElemType e){
	//插入元素e为新的栈顶元素
	if((S->top)-(S->base) >= S->stacksize){	//栈满，追加存储空间 
		S->base = (SElemType *)realloc(S->base,
					(S->stacksize + STACKINCREMENT) * sizeof(SElemType));
		if(!S->base) exit(OVERFLOW);	//存储分配失败 
		S->stacksize += STACKINCREMENT;
	}
	*S->top++ = e; 
	return OK; 
}//Push

Status Pop(SqStack *S,SElemType *e){
	//若栈不空，则删除S的栈顶元素，用e返回其值，并返回OK；否则返回ERROR
	if(S->top == S->base) return ERROR;
	*e = *--S->top; return OK;
}//Pop

Status StackTraverse(SqStack S,Status (*visit)(SElemType *p)){
	//从栈底到栈顶依次对栈中每个元素调用函数visit()。一旦visit()失败，则操作失败
	SElemType *p = S.top;
	while(p != S.base){
		if(!(*visit)(p-1)) return ERROR;
		p--;
	}
	return OK;
}//StackTraverse 
	
#endif
 
