
#ifndef CYLSQQUEUE_H
#define CYLSQQUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include "status.h"

// - - - - - 循环队列——队列的顺序存储结构 - - - - - 
#define MAXQSIZE 100	//最大队列长度
typedef struct{
	QElemType *base;	//初始化的动态分配存储空间 
	int front;			//头指针，若队列不空，指向队列头元素 
	int rare;			//尾指针，如队列不空，指向队列尾元素的下一个位置 
}SqQueue;

// - - - - - 循环队列的基本操作的算法描述 - - - - - 
Status InitQueue(SqQueue *Q){
	//构造一个空队列Q
	Q->base = (QElemType *)malloc(MAXQSIZE * sizeof(QElemType));
	if(!Q->base) exit(OVERFLOW);	//存储分配失败 
	Q->front = Q->rare = 0;
	return OK;
}//InitQueue

Status DestroyQueue(SqQueue *Q){
	//销毁队列Q，Q不再存在
	free(Q->base);
	Q->base = NULL;
	Q->front = Q->rare = 0;
	Q = NULL;
	return OK;
}//DestroyQueue

Status ClearQueue(SqQueue *Q){
	//将Q清为空队列
	Q->front = Q->rare = 0;
	return OK;
}//ClearQueue

Status QueueEmpty(SqQueue Q){
	//若队列Q为空队列，则返回TRUE，否则返回FALSE
	return (Q.front == Q.rare)? TRUE:FALSE;		//Q的头指针与尾指针相等则Q为空 
}//QueueEmpty 

int QueueLength(SqQueue Q){
	//返回Q的元素个数，即队列的长度
	return ((Q.rare - Q.front + MAXQSIZE) % MAXQSIZE);	//★
}//QueueLength

Status GetHead(SqQueue Q,QElemType *e){
	//若队列不空，则用e返回Q的队头元素，并返回OK；否则返回ERROR
	if(Q.front != Q.rare){
		*e = Q.base[Q.front];
		return OK;
	}
	return ERROR;
}//GetHead

Status EnQueue(SqQueue *Q,QElemType e){
	//插入元素e为Q的新的队尾元素
	if(Q->front == (Q->rare + 1) % MAXQSIZE) return ERROR;	//队列满（队列头指针在尾指针的下一位置上）★
	(*Q).base[Q->rare] = e;
	Q->rare = (Q->rare + 1) % MAXQSIZE;		//所有的整型指针加1操作都要写成循环形式★
	return OK;
}//EnQueue

Status DeQueue(SqQueue *Q,QElemType *e){
	//若队列不空，则删除Q的队头元素，用e返回其值，并返回OK；
	//否则返回ERROR
	if(Q->front != Q->rare){
		*e = (*Q).base[Q->front];
		Q->front = (Q->front + 1) % MAXQSIZE;
		return OK;
	}
	return ERROR;
}//DeQueue

Status QueueTraverse(SqQueue Q,Status visit(QElemType)){
	//从队头到队尾依次对队列Q中每个元素调用函数visit()。一旦visit失败，则操作失败。
	int tmp = Q.front;
	while(tmp != Q.rare){
		if(!visit(Q.base[tmp])) return ERROR;
		tmp = (tmp + 1) % MAXQSIZE;
	}
	return OK;	
}//QueueTraverse

#endif 
