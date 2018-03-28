
#include <stdio.h>
#include <stdlib.h>
#include "status.h" 

//- - - - - 线性表的静态单链表存储结构 - - - - -
#define ElemType 	int	//线性表存放的数据类型
#define MAXSIZE 1000		//链表的最大长度
typedef struct{
	ElemType data;
	int cur;
}component,SLinkList[MAXSIZE]; 
//- - - - - - - - - - - - - - - - - - - - - - - -

int LocateELem_SL(SLinkList space,int S,ElemType e){
	//在静态单链线性表L中查找第1个值为e的元素。
	//若找到，则返回它在L中的位序，否则返回0。
	//S为其头结点。 
	int i = space[S].cur; int count = 1;	//i指示表中第一个结点 
	while(i&&space[i].data!=e){
		i = space[i].cur; ++count; 	//在表中顺链查找★ 
	}
	if(!i) return i;
	else return count;	 	 
}//LocateElem_SL 

void InitSpace_SL(SLinkList space){
	//将一维数组space中各分量链成一个备用链表，space[0].cur为头指针，
	//"0"表示空指针
	int i;
	for(i=0;i<MAXSIZE-1;++i) space[i].cur = i+1;
	space[MAXSIZE-1].cur = 0; 
}//InitSpace_SL

int Malloc_SL(SLinkList space){
	//若备用空间链表非空，则返回分配的结点下标，否则返回0
	int i = space[0].cur;
	if(space[0].cur) space[0].cur = space[i].cur;
	return i;
}//Malloc_SL

void Free_SL(SLinkList space,int k){
	//将下标为k的空闲节点回收到备用链表 
	space[k].cur = space[0].cur; space[0].cur = k;
}//Free_SL

Status ListInsert_SL(SLinkList space,int S,int k,ElemType e){
	//在有备用链表的静态单链线性表L中第k个位置之前插入元素e， 
	//S为其头指针。假设备用空间足够大，space[0].cur为其头指针。 
	int i = S; int count = 0;
	while(i&&count<k-1) {
		i = space[i].cur; ++count;	//寻找处于第k-1位置的结点 
	}
	if(count!=k-1) return ERROR;		//插入位置不合理 
	int j = Malloc_SL(space);		//生成新结点 
	space[j].data = e; space[j].cur = space[i].cur; //插入S中 
	space[i].cur = j;
	return OK; 
}//ListInsert_SL

Status ListDelete_SL(SLinkList space,int S,int k,ElemType *e){
	//在带有备用链表的静态单链线性表中，删除位于第k个位置的元素，并用e返回其值
	int i = S; int count = 0;
	while(space[i].cur&&count<k-1){		//寻找位于第k个位置的元素，并令i指向其前趋 
		i = space[i].cur; ++count;
	}
	if(!space[i].cur||count>k-1) return ERROR;		//删除位置不合理 
	int j = space[i].cur; space[i].cur = space[j].cur;	//删除并释放结点 
	*e = space[j].data; Free_SL(space,j);
	return OK;
}//ListDelete_SL

Status ListTraverse_SL(SLinkList space,int S){
	//按顺序遍历静态单链表，并输出各项的值，S为其头指针
	 int i = space[S].cur;
	 int count = 1;
	 if (!i) return ERROR;
	 while(i){
	 	printf("静态单链表的第%d项为%d;\n",count,space[i].data);
	 	i = space[i].cur;
	 	count++;
	 }
	 return OK;
}//ListTraverse_SL

void difference(SLinkList space,int *S){
	//依次输入集合A和B的元素，在一维数组space中建立表示集合(A-B)∪(B-A)
	//的静态链表，S为其头指针。假设备用空间足够大，space[0].cur为其头指针。
	InitSpace_SL(space);			//初始化备用空间 
	*S = Malloc_SL(space);			//生成S的头结点 
	int r = *S;				//r指向S的当前最后结点★
	int num_a,num_b;int data_b;
	int i; int a,b;	
	printf("请分别输入A与B的元素个数，" 
			"A："); scanf("%d",&num_a); 
	printf	("B:"); scanf("%d",&num_b);		//输入A和B的元素个数 
	printf("- - - - - - - - - - - - - - - - - - - - - - - - - \n");	
	for(i=0;i<num_a;++i){				//建立集合A的链表 
		a = Malloc_SL(space);			//分配结点 
		printf("请输入A的第%d个值：",i+1);
		scanf("%d",&space[a].data);		//输入A的元素值 
		space[r].cur = a; r = a;		//输入到表尾★ 
	}//for 
	space[r].cur = 0;				//尾结点的指针为空★
	printf("- - - - - - - - - - - - - - - - - - - - - - - - - \n");	
	for(i=0;i<num_b;++i){				//依次输入B的元素，
							//若不在当前表中，则插入，否则删除 									
		printf("请输入B的第%d个值:",i+1);
		scanf("%d",&data_b);
		int p = *S;int k = space[*S].cur;	//k指向集合A中第一个结点★(p指向其前趋) 
		while(k!=space[r].cur&&space[k].data!=data_b){//在当前表中查找
								//(也写成k&&space[k].data!=data_b) 
			p = k; k = space[k].cur;	//★ 
		}//while
		if(k == space[r].cur){			//当前表中不存在该元素，
							//插入在r所指结点之后，且r的位置不变
			int j = Malloc_SL(space);
			space[j].data = data_b;
			space[j].cur = space[r].cur;
			space[r].cur = j;
		}//if
		else{					//该元素已在表中，删除之(也写成if(k))
			space[p].cur = space[k].cur;
			Free_SL(space,k);
			if(r == k) r = p;		//若删除的是r所指结点，则需修改尾指针★ 
		}//else 
	}//for
}//difference

int main()
{
	//Operation;
	printf("请选择对应操作：\n"
	"0:带备用链表的静态单链表相关操作；\n"
	"1:求集合(A-B)∪(B-A)的操作;\n");
	printf("请输入选择的操作码：");
	int opt; scanf("%d",&opt); 
	
	if(opt == 0){
		printf("\n- - - - - 以下为带备用链表的静态单链表基本操作- - - - - \n");
		SLinkList space;
		InitSpace_SL(space);
		int S = Malloc_SL(space);
		int data1 = Malloc_SL(space);
		int data2 = Malloc_SL(space);
		int data3 = Malloc_SL(space);
		int data4 = Malloc_SL(space);
		int data5 = Malloc_SL(space);
		space[data5].cur = 0;			//★(建立时要使最后一项cur为0)	 
		printf("请输入静态链表的五项，中间以空格隔开:"); 
		scanf("%d %d %d %d %d",&space[data1].data,&space[data2].data,
			&space[data3].data,&space[data4].data,&space[data5].data);
		printf("建立后的静态单链表各项为：\n");
		ListTraverse_SL(space,S);
		printf("其中存放数据5的项的位序为：");
		int place = LocateELem_SL(space,S,5); printf("%d\n",place); 		
		ListInsert_SL(space,S,2,3*space[data1].data);
		printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - \n");		
		printf("在第二项前插入(3*第一项)后的静态单链表各项为：\n");	
		ListTraverse_SL(space,S);
		int e;
		ListDelete_SL(space,S,5,&e);
		printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - \n");		
		printf("在静态单链表的删除中，被删除的第五项值为：%d\n",e);
		printf("删除后的静态单链表各项为：\n");	
		ListTraverse_SL(space,S);
	}	
	
	else if(opt == 1){	
		printf("\n- - - - - 以下为求集合(A-B)∪(B-A)的操作- - - - - \n");	
		SLinkList space; int S;
		difference(space,&S);
		printf("- - - - - - - - - - - - - - - - - - - - - - - - - \n"
				"集合(A-B)∪(B-A)中各项值为：\n");
		ListTraverse_SL(space,S);
	}
	
	else return ERROR; 
	return 0;
}//静态单链表初始化/查找/增删/遍历/求集合(A-B)∪(B-A)

