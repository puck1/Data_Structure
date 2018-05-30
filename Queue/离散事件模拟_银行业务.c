
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>				//提供sleep()函数，以秒为单位
#include "status.h"

#define max_durtime 	30		//办理业务所需最长时间
#define max_intertime	5		//客户到来的最长时间间隔

#define open_hour 	8			//开门时刻
#define open_min 	0
#define close_hour 	18			//关门时刻
#define close_min 	0

typedef enum {
	new_arr,leave_1,leave_2,leave_3,leave_4
}EventType;		//事件类型，包括新客户到达以及四个窗口客户离开

typedef struct {
	int hour;
	int minute;
}TimeType;		//时刻类型

typedef struct {
	TimeType	OccurTime;		//事件发生时刻
	EventType 	NType;			//事件类型
}Event,ElemType;				//事件类型，有序链表LinkList的数据元素类型

#include "..\Linear_List\ExtendedLinkList.h"

typedef LinkList EventList;		//事件链表类型，定义为有序链表

typedef struct {
	int Numbering;				//客户编号
	TimeType ArrialTime;		//到达时刻
	int Duration;				//办理事务所需时间
}QElemType;						//队列的数据元素类型

#include "LinkQueue.h"

// - - - - - 程序中用到的主要变量 - - - - -
TimeType 	CurTime; 			//当前时刻
TimeType	CloseTime;			//关门时刻
EventList 	ev;					//事件表
Event 		en;					//事件
LinkQueue 	q[5];				//4个客户队列
QElemType customer;				//客户记录
int TotalTime, CustomerNum;		//累计客户逗留时间，客户数

// - - - - - 增补的基本函数的算法描述 - - - - -
Status OrderInsert(LinkList *L,ElemType e,int (*compare)(TimeType,TimeType)){
	//按时间有序判定函数compare()的约定，将值为e的结点插入到有序链表L的适当位置上
	Link p = L->head->next;
	Link s;
	while(p){
		if((*compare)(p->data.OccurTime,e.OccurTime) > 0){	//寻找比e晚发生的第一个事件
			if(!MakeNode(&s,e)) return ERROR;
			InsBefore(L,&p,s);		//将e插入到该事件之前
			return OK;
		}
		p = p->next;
	}
	if(!MakeNode(&s,e)) return ERROR;
	Append(L,s); return OK;			//如果没有比e晚发生的事件，则插入到末尾
}//OrderInsert

Status DelFirst_L(LinkList *L,Event *e){
	//删除链表L中第一个结点并以q返回
	Link q;
	if(!DelFirst(L->head,&q)) return ERROR;
	if(q == L->tail) L->tail = L->head;		//删除的是尾结点
	--L->len;
	*e = q->data;
	return OK;
}//DelFirst_L

//- - - - - 时间相关操作的算法描述 - - - - -
TimeType SetTime(TimeType time,int min){
	//计算经过若干分钟后的时刻
	TimeType result;
	result.hour = time.hour;
	result.minute = time.minute + min;
	if(result.minute >= 60){	//分钟数溢出，进位
		result.hour += result.minute / 60;
		result.minute %= 60;
	}
	return result;
}//SetTime

TimeType GetFirstTime(EventList e){
	//返回事件链表中第一个事件的发生时刻，链表中必须有事件
	return e.head->next->data.OccurTime;
}//GetFirstTime

int CmpTime(TimeType a,TimeType b){
	//比较两个时刻的先后，并返回-1或0或者1
	int ret;
	if(a.hour < b.hour) ret = -1;
	else if(a.hour > b.hour) ret = 1;
	else if(a.minute < b.minute) ret = -1;
	else if(a.minute > b.minute) ret = 1;
	else ret = 0;
	return ret;
}//cmp

int StayTime(){
	//计算并返回当前顾客的停留时间，以分钟为单位
	return((CurTime.hour - customer.ArrialTime.hour) * 60
			+ CurTime.minute - customer.ArrialTime.minute);
}//StayTime

void TimePass(){
	//时间流逝过程
	if(CurTime.minute == 59){	//进位
		CurTime.minute = 0;
		++CurTime.hour;
	}
	else ++CurTime.minute;
}//TimePass

// - - - - - 模拟相关操作的算法描述 - - - - -
void OpenForDay(){
	//初始化操作，银行开门营业
	int firstinter;
	TotalTime = 0; CustomerNum = 0;				//初始化累计时间和客户数为0
	CurTime.hour = open_hour;
	CurTime.minute = open_min;					//设定开门时间为当前时间
	CloseTime.hour = close_hour;
	CloseTime.minute = close_min; 				//设定关门时间
	InitList(&ev); 								//初始化事件链表为空表
	firstinter = rand() % max_intertime + 1;	//随机获得第一个客户到达的时间间隔
	en.OccurTime = SetTime(CurTime,firstinter);
	en.NType = new_arr;							//设定第一个客户到达事件
	OrderInsert(&ev,en,&CmpTime); 				//插入事件表
	int i;
	for(i = 1; i <= 4; ++i) InitQueue(&q[i]);	//置空队列
}//OpenForDay

Status EventDrived(EventList el){
	//事件驱动，判断当前时刻是否有事件发生
	if(!el.head->next) return FALSE;			//事件表中已无事件
	if(!CmpTime(GetFirstTime(el),CurTime))		//事件在当前时刻发生
		return TRUE;
	return FALSE;
}//EventDrived

void CustomerArrived(){
	//处理客户到达事件，en.NType = new_arr。
	int intertime;
	int i;
	++CustomerNum;
	customer.Duration = rand() % max_durtime + 1;	//当前顾客办理事务用时
	customer.ArrialTime = CurTime;					//当前时刻即为顾客的到达时刻
	customer.Numbering = CustomerNum;				//顾客编号
	i = Minimum(q);									//求长度最短队列
	EnQueue(&q[i],customer);
	intertime = rand() % max_intertime + 1;		//与下一顾客到达时刻的间隔
	en.OccurTime = SetTime(CurTime,intertime);	//下一顾客到达的时刻
	if(CmpTime(en.OccurTime,CloseTime) == -1){	//银行尚未关门，插入事件表
		en.NType = new_arr;
		OrderInsert(&ev,en,&CmpTime);
	}
	if(QueueLength(q[i]) == 1){		//设定第i队列的一个离开时刻并插入事件表
		en.NType =  i;
		en.OccurTime = SetTime(CurTime,customer.Duration);
		OrderInsert(&ev,en,&CmpTime);
	}
}//CustomerArrived

void CustomerDeparture(){
	//处理客户离开事件，en.NType > 0。
	int i = en.NType; DeQueue(&q[i],&customer);		//删除第i队列的排头客户
	TotalTime += StayTime();	//累计客户逗留时间
	if(!QueueEmpty(q[i])){		//设定第i队列的一个离开事件并插入事件表
		GetQHead(q[i],&customer);
		en.NType = i;
		en.OccurTime = SetTime(CurTime,customer.Duration);
		OrderInsert(&ev,en,&CmpTime);
	}
}//CustomerDeparture

void CloseForDay(){
	//关门清算，给出顾客停留的平均时间
	printf("There are %d customers on the day.\n"
			"The average time of staying is %.2f minutes.\n",
			CustomerNum,(float)TotalTime / CustomerNum);	//平均时间转换为float型并保留两位小数
}//CloseForDay

int Minimum(LinkQueue *q){
	//寻找长度最短队列，如果多个队列长度都为最短，返回序号最小的队列号
	int i = 4, j;
	int len1 = QueueLength(q[4]), len2;
	for(j = 3; j > 0; --j){
		len2 = QueueLength(q[j]);
		if(len2 <= len1) {i = j; len1 = len2;}
	}
	return i;
}//Minimum

void ShowBank(){
	//将当前时刻及银行队列情况刷新并打印出来
	int i;
	QueuePtr p;
	system("cls");
	printf("Current time： %02d:%02d\n",CurTime.hour,CurTime.minute);	//%0nd表示n位整数，不足位补0
	for(i = 1; i <= 4; ++i){
		p = q[i].front;
		while(p){
			if(p == q[i].front)
				printf("Counter%d●:",i);
			else
				printf("<%03d>",p->data.Numbering);
			if(p == q[i].rear) printf("\n");	//遇队列末尾换行
			p = p->next;
		}
	}
	sleep(1);
}//Show

void Bank_Simulation(){
	//银行业务模拟，统计一天内客户在银行逗留的平均时间。
	srand((unsigned)time(NULL));	//用系统时间做随机数种子
	OpenForDay();					//初始化
	for(; (CmpTime(CurTime,CloseTime) != 1) || (!ListEmpty(ev)); TimePass()){
									//到达关门时刻后继续处理离开事件
		while(EventDrived(ev)){		//事件驱动
			DelFirst_L(&ev,&en);	//摘下第一个当前时刻发生的事件
			switch(en.NType){
				case new_arr: CustomerArrived(); break;		//处理客户到达事件
				default: CustomerDeparture(); break;		//处理客户离开事件
			}
		}
		ShowBank();
	}
	CloseForDay();		//计算并输出平均逗留时间
}//Bank_Simulation

int main(){
	Bank_Simulation();
	return 0;
}//离散事件模拟_银行业务
