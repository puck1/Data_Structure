/**
 * This file defines a stack structure to storage operands.
 */
  
#ifndef OPNDSTACK_H
#define OPNDSTACK_H 

#include <stdio.h>
#include <stdlib.h>
#include "status.h"

	//- - - - - 运算数栈的顺序存储表示 - - - - - 
#define STACK_INIT_SIZE 100	//存储空间初始分配量 
#define STACKINCREMENT 10	//存储空间分配增量
typedef struct{
	OperandType *base;		//在栈构造之前和销毁之后，base的值为NULL 
	OperandType *top;		//栈顶指针 
	int stacksize;			//当前已分配的存储空间，以元素为单位 
}OPNDStack;
	//- - - - - - - - - - - - - - - - - - - 

	//- - - - - 基本操作的算法描述 - - - - - 
Status InitStack_OPND(OPNDStack *S){
	//构造一个空栈S
	S->base = (OperandType *)malloc(STACK_INIT_SIZE * sizeof(OperandType));
	if(!S->base) exit(OVERFLOW);	//存储分配失败 
	S->top = S->base;
	S->stacksize = STACK_INIT_SIZE;
	return OK; 
}//InitStack_OPND

Status DestroyStack_OPND(OPNDStack *S){
	//销毁栈S，S不再存在
	free(S->base);
	S->base = S->top = NULL;
	S->stacksize = 0;
	S = NULL;
	return OK;
}//DestroyStack_OPND

Status ClearStack_OPND(OPNDStack *S){
	//把S置为空栈
	OperandType *p = S->top;
	OperandType Se;
	while(p != S->base){
		Pop_OPND(S,&Se);
		p = S->top;
	}
	return OK;
}//ClearStack_OPND

Status StackEmpty_OPND(OPNDStack S){
	//若栈S为空栈，则返回TRUE，否则返回FALSE
	if(S.base == S.top) return TRUE;
	return FALSE;
}//StackEmpty_OPND

int StackLength_OPND(OPNDStack S){
	//返回S的元素个数，即栈的长度
	return S.top - S.base;
}//StackLength_OPND

Status GetTop_OPND(OPNDStack S,OperandType *e){
	//若栈不空，则用e返回S的栈顶元素，并返回OK；否则返回ERROR
	if(S.base == S.top) return ERROR;
	*e = *(S.top-1); return OK;
}//GetTop_OPND

Status Push_OPND(OPNDStack *S,OperandType e){
	//插入元素e为新的栈顶元素
	if((S->top)-(S->base) >= S->stacksize){	//栈满，追加存储空间 
		S->base = (OperandType *)realloc(S->base,
					(S->stacksize + STACKINCREMENT) * sizeof(OperandType));
		if(!S->base) exit(OVERFLOW);	//存储分配失败 
		S->stacksize += STACKINCREMENT;
	}
	*S->top++ = e;
	return OK; 
}//Push_OPND

Status Pop_OPND(OPNDStack *S,OperandType *e){
	//若栈不空，则删除S的栈顶元素，用e返回其值，并返回OK；否则返回ERROR
	if(S->top == S->base) return ERROR;
	*e = *--S->top; return OK;
}//Pop_OPND

Status StackTraverse_OPND(OPNDStack S,Status (*visit)(OperandType *p)){
	//从栈底到栈顶依次对栈中每个元素调用函数visit()。一旦visit()失败，则操作失败
	OperandType *p = S.top;
	while(p != S.base){
		if(!(*visit)(p-1)) return ERROR;
		p--;
	}
	return OK;
}//StackTraverse_OPND 
	
#endif

