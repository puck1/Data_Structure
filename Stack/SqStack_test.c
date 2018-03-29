
typedef int SElemType; 	//测试所用栈中存储元素暂定为int 

#include <stdio.h>
#include "SqStack.h" 

	//- - - - - 通用基本函数的算法描述 - - - - - 
Status Print(SElemType *p){
	//输出访问单元的值
	printf("%d ",*p); 
}//visit

int main(){
	//Operation;
	SqStack S;
	SElemType Se1,Se2,Se;
	printf("以顺序存储结构创建栈S...\n");
	if(InitStack(&S)) printf("初始化成功...\n");
	int test[99] = {[0 ... 98]=1}; int i;
	printf("将栈底99项置1...\n");
	for(i=0;i<99;++i) Push(&S,test[i]);
	printf("此时栈元素个数为：%d,栈的总存储空间为：%d\n\n",StackLength(S),S.stacksize);
	printf("请输入第100项："); scanf("%d",&Se1); Push(&S,Se1); 	GetTop(S,&Se); printf("此时栈顶元素为：%d\n",Se);
	printf("请输入第101项："); scanf("%d",&Se2); Push(&S,Se2); 	GetTop(S,&Se); printf("此时栈顶元素为：%d\n",Se);
	printf("此时栈元素个数为：%d,栈的总存储空间为：%d\n\n",StackLength(S),S.stacksize);
	printf("此时栈顶到栈底各项为："); StackTraverse(S,&Print); printf("\n\n");
	printf("栈顶两项出栈...\n");
	Pop(&S,&Se); printf("%d ",Se);
	Pop(&S,&Se); printf("%d出栈操作成功...\n",Se);
	GetTop(S,&Se); printf("此时栈顶元素为：%d\n",Se);
	printf("此时栈元素个数为：%d,栈的总存储空间为：%d\n\n",StackLength(S),S.stacksize);
	printf("此时栈顶到栈底各项为："); StackTraverse(S,&Print); printf("\n\n");
	printf("清空栈S...\n"); ClearStack(&S);
	StackEmpty(S)?printf("栈S已置空..."):printf("栈S未空！"); printf("\n");
	printf("销毁栈S...\n"); DestroyStack(&S); 
	S.base == NULL && S.top == NULL && S.stacksize == 0?
			printf("栈S已销毁..."):printf("栈S未销毁！"); printf("\n");
	return 0;
}//顺序存储栈的初始化/入栈/出栈/查询/遍历/清空/销毁
