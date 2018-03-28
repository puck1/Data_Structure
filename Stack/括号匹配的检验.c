
typedef char SElemType; 	//栈中存储元素定为char

#include <stdio.h>
#include "status.h"
#include "SqStack.h"

Status BracketTest(){
	//输入含'[',']','(',')'的表达式，检验其括号是否正确匹配
	//若正确匹配，则返回TRUE，否则返回FALSE并给出原因 
	SqStack BRACKET;SqStack EXPECT;		//栈BRACKET存储给出的括号类型，EXPECT存储期望
	char str[100]; SElemType e;
	int i = 0; int Bcnt = 0;	//Bcnt为括号计数 
	InitStack(&BRACKET); InitStack(&EXPECT);
	printf("请输入要进行括号匹配检验的括号表达式："); 
	scanf("%s",str);
	while(str[i]){
		if(str[i] == '('){			//'('入栈 
			++Bcnt;			
			Push(&BRACKET,str[i]);
			Push(&EXPECT,')');		//期望为')' 
		}
		else if(str[i] == '['){		//'['入栈
			++Bcnt;			
			Push(&BRACKET,str[i]);
			Push(&EXPECT,']');		//期望为']'
		}
		else if(str[i] == ')'||str[i] == ']'){
			++Bcnt;
			GetTop(EXPECT,&e);
			if(str[i] == e){		//期望匹配，将匹配括号从栈中移除 
				Pop(&BRACKET,&e);
				Pop(&EXPECT,&e);
			}
			else{
				GetTop(EXPECT,&e);
				printf("第 %d 个括号格式不匹配！正确格式应为 '%c'\n",Bcnt,e);
				DestroyStack(&BRACKET);
				DestroyStack(&EXPECT);
				return FALSE;
			}
		}
		++i;
	}
	if(!StackEmpty(BRACKET)){		//仍有括号未被匹配 
		GetTop(EXPECT,&e);
		printf("缺少括号类型 '%c' ！\n",e);
		DestroyStack(&BRACKET);
		DestroyStack(&EXPECT);
		return ERROR;
	}
	else{
		printf("括号格式正确。\n");
		DestroyStack(&BRACKET);
		DestroyStack(&EXPECT);
		return OK;
	}
}//BracketTest 

int main(){
	BracketTest();
	return 0;
}//括号匹配的检验
