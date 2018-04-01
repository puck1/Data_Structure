
typedef int QElemType;		//测试队列存储数据类型定为int 

#include <stdio.h>
#include "CylSqQueue.h"

// - - - - - 通用基本函数的算法描述 - - - - - 
Status print(QElemType elem){
	//输出访问队列元素的值
	printf("%d ",elem);
	return OK;
}//print 

int main(){
	//Operation;
	SqQueue Q;
	QElemType e; QElemType tmp;
	if(InitQueue(&Q)) printf("空队列Q构造成功...请在Q中逐步插入5个值...\n");
	int i;
	for(i=0;i<5;i++){
		scanf("%d",&e);
		if(EnQueue(&Q,e)) printf("插入元素 %d 成功！\n",e);
	}
	printf("此时队列Q中有 %d 个元素，从队首到队尾元素依次为：",QueueLength(Q));
	QueueTraverse(Q,print); printf("\n");
	DeQueue(&Q,&e); GetQHead(Q,&tmp);
	printf("删除队头元素 %d 后，新的队头元素为 %d \n",e,tmp);
	printf("插入元素100为Q的新的队尾元素后，");  EnQueue(&Q,100);
	printf("此时队列Q中有 %d 个元素，从队首到队尾元素依次为：",QueueLength(Q));
	QueueTraverse(Q,print); printf("\n");
	printf("测试完成...");
	ClearQueue(&Q); if(QueueEmpty(Q)) printf("队列Q已置空...\n");
	DestroyQueue(&Q); if((!Q.front)&&(!Q.rear)&&(!Q.base)) printf("队列Q已被销毁...\n");
	return 0;
}//队列的循环顺序表示与实现
