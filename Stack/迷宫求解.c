
#include <stdio.h>
#include <stdlib.h>		//提供system/rand/srand原型
#include <time.h> 		//提供time原型
#include <unistd.h>		//gcc中需要其提供sleep（秒为单位）原型
#include "status.h"

#define N 15			//迷宫规模为N*N
#define X 5 			//X用于随机数取余，其生成的随机数范围是0到X-1
						//X越大，生成可通行迷宫的可能性越大

typedef enum {		//枚举类型是一种基本数据类型，而不是一种构造类型，每个成员有固定序号0,1,2...
	Wall,				//墙壁
	Obstacle,			//障碍物
	Way,				//路径
	BlindAlley,			//死胡同
	Down,Right,Up,Left	//方向，"↑""↓""←""→"
}MazeNode;				//迷宫节点标记

typedef struct {
	int x;
	int y;
}PosType;	//横纵坐标

typedef struct {
	int		ord;		//通道块在路径上的"序号"
	PosType	seat;		//通道块在迷宫中的"坐标位置"
	int 	dir;		//从此通道块走向下一通道块的"方向"
}SElemType;				//栈的元素类型

#include "SqStack.h"

typedef int MazeType;	//迷宫元素类型

void InitMaze(MazeType maze[N][N],PosType *ent,PosType *exit){
	int i,j,dice;
	srand((unsigned)time(NULL));		//用系统时间做随机数种子
	for(i=0;i<N;++i){
		for(j=0;j<N;++j){
			if(i == 0||i == N-1||j == 0||j == N-1){
				maze[i][j] = Wall;		//建立迷宫外墙
			}
			else{
				dice = rand()%X;		//掷骰子，生成随机数填充迷宫
				if(!dice)
					maze[i][j] = Obstacle;	//1/X几率生成障碍物
				else
					maze[i][j] = Way;		//未掷中则生成路径
			}
		}
	}
	ent->x = 0; ent->y = 1;			//迷宫入口(0,1)
	exit->x = N-1; exit->y = N-2;	//迷宫出口(N-2,N-1)
	maze[0][1] = maze[N-1][N-2] = Way;	//开放入口及出口
	maze[1][1] = maze[N-2][N-2] = Way;	//为提高求解成功率，清理入口及出口前通道块
}//InitMaze

void PrintMaze(MazeType maze[N][N]){
	int i,j;
	for(i=0;i<N;++i){
		for(j=0;j<N;++j){
			if(maze[i][j] == Wall)
				printf("■");		//墙壁
			else if(maze[i][j] == Obstacle)
				printf("▓");		//障碍
			else if(maze[i][j] == Up)
				printf("↑");
			else if(maze[i][j] == Down)
				printf("↓");
			else if(maze[i][j] == Left)
				printf("←");
			else if(maze[i][j] == Right)
				printf("→");
			else printf("  ");		//其他空路径
		}
		printf("\n");		//★每一行末换行
	}
}//PrintMaze

void SetSElemType(SElemType *e,int ord,PosType seat,int dir){
	//把给定的ord,seat,dir设置给指定的数据
	(*e).ord = ord;
	(*e).seat = seat;
	(*e).dir = dir;
}

void FootPrint(MazeType maze[N][N],PosType pos){
	//在一个迷宫中未走过的通道块上留下足迹
	maze[pos.x][pos.y] = Down;		//初始向下探索
}//FootPrint

Status Pass(MazeType maze[N][N],PosType pos){
	//判断迷宫的一个位置是否能通过，若能通过，返回TRUE，否则返回FALSE
	if(maze[pos.x][pos.y] == Way) return TRUE;
	return FALSE;
}//Pass

PosType NextPos(PosType pos,int dir){
	//输出当前位置的某一方向上的下一位置坐标
	PosType next = pos;
	switch(dir){
		case Down: ++next.x; break;
		case Right: ++next.y; break;
		case Up: --next.x; break;
		case Left: --next.y; break;
	}
	return next;
}//NextPos

void MarkPrint(MazeType maze[N][N],PosType pos){
	//将迷宫的当前位置标记为不能通过
	maze[pos.x][pos.y] = BlindAlley;	//置为BlindAlley死胡同
}//MarkPrint

Status EqualPos(PosType pos1,PosType pos2){
	//比较两个位置结构体坐标是否相等，若相等，返回TRUE，否则返回FALSE
	if(pos1.x==pos2.x && pos1.y==pos2.y) return TRUE;
	return FALSE;
}//EqualPos

void ShowMaze(MazeType maze[N][N]){
	//用于进行操作后刷新迷宫
	sleep(1);			//暂停
	system("cls");		//清除屏幕现有内容
	PrintMaze(maze);	//在屏幕上打印迷宫
}//ShowMaze

Status MazePath(MazeType maze[N][N],PosType ent,PosType exit){
	//若迷宫maze中存在从入口ent到出口exit的通道，则求得一条放在栈中（从栈底到栈
	//顶），并返回TRUE；否则返回FALSE
	SqStack S; InitStack(&S);
	PosType curpos = ent;			//设定"当前位置"为"入口位置"
	int curstep = 1;				//探索第一步
	SElemType curelem;				//存储当前通道块信息
	do{
		if(Pass(maze,curpos)){		//当前位置可以通过，即是未曾走到过的通道块
			FootPrint(maze,curpos);	//留下足迹(即为设置路径方向)
			ShowMaze(maze);
			SetSElemType(&curelem,curstep,curpos,Down);	//设置当前通道块信息
			Push(&S,curelem);		//加入路径
			if(EqualPos(curpos,exit)) return TRUE;		//到达出口(判断结构体是否相等必须各个成员分别判断)★
			curpos = NextPos(curpos,Down);				//下一位置是当前位置的下方
			++curstep;				//探索下一步
		}//if
		else{	//当前位置不能通过
			if(!StackEmpty(S)){
				Pop(&S,&curelem);	//暂时取出当前通道块
				while(!StackEmpty(S) && curelem.dir==Left){	//如果四个方向皆不能通过
					MarkPrint(maze,curelem.seat);			//留下不能通过的标记
					ShowMaze(maze);
					Pop(&S,&curelem);	//退回一步
				}//while
				if(curelem.dir < Left){
					maze[curelem.seat.x][curelem.seat.y] = ++curelem.dir;//换下一个方向
					ShowMaze(maze);
					Push(&S,curelem);	//推入栈中
					curpos = NextPos(curelem.seat,curelem.dir);			//设定当前位置是该新方向上的相邻块
				}//if
			}//if
		}//else
	}while(!StackEmpty(S));
	return FALSE;
}//MazePath

int main(){
	MazeType maze[N][N];
	PosType ent,exit;
	InitMaze(maze,&ent,&exit);
	PrintMaze(maze);
	printf("\n");
	if(!MazePath(maze,ent,exit)) printf("\nThe maze has no solution!\n");
	else printf("\nSucceed!");
	return 0;
}//迷宫求解
