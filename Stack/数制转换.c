
#include <stdio.h> 
#include "SqStack.h"

void conversion(){
	//对于输入的任意一个非负十进制整数，打印输出与其等值的八进制数
	int N; SqStack S; SElemType e;
	InitStack(&S);		//构造空栈 
	printf("请输入一个非负十进制整数N："); scanf("%d",&N);
	while(N){			//除基取余入栈 
		Push(&S,N % 8);
		N /= 8;
	}
	printf("转换成八进制数为："); 
	while(!StackEmpty(S)){
		Pop(&S,&e);		//从高位到低位输出 
		printf("%d",e);
	}
	DestroyStack(&S);
}//conversion 

int main(){
	conversion(); 
	return 0;
}//十进制数转换为八进制数 
