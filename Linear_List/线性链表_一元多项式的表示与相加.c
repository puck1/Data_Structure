
//- - - - - 抽象数据类型Polynomial的实现 - - - - -
typedef struct {	//项的表示，多项式的项作为LinkList的数据元素
	float  coef;		//系数
	int    expn;		//指数
}term,ElemType;		//两个类型名：term用于本ADT，ElemType为LinkList的数据对象名
//- - - - - - - - - - - - - - - - - - - - - - - - -

#include "status.h"
#include "ExtendedLinkList.h"

typedef LinkList Polynomial;	//用带表头结点的有序链表表示多项式

	//- - - - - 需要增改的一些链表基本函数 - - - - -
	//表示一元多项式的应该是有序链表，其基本操作定义与线性链表有两处不同，
	//一是LocateElem的职责不同，二是需增加按有序关系进行插入的操作OrderInsert
Status OrderLocateElem(LinkList L,ElemType e,Position *q,
										int (*compare)(ElemType,ElemType)){
	//若有序链表L中存在与e满足判定函数compare()取值为0的元素，则q指示L中第一个
	//值为e的节点的位置，并返回TRUE；否则q指示第一个与e满足判定函数compare()取
	//值>0的元素的前驱的位置，并返回FALSE
	Link p = GetHead(L);
	int FIRST = 0;
	while(p->next){
		if((*compare)(p->next->data,e)==0){		//存在与e满足判定函数compare()取值为0的元素
			*q = p->next;						//指示L中第一个值为e的节点的位置
			return TRUE;
		}
		else if((*compare)(p->next->data,e)>0){	//寻找第一个与e满足判定函数
			if(!FIRST){							//compare()取值>0的元素的前驱
				*q = p;
				FIRST = 1;
			}
		}
		p = p->next;
	}
	if(!FIRST)			//不存在与e满足判定函数compare()取值>0的元素
	*q = L.tail;		//q指向尾结点
	return FALSE;
}//OrderLocateElem

Status OrderInsert(LinkList *L,ElemType e,int (*compare)(ElemType,ElemType)){
	//按有序判定函数compare()的约定，将值为e的结点插入到有序链表L的适当位置上
	Link p = L->head->next;
	Link s;
	while(p){
		if((*compare)(p->data,e) > 0){			//寻找值比e大的第一个结点
			if(!MakeNode(&s,e)) return ERROR;
			InsBefore(L,&p,s);					//将值为e的结点插入到该结点之前
			return OK;
		}
		p = p->next;
	}
	if(!MakeNode(&s,e)) return ERROR;
	Append(L,s); return OK;						//如果没有值比e大的结点，则插入到末尾
}//OrderInsert

	//- - - - - 通用基本函数的算法描述 - - - - -
int cmp(term a,term b){
	//依a的指数值<(或=)(或>)b的指数值，分别返回-1、0和+1
	if(a.expn-b.expn<0) return -1;
	else if(a.expn-b.expn>0) return +1;
	return 0;
}//cmp

	//- - - - - 基本操作的函数算法描述 - - - - -
void CreatePolyn(Polynomial *P,int m){
	//输入m项的系数和指数，建立表示一元多项式的有序链表P
	int i; ElemType e; Link s; Link q;
	InitList(P); Link h= GetHead(*P);
	e.coef = 0.0; e.expn = -1; SetCurElem(h,e);		//设置头结点的数据元素
	printf("请输入各项的系数与指数，中间用空格隔开;\n");
	for(i=1;i<=m;++i){	//依次输入m个非零项
		back:printf("请输入第%d项：",i);
		scanf("%f %d",&e.coef,&e.expn);
		if(e.coef == 0.0){							//输入系数为0无意义
			printf("输入系数不能为0，请检查后重新输入！\n");
			goto back;
		}
		if(!OrderLocateElem(*P,e,&q,&cmp)){			//当前链表中不存在该指数项★
			if(MakeNode(&s,e)) InsAfter(P,&q,s);	//生成结点并插入链表
		}
		else{
			printf("当前已存在与其指数相同的项，请检查后重新输入！\n");
			goto back;
		}
	}
	printf("一元多项式建立完成！\n");
}//CreatePolyn

void DestroyPolyn(Polynomial *P){
	//销毁一元多项式P
	DestroyList(P);
}//DestroyPolyn

void PrintPolyn(Polynomial P){
	//打印输出一元多项式P
	Link q = P.head->next;
	while(q){
		if(q->data.coef == 1.0){			//系数为1.0
			if(q != P.head->next)			//第1项系数不打印任何字符
				printf("+");
		}
		else if(q->data.coef == -1.0)		//系数为-1.0
			printf("-");
		else if(q->data.coef > 1.0){		//系数>1.0
			if(q == P.head->next)			//第1项系数不打印'+'
				printf("%g",q->data.coef);
			else
				printf("+%g",q->data.coef);
		}
		else printf("%g",q->data.coef);		//系数<-1.0
		switch(q->data.expn){
			case 0:	if(q->data.coef==1.0||q->data.coef==-1.0)
						printf("1");		//指数为0且系数为1或-1
			 	   	break;					//指数为0系数为其他
			case 1:	printf("x"); break;		//指数为1
			default:printf("x^%d",q->data.expn); break;
		}
		q = q->next;
	}
	printf("\n");
}//PrintPolyn

int PolynLength(Polynomial P){
	//返回一元多项式P中的项数
	return ListLength(P);
}//PolynLength

void AddPolyn(Polynomial *Pa,Polynomial *Pb){
	//完成多项式相加运算，即：Pa = Pa + Pb，并销毁一元多项式Pb
	Link ha = GetHead(*Pa); Link hb = GetHead(*Pb);			//ha和hb分别指向Pa和Pb的头结点
	Link qa = NextPos(*Pa,ha); Link qb = NextPos(*Pb,hb);	//qa和qb分别指向Pa和Pb中当前结点
	while(qa&&qb){	//qa和qb均非空
		term a = GetCurElem(qa); term b = GetCurElem(qb);	//a和b为两表中当前比较元素
		switch(cmp(a,b)){
			case -1:	//多项式Pa中当前结点的指数值小
				ha = qa; qa = NextPos(*Pa,qa); break;
			case 1:		//多项式Pb中当前结点的指数值小
				DelFirst(hb,&qb); InsFirst(ha,qb); ++Pa->len;
				qb = NextPos(*Pb,hb); ha = NextPos(*Pa,ha); break;
			case 0:		//两者的指数值相等
				{float sum = a.coef + b.coef;				//★不要把变量定义在case或goto的label之后，因为变量定义不是语句
				if(!sum){	//删除多项式Pa中当前结点		//解决方法为预先定义或用括号将其与作用域括起来
					if(Pa->tail == qa) Pa->tail = PriorPos(*Pa,qa);	//如果qa是尾结点，则设置新的尾结点为其前趋
					DelFirst(ha,&qa); FreeNode(&qa); --Pa->len;
				}
				else{	//修改多项式Pa中当前结点的系数值
					qa->data.coef = sum; ha = NextPos(*Pa,ha);
				}
				DelFirst(hb,&qb); FreeNode(&qb);
				qb = NextPos(*Pb,hb); qa = NextPos(*Pa,ha);} break;
		}//switch
	}//while
	if(!ListEmpty(*Pb)) Append(Pa,qb);	//链接Pb中剩余结点
	FreeNode(&hb);			//释放Pb的头结点
	Pb->head = Pb->tail = NULL; Pb->len = 0; //设置Pb为销毁状态
}//AddPolyn

void SubtractPolyn(Polynomial *Pa,Polynomial *Pb){
	//完成多项式相减运算，即：Pa = Pa - Pb，并销毁一元多项式Pb
	Link ha = GetHead(*Pa); Link hb = GetHead(*Pb);			//ha和hb分别指向Pa和Pb的头结点
	Link qa = NextPos(*Pa,ha); Link qb = NextPos(*Pb,hb);	//qa和qb分别指向Pa和Pb中当前结点
	while(qa&&qb){	//qa和qb均非空
		term a = GetCurElem(qa); term b = GetCurElem(qb);	//a和b为两表中当前比较元素
		switch(cmp(a,b)){
			case -1:	//多项式Pa中当前结点的指数值小
				ha = qa; qa = NextPos(*Pa,qa); break;
			case 1:		//多项式Pb中当前结点的指数值小
				qb->data.coef = -qb->data.coef;
				DelFirst(hb,&qb); InsFirst(ha,qb); ++Pa->len;
				qb = NextPos(*Pb,hb); ha = NextPos(*Pa,ha); break;
			case 0:		//两者的指数值相等
				{float sub = a.coef - b.coef;
				if(!sub){	//删除多项式Pa中当前结点
					if(Pa->tail == qa) Pa->tail = PriorPos(*Pa,qa);
					DelFirst(ha,&qa); FreeNode(&qa); --Pa->len;
				}
				else{	//修改多项式Pa中当前结点的系数值
					qa->data.coef = sub; ha = NextPos(*Pa,ha);
				}
				DelFirst(hb,&qb); FreeNode(&qb);
				qb = NextPos(*Pb,hb); qa = NextPos(*Pa,ha);} break;
		}//switch
	}//while
	if(!ListEmpty(*Pb)) Append(Pa,qb);	//链接Pb中剩余结点
	while(qb){				//并将它们系数取负
		qb->data.coef = -qb->data.coef;
		qb = NextPos(*Pa,qb);
	}
	FreeNode(&hb);			//释放Pb的头结点
	Pb->head = Pb->tail = NULL; Pb->len = 0; //设置Pb为销毁状态
}//SubtractPolyn

void MultiplyPolyn(Polynomial *Pa,Polynomial *Pb){
	//完成多项式相乘运算，即：Pa = Pa * Pb，并销毁一元多项式Pb
	Link ha = GetHead(*Pa); Link hb = GetHead(*Pb);
	Link qa = NextPos(*Pa,ha); Link qb = NextPos(*Pb,hb);
	term Ea,Eb;
	int i;
	Polynomial Ptmp1; Polynomial Ptmp2;
	InitList(&Ptmp1);		//Ptmp1暂存已取出的Pa项与Pb全项乘积之和，即Ptmp2之和
	while(qa){
		InitList(&Ptmp2);	//Ptmp2暂存Pa当前取出的一项与Pb全项的乘积之和
		DelFirst(ha,&qa);	//取出Pa一项与Pb相乘
		if(qa == Pa->tail) Pa->tail = ha; --Pa->len;	//修改Pa相关参数
		Ea = GetCurElem(qa);
		for(i=1;i<=PolynLength(*Pb);i++){	//分别取Pb各项与Pa一项相乘
			Eb = GetCurElem(qb);
			Eb.coef *= Ea.coef;
			Eb.expn += Ea.expn;
			OrderInsert(&Ptmp2,Eb,&cmp);	//将Pa一项与Pb一项乘积插入Ptmp2中
			qb = qb->next;
		}
		AddPolyn(&Ptmp1,&Ptmp2);
		qa = NextPos(*Pa,ha);
		qb = NextPos(*Pb,hb);
	}
	AddPolyn(Pa,&Ptmp1);	//Pa已为空，将Ptmp1存入Pa中
	DestroyList(Pb);		//销毁Pb
	free(Ptmp1.head);Ptmp1.head = Ptmp1.tail = NULL; Ptmp1.len = 0;	//销毁Ptmp1
	DestroyList(&Ptmp2);	//销毁Ptmp2
}//MultiplyPolyn

int main(){
	//Operation;
	int OPT;
	Polynomial Pa,Pb;
	int la,lb;

	printf("建立多项式Pa...\n""请输入Pa的项数：");
	scanf("%d",&la); CreatePolyn(&Pa,la);
	printf("建立的多项式Pa为："); PrintPolyn(Pa);

	printf("\n建立多项式Pb...\n""请输入Pb的项数：");
	scanf("%d",&lb); CreatePolyn(&Pb,lb);
	printf("建立的多项式Pb为："); PrintPolyn(Pb);

	printf("\n请选择要对Pa与Pb进行的操作:\n"
	"0:完成多项式相加运算，即：Pa = Pa + Pb，并销毁Pb；\n"
	"1:完成多项式相减运算，即：Pa = Pa - Pb，并销毁Pb；\n"
	"2:完成多项式相乘运算，即：Pa = Pa * Pb，并销毁Pb；\n");
	printf("输入对应操作码："); scanf("%d",&OPT);
	switch(OPT){
		case 0:	AddPolyn(&Pa,&Pb);
				printf("相加后多项式Pa为："); PrintPolyn(Pa); break;
		case 1: SubtractPolyn(&Pa,&Pb);
				printf("相减后多项式Pa为："); PrintPolyn(Pa); break;
		case 2: MultiplyPolyn(&Pa,&Pb);
				printf("相乘后多项式Pa为："); PrintPolyn(Pa); break;
	}
	if(!Pb.head&&!Pb.tail&&!Pb.len) printf("Pb已销毁...\n");
	return 0;
}//多项式的建立/打印/相加/相减/相乘/销毁

