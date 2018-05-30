

typedef int AElemType;		//测试数组存储数据类型定为int

#include <stdio.h>
#include "SqArray.h"

int main(){
	SqArray A;
	InitArray(&A,3,3,3,3);		//用3*3*3的三维数组进行测试
	int i, j, k;
	AElemType e = 1;
	printf("建立3*3*3三维数组A...\n");
	for(i = 0; i < A.bounds[0]; ++i)
		for(j = 0; j < A.bounds[1]; ++j)
			for(k = 0; k < A.bounds[2]; ++k){
				Assign(&A,e,i,j,k);
				++e;
			}
	printf("建立的数组A为：");
	PrintArray(A);
	Value(A,&e,0,0,0); printf("A[0][0][0] = %d;\t",e);
	Value(A,&e,0,1,2); printf("A[0][1][2] = %d;\t",e);
	Value(A,&e,2,2,2); printf("A[2][2][2] = %d;\n",e);
	DestroyArray(&A);
	(A.base && A.bounds && A.constants && A.dim != 0)? printf("数组A未销毁！\n"): printf("数组A已销毁！\n");
	return 0;
}//顺序存储的数组的建立/遍历/存取/销毁

