
typedef int QElemType;		//测试所用队列存储的数据类型定为int 

#include "LinkQueue.h"

// - - - - - 通用基本函数的算法描述 - - - - - 
Status print(QueuePtr Ptr){
	//输出访问队列元素的值
	printf("%d ",Ptr->data);
	return OK;
}//print 

int main(){
	//Operation;
	LinkQueue Q;
	QElemType e; QElemType tmp;
	if(InitQueue(&Q)) printf("空队列Q构造成功...请在Q中逐步插入5个值...\n");
	int i;
	for(i=0;i<5;i++){
		scanf("%d",&e);
		if(EnQueue(&Q,e)) printf("插入元素 %d 成功！\n",e);
	}
	printf("此时队列Q中有 %d 个元素，从队首到队尾元素依次为：",QueueLength(Q));
	QueueTraverse(Q,print); printf("\n");
	DeQueue(&Q,&e); GetHead(Q,&tmp);
	printf("删除队头元素 %d 后，新的队头元素为 %d \n",e,tmp);
	printf("插入元素100为Q的新的队尾元素后，");  EnQueue(&Q,100);
	printf("此时队列Q中有 %d 个元素，从队首到队尾元素依次为：",QueueLength(Q));
	QueueTraverse(Q,print); printf("\n");
	printf("测试完成...");
	ClearQueue(&Q); if(QueueLength(Q)==0 && QueueEmpty(Q)) printf("队列Q已置空...\n");
	DestroyQueue(&Q); if((!Q.front)&&(!Q.rare)) printf("队列Q已被销毁...\n");
	return 0;
}//队列的链式表示与实现 
 
