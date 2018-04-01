
#ifndef SQARRAY_H
#define SQARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>			//标准头文件，提供宏va_start/va_arg和va_end，
							//用于存取变长参数表 
#include "status.h"

#define MAX_ARRAY_DIM 8		//假设数组维数的最大值为8
 
typedef struct {
	AElemType	*base;		//数组元素基址，由InitArray分配 
	int			dim;		//数组维数 
	int			*bounds;	//数组维界基址，由InitArray分配 
	int			*constants;	//数组映像函数常量基址，由InitArray分配 
}SqArray;

//- - - - - 基本操作的算法描述 - - - - - 
Status InitArray(SqArray *A,int dim,...){
	//若维数dim和随后的各维长度合法，则构造相应的数组A，并返回OK。
	if(dim<1 || dim>MAX_ARRAY_DIM) return ERROR;
	A->dim = dim;
	A->bounds = (int *)malloc(dim * sizeof(int));
	if(!A->bounds) exit(OVERFLOW);
	//若各维长度合法，则存入A.bounds，并求出A的元素总数elemtotal
	int elemtotal = 1;
	va_list ap;
	va_start(ap,dim);		//ap为va_list类型，是存放变长参数表信息的数组 
	int i;
	for(i=0;i<dim;++i){
		A->bounds[i] = va_arg(ap,int);
		if(A->bounds[i] < 0) return UNDERFLOW;
		elemtotal *= A->bounds[i];
	}
	va_end(ap);
	A->base = (AElemType *)malloc(elemtotal * sizeof(AElemType));
	if(!A->base) exit(OVERFLOW);
	//求映像函数的常数ci，并存入A.constants[i-1],i=1,...,dim
	A->constants = (int *)malloc(dim * sizeof(int));
	if(!A->constants) exit(OVERFLOW);
	A->constants[dim-1] = 1;	//L=1，指针的增减以元素的大小为单位
	for(i=dim-2;i>=0;--i)
		A->constants[i] = A->bounds[i+1] * A->constants[i+1];
	return OK;
}//InitArray

Status DestroyArray(SqArray *A){
	//销毁数组A。
	if(!A->base) return ERROR;
	free(A->base); A->base = NULL;
	if(!A->bounds) return ERROR;
	free(A->bounds); A->bounds = NULL;
	if(!A->constants) return ERROR;
	free(A->constants); A->constants = NULL;
	A->dim = 0;
	return OK;
}//DestroyArray

Status Locate(SqArray A,va_list ap,int *off){
	//若ap指示的各下标值合法，则求出该元素在A中相对地址off
	*off = 0;
	int i,j;
	for(i=0;i<A.dim;++i){
		j = va_arg(ap,int);
		if(j<0 || j>=A.bounds[i]) return OVERFLOW;		//下标值不合法 
		*off += A.constants[i] * j;
	}
	return OK;
}//Locate 

Status Value(SqArray A,AElemType *e,...){
	//A是n维数组，e为元素变量，随后是n个下标值。
	//若下标不超界，则e赋值为所指定的A的元素值，并返回OK。
	va_list ap;
	va_start(ap,*e);
	int off;
	if(Locate(A,ap,&off) <= 0) return OVERFLOW;
	*e = *(A.base + off);
	return OK;
}//Value

Status Assign(SqArray *A,AElemType e,...){
	//A是n维数组，e为元素变量，随后是n个下标值。
	//若下标不超界，则将e的值赋给所指定的A的元素，并返回OK
	va_list ap;
	va_start(ap,e);
	int off;
	if(Locate(*A,ap,&off) <= 0) return OVERFLOW;
	*(A->base + off) = e;
	return OK;
}//Assign 

void PrintArray(SqArray A){
	//将数组A顺序打印出来
	int i,totalelem=1;
	for(i=0;i<A.dim;++i){
		totalelem *= A.bounds[i];
	}
	for(i=0;i<totalelem;++i){
		printf("%d ",A.base[i]);	//数组存储的数据类型暂定为int 
	}
	printf("\n"); 
}//PrintArray

#endif

