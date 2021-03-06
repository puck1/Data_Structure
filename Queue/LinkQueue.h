
#ifndef LINKQUEUE_H
#define LINKQUEUE_H 

#include <stdio.h>
#include <stdlib.h>
#include "status.h"

// - - - - - 单链队列——队列的链式存储结构 - - - - - 
typedef struct QNode{
	QElemType 		data;
	struct QNode 	*next;
}QNode,*QueuePtr;

typedef struct{
	QueuePtr front;	//队头指针 
	QueuePtr rear;	//队尾指针 
}LinkQueue;

// - - - - - 基本操作的函数算法说明 - - - - - 
Status InitQueue(LinkQueue *Q){
	//构造一个空队列Q
	Q->front = Q->rear = (QueuePtr)malloc(sizeof(QNode));
	if(!Q->front) exit(OVERFLOW);		//存储分配失败 
	Q->front->next = NULL;
	return OK;
}//InitQueue

Status DestroyQueue(LinkQueue *Q){
	//销毁队列Q，Q不再存在
	while(Q->front){
		Q->rear = Q->front->next;
		free(Q->front);
		Q->front = Q->rear;
	}
	Q = NULL;
	return OK;
}//DestroyQueue

Status ClearQueue(LinkQueue *Q){
	//将Q清为空队列
	Q->rear = Q->front->next;
	while(Q->rear){
		Q->front->next = Q->rear->next; 
		free(Q->rear);
		Q->rear = Q->front->next; 
	}
	Q->rear = Q->front;
	return OK;
}//ClearQueue

Status QueueEmpty(LinkQueue Q){
	//若队列Q为空队列，则返回TRUE，否则返回FALSE
	return (Q.front == Q.rear)? TRUE:FALSE;
}//QueueEmpty 

int QueueLength(LinkQueue Q){
	//返回Q的元素个数，即为队列的长度
	QueuePtr p = Q.front->next;
	int cnt = 0;
	while(p){
		++cnt;
		p = p->next;
	}
	return cnt;
}//QueueLength

Status GetQHead(LinkQueue Q,QElemType *e){
	//若队列不空，则用e返回Q的队头元素，并返回OK；否则返回ERROR
	if(Q.front->next){
		*e = Q.front->next->data;
		return OK;
	}
	return ERROR;
}//GetQHead

Status EnQueue(LinkQueue *Q,QElemType e){
	//插入元素e为Q的新的队尾元素
	QueuePtr newrear = (QueuePtr)malloc(sizeof(QNode));
	if(!newrear) exit(OVERFLOW);
	newrear->data = e; newrear->next = NULL;
	Q->rear->next = newrear;
	Q->rear = newrear;		//尾指针重新定位★ 
	return OK; 
}//EnQueue

Status DeQueue(LinkQueue *Q,QElemType *e){
	//若队列不空，则删除Q的队头元素，用e返回其值，并返回OK；
	//否则返回ERROR 
	if(Q->front != Q->rear){
		QueuePtr p = Q->front->next;
		Q->front->next = p->next;
		if(p == Q->rear) Q->rear = Q->front;	//队列中最后一个元素被删后，队列尾指针重新定位★ 
		*e = p->data;
		free(p);
		return OK;
	}
	return ERROR;
}//DeQueue

Status QueueTraverse(LinkQueue Q,Status visit(QueuePtr)){
	//从队头到队尾依次对队列Q中每个元素调用函数visit()。一旦visit失败，则操作失败。
	QueuePtr p = Q.front->next;
	while(p){
		if(!visit(p)) return ERROR;
		p = p->next;
	}
	return OK;
}//QueueTraverse

#endif

