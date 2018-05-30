
typedef char SElemType;		//栈中存储元素定为char

#include <stdio.h>
#include <stdlib.h>
#include "SqStack.h"

Status StackToFile(SqStack S){
	//把栈中存储的数据写入一个文本文件data_tmp中
	FILE *fp;
	fp = fopen("data_tmp.txt","a+");	//a+：写入的数据会被加到文件尾后
	if(fp == NULL){
		printf("File cannot open!");
		exit(ERROR);
	}
	// fseek(fp,0L,SEEK_END);	//fp定位到文件末尾续写
	SElemType *p = S.base;
	while(p != S.top){		//从栈底到栈顶输出字符
		fprintf(fp,"%c",*p);
		++p;
	}
	fclose(fp);
	return OK;
}//StackToFile

Status FileTraverse(){
	//读文本文件data_tmp并打印
	FILE *fp = fopen("data_tmp.txt","r");
	if(fp == NULL){
		printf("File cannot open!");
		exit(ERROR);
	}
	char ch = fgetc(fp);	//fgetc从文件指针指向的文件中读取一个字符,光标位置后移一个字节
							//如果读到文件末尾或者读取出错时返回EOF
	while(ch != EOF){
		printf("%c",ch);
		ch = fgetc(fp);
	}
	return OK;
}//FileTraverse

void LinkEdit(Status (*transfer)(SqStack)){
	//利用字符栈S，从终端接受一行并传送至调用过程的数据区。
	SqStack S;
	InitStack(&S);
	char ch = getchar();	//从终端接收第一个字符
	while(ch != EOF){		//EOF为全文结束符
		while(ch != EOF && ch != '\n'){
			switch(ch){
				case '#': Pop(&S,&ch); 		break;	//退栈
				case '@': ClearStack(&S); 	break;	//重置S为空栈
				default: Push(&S,ch); 		break;	//有效字符进栈
			}//switch
			ch = getchar();	//从终端接收下一个字符
		}
		if(ch == '\n') Push(&S,ch);	//换行符入栈
		(*transfer)(S);		//将从栈底到栈顶的栈内字符传送至缓冲区
		ClearStack(&S);		//重置S为空栈
		if(ch != EOF) ch = getchar();
	}
	DestroyStack(&S);
}//LinkEdit

int main(){
	LinkEdit(&StackToFile);
	printf("Transfer successfully!\n"
			"Confirm temporary data? 1/0;  ");
	int OPT; scanf("%d",&OPT);
	if(OPT) FileTraverse();
	return 0;
}//行编辑程序

/**
 * 	FileTraverse的另一种方法：取得文件长度并分配定长字符串，整体输出文件内容。
 * 	此方法问题在于：将字符\n输入文件时，会在文件中输入\n（回车）\r（换行）两个字符；
 *	利用fseek/ftell获取文件长度时，这两个字符都会被计算；
 *	而fread等从文件输入的函数在遇到这两个字符时候，只会读入\n一个字符，而且计数为1。
 *	因此malloc就会多分配空间，从而引发错误。

	fseek(fp,0L,SEEK_END);	//定位到文件末尾
	int flen = ftell(fp);	//ftell指示指针与文件开头的偏移字节数
	char *p = (char*)malloc(flen + 1);
	if(p == NULL){
		fclose(fp);
		exit(OVERFLOW);
	}
	fseek(fp,0L,SEEK_SET);	//定位到文件开头
	fread(p,flen,1,fp);
	p[flen] = '\0';
	printf("%s",p);
 */
