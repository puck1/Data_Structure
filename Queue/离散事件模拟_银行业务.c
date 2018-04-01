
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "status.h"

#define max_durtime 	30		//����ҵ�������ʱ��
#define max_intertime	10		//�ͻ��������ʱ���� 

#define open_hour 	8		//����ʱ�� 
#define open_min 	0
#define close_hour 	18		//����ʱ�� 
#define close_min 	0 

typedef enum {
	new_arr,leave_1,leave_2,leave_3,leave_4
}EventType;		//�¼����ͣ������¿ͻ������Լ��ĸ����ڿͻ��뿪 

typedef struct {
	int hour;
	int minute;
}TimeType;		//ʱ������ 

typedef struct {
	TimeType	OccurTime;		//�¼�����ʱ�� 
	EventType 	NType;			//�¼�����
}Event,ElemType;				//�¼����ͣ���������LinkList������Ԫ������ 

#include "..\Linear_List\ExtendedLinkList.h"

typedef LinkList EventList;		//�¼��������ͣ�����Ϊ��������

typedef struct {
	int Numbering;				//�ͻ���� 
	TimeType ArrialTime;		//����ʱ�� 
	int Duration;				//������������ʱ��
}QElemType;						//���е�����Ԫ������

#include "LinkQueue.h"

// - - - - - �������õ�����Ҫ���� - - - - -  
TimeType 	CurTime; 			//��ǰʱ�� 
TimeType	CloseTime;			//����ʱ�� 
EventList 	ev;					//�¼��� 
Event 		en;					//�¼� 
LinkQueue 	q[5];				//4���ͻ�����
QElemType customer;				//�ͻ���¼
int TotalTime,CustomerNum;		//�ۼƿͻ�����ʱ�䣬�ͻ���

// - - - - - �����Ļ������� - - - - -  
Status OrderInsert(LinkList *L,ElemType e,int (*compare)(TimeType,TimeType)){
	//��ʱ�������ж�����compare()��Լ������ֵΪe�Ľ����뵽��������L���ʵ�λ����
	Link p = L->head->next;
	Link s;
	while(p){
		if((*compare)(p->data.OccurTime,e.OccurTime) > 0){	//Ѱ�ұ�e�����ĵ�һ���¼�
			if(!MakeNode(&s,e)) return ERROR;
			InsBefore(L,&p,s);		//��e���뵽���¼�֮ǰ
			return OK;
		}
		p = p->next;
	}
	if(!MakeNode(&s,e)) return ERROR;
	Append(L,s); return OK;			//���û�б�e�������¼�������뵽ĩβ
}//OrderInsert

Status DelFirst_L(LinkList *L,Event *e){
	//ɾ������L�е�һ����㲢��q����
	Link q;
	if(!DelFirst(L->head,&q)) return ERROR;
	if(q == L->tail) L->tail = L->head;		//ɾ������β��� 
	--L->len;
	*e = q->data;
	return OK;
}//DelFirst_L

//- - - - - ʱ����غ����㷨˵�� - - - - - 
TimeType SetTime(TimeType time,int min){
	//���㾭�����ɷ��Ӻ��ʱ��
	TimeType result;
	result.hour = time.hour;
	result.minute = time.minute + min;
	if(result.minute >= 60){		//�������������λ 
		result.hour += result.minute / 60;
		result.minute %= 60;
	}
	return result;
}//SetTime

TimeType GetFirstTime(EventList e){
	//�����¼������е�һ���¼��ķ���ʱ�̣������б������¼� 
	return e.head->next->data.OccurTime;
}//GetFirstTime

int CmpTime(TimeType a,TimeType b){
	//�Ƚ�����ʱ�̵��Ⱥ󣬲�����-1��0����1
	int ret;
	if(a.hour < b.hour) ret = -1;
	else if(a.hour > b.hour) ret = 1;
	else if(a.minute < b.minute) ret = -1;
	else if(a.minute > b.minute) ret = 1;
	else ret = 0;
	return ret;
}//cmp

int StayTime(){
	//���㲢���ص�ǰ�˿͵�ͣ��ʱ�䣬�Է���Ϊ��λ
	return((CurTime.hour - customer.ArrialTime.hour) * 60 
			+ CurTime.minute - customer.ArrialTime.minute);
}//StayTime

void TimePass(){
	//ʱ�����Ź���
	if(CurTime.minute == 59){		//��λ 
		CurTime.minute = 0;
		++CurTime.hour;
	}
	else ++CurTime.minute;
}//TimePass

void OpenForDay(){
	//��ʼ�����������п���Ӫҵ 
	int firstinter;
	TotalTime = 0; CustomerNum = 0;				//��ʼ���ۼ�ʱ��Ϳͻ���Ϊ0 
	CurTime.hour = open_hour;
	CurTime.minute = open_min;					//�趨����ʱ��Ϊ��ǰʱ��  
	CloseTime.hour = close_hour;
	CloseTime.minute = close_min; 				//�趨����ʱ�� 
	InitList(&ev); 								//��ʼ���¼�����Ϊ�ձ� 
	firstinter = rand() % max_intertime + 1;	//�����õ�һ���ͻ������ʱ���� 
	en.OccurTime = SetTime(CurTime,firstinter); 
	en.NType = new_arr;							//�趨��һ���ͻ������¼� 
	OrderInsert(&ev,en,&CmpTime); 				//�����¼��� 
	int i;
	for(i=1;i<=4;++i) InitQueue(&q[i]);			//�ÿն��� 
}//OpenForDay

Status EventDrived(EventList el){
	//�¼��������жϵ�ǰʱ���Ƿ����¼�����
	if(!el.head->next) return FALSE;			//�¼����������¼� 
	if(!CmpTime(GetFirstTime(el),CurTime))		//�¼��ڵ�ǰʱ�̷��� 
		return TRUE;
	return FALSE;
}//EventDrived

void CustomerArrived(){
	//����ͻ������¼���en.NType = new_arr��
	int intertime; 
	int i;
	++CustomerNum;
	customer.Duration = rand() % max_durtime + 1;	//��ǰ�˿Ͱ���������ʱ 
	customer.ArrialTime = CurTime;					//��ǰʱ�̼�Ϊ�˿͵ĵ���ʱ�� 
	customer.Numbering = CustomerNum;				//�˿ͱ�� 
	i = Minimum(q);									//�󳤶���̶��� 
	EnQueue(&q[i],customer);
	intertime = rand() % max_intertime + 1;		//����һ�˿͵���ʱ�̵ļ��
	en.OccurTime = SetTime(CurTime,intertime);	//��һ�˿͵����ʱ�� 
	if(CmpTime(en.OccurTime,CloseTime) == -1){	//������δ���ţ������¼��� 
		en.NType = new_arr;
		OrderInsert(&ev,en,&CmpTime);
	}
	if(QueueLength(q[i]) == 1){		//�趨��i���е�һ���뿪ʱ�̲������¼��� 
		en.NType =  i;
		en.OccurTime = SetTime(CurTime,customer.Duration);
		OrderInsert(&ev,en,&CmpTime);
	}
}//CustomerArrived

void CustomerDeparture(){
	//����ͻ��뿪�¼���en.NType > 0�� 
	int i = en.NType; DeQueue(&q[i],&customer);		//ɾ����i���е���ͷ�ͻ� 
	TotalTime += StayTime();	//�ۼƿͻ�����ʱ�� 
	if(!QueueEmpty(q[i])){		//�趨��i���е�һ���뿪�¼��������¼��� 
		GetQHead(q[i],&customer);
		en.NType = i;
		en.OccurTime = SetTime(CurTime,customer.Duration);
		OrderInsert(&ev,en,&CmpTime);
	}
}//CustomerDeparture 

void CloseForDay(){
	//�������㣬�����˿�ͣ����ƽ��ʱ�� 
	printf("There are %d customers on the day.\n"
			"The average time of staying is %.2f minutes.\n",
			CustomerNum,(float)TotalTime/CustomerNum);		//ƽ��ʱ��ת��Ϊfloat�Ͳ�������λС�� 
}//CloseForDay

int Minimum(LinkQueue *q){
	//Ѱ�ҳ�����̶��У����������г��ȶ�Ϊ��̣����������С�Ķ��к� 
	int len = QueueLength(q[1]);
	int i=1,j;
	for(j=4;j>=2;--j){
		if(QueueLength(q[j]) < len)
			i = j;
	}
	return i;
}//Minimum

void ShowBank(){
	//����ǰʱ�̼����ж������ˢ�²���ӡ���� 
	int i;
	QueuePtr p;
	system("cls");
	printf("Current time�� %02d:%02d\n",CurTime.hour,CurTime.minute);	//%0nd��ʾnλ����������λ��0 
	for(i=1;i<=4;++i){
		p = q[i].front;
		while(p){
			if(p == q[i].front)
				printf("Counter%d��:",i);
			else
				printf("<%03d>",p->data.Numbering);
			if(p == q[i].rear) printf("\n");	//������ĩβ���� 
			p = p->next;
		}
	}
	sleep(1);
}//Show

void Bank_Simulation(){
	//����ҵ��ģ�⣬ͳ��һ���ڿͻ������ж�����ƽ��ʱ�䡣
	srand((unsigned)time(NULL));	//��ϵͳʱ�������������
	OpenForDay();					//��ʼ��
	for(CurTime;(CmpTime(CurTime,CloseTime)!=1)||(!ListEmpty(ev));TimePass()){
									//�������ʱ�̺���������뿪�¼�
		while(EventDrived(ev)){		//�¼�����
			DelFirst_L(&ev,&en);	//ժ�µ�һ����ǰʱ�̷������¼� 
			switch(en.NType){
				case new_arr: CustomerArrived(); break;		//����ͻ������¼� 
				default: CustomerDeparture(); break;		//����ͻ��뿪�¼� 
			}
		}
		ShowBank();
	}
	CloseForDay();		//���㲢���ƽ������ʱ��
}//Bank_Simulation

int main(){
	Bank_Simulation();
	return 0;
}//��ɢ�¼�ģ��_����ҵ�� 

