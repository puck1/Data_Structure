
typedef char OperatorType;		//运算符用字符存储
typedef float OperandType; 		//运算数用浮点型数存储

#include <stdio.h>
#include <stdlib.h>
#include "OPTRStack.h"
#include "OPNDStack.h"

OperatorType OP[] = {'+','-','*','/','(',')','#'};		//定义全局变量OP为全体运算符集合

float StringToNum(char str[]){
	//将不带运算符的字符串中的数字字符转换成浮点型数，并返回该值
	float integer = 0.0;	//整数部分
	float decimal = 0.0;	//小数部分
	int i = 0;
	while(str[i] != '\0' && str[i] != '.'){		//从头遍历至小数点，顺序求整数部分
		integer = 10.0 * integer + str[i] - 48;	//char型与int型相差48★
		++i;
	}
	if(str[i] == '.'){		//存在小数部分
		while(str[i] != '\0') ++i;
		for(i = i - 1; str[i] != '.'; --i)		//从末尾遍历至小数点，倒序求小数部分
			decimal = decimal / 10.0 + str[i] - 48;
		decimal /= 10.0;
	}
	return (integer + decimal);
}//StringToNum

Status In(char c,OperatorType OP[]){
	//判断c是否是运算符集合OP中的元素，
	//若是，返回TRUE，否则返回FALSE
	int i = 0;
	while(OP[i]){
		if(c == OP[i]) return TRUE;
		else ++i;
	}
	return FALSE;
}//In

OperatorType Precede(OperatorType c1,OperatorType c2){
	//比较两个运算符的优先级，并返回'>','<'或'='
	OperatorType result;
	switch(c2){
		case '+':
		case '-':
			if(c1 == '(' || c1 == '#')
				result =  '<';
			else result = '>'; break;
		case '*':
		case '/':
			if(c1 == '*' || c1 == '/' || c1 == ')')
				result = '>';
			else result = '<'; break;
		case '(':
			if(c1 == ')'){
				printf("括号匹配错误！\n");
				exit(ERROR);
			}
			result = '<'; break;
		case ')':
			if(c1 == '(') result = '=';
			else if(c1 == '#'){
				printf("括号输入错误！\n");
				exit(ERROR);
			}
			else result = '>'; break;
		case '#':
			if(c1 == '('){
				printf("存在多余括号！\n");
				exit(ERROR);
			}
			else if(c1 == '#') result = '=';
			else result = '>'; break;
	}
	return result;
}//Precede

OperandType Operate(OperandType alpha,OperatorType theta,OperandType beta){
	//计算表达式(α theta β)的值，并返回其结果
	float gamma;
	switch(theta){
		case '+': gamma = alpha + beta; break;
		case '-': gamma = alpha - beta; break;
		case '*': gamma = alpha * beta; break;
		case '/': gamma = alpha / beta; break;
	}
	return gamma;
}//Operate

OperandType EvaluateExpression(){
	//算术表达式求值的算符优先算法。设OPTR和OPND分别为运算符栈和运算数栈，
	//OP为运算符集合。
	OPTRStack OPTR; OPNDStack OPND;
	InitStack_OPTR(&OPTR); Push_OPTR(&OPTR,'#');	//栈OPTR寄存运算符，初始将'#'压入栈中，
													//两个'#'相遇表示运算结束

	InitStack_OPND(&OPND); Push_OPND(&OPND,0.0);	//OPND寄存操作数或运算结果,初始将"0.0"压入栈中，
													//当第一个值为负时方便运算
	OperandType alpha,beta;
	OperatorType theta;
	OperatorType tmpoptr;
	OperatorType optr_top;			//OPTR栈顶元素
	GetTop_OPTR(OPTR,&optr_top);
	char c = getchar();
	while(c!='#' || optr_top!='#'){
		if(!In(c,OP)){				//如果不是运算符
			int i = 0;
			char tmpstr[100];
			do{
				tmpstr[i] = c;		//暂存到字符串中
				c = getchar();
				++i;
			}while(!In(c,OP));
			tmpstr[i] = '\0';
			Push_OPND(&OPND,StringToNum(tmpstr));	//将字符串转换成浮点数压入栈OPND中
		}
		else
			switch(Precede(optr_top,c)){
				case '<':	//栈顶元素优先权低
					if(optr_top == '(' && c == '-' && (StackLength_OPTR(OPTR) > StackLength_OPND(OPND)))
						Push_OPND(&OPND,0.0);		//此时进行a*(-b)或a/(-b)的处理，在'-'前补'0.0'
					Push_OPTR(&OPTR,c);
					GetTop_OPTR(OPTR,&optr_top);
					c = getchar();
					break;
				case '=':	//说明遇到括号，脱括号并接受下一字符★
					Pop_OPTR(&OPTR,&tmpoptr);
					GetTop_OPTR(OPTR,&optr_top);
					c = getchar();
					break;
				case '>':	//退栈并将运算结果入栈
					Pop_OPTR(&OPTR,&theta);
					Pop_OPND(&OPND,&beta); Pop_OPND(&OPND,&alpha);
					Push_OPND(&OPND,Operate(alpha,theta,beta));
					GetTop_OPTR(OPTR,&optr_top);
					break;
			}//switch
	}//while
	OperandType result;
	GetTop_OPND(OPND,&result);
	return result;
}//EvaluateExpression

int main(){
	printf("请输入要进行求值的表达式，以#结束：");
	printf("结果为 %g\n",EvaluateExpression());
	return 0;
}//表达式求值
