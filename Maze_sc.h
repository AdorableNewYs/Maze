#ifndef MAZE_SC_H
#define MAZE_SC_H

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<Windows.h>

//标记是否被访问
#define True  (1)
#define False (0)
#define yes (1)
#define no (0)
//迷宫边界没有墙的表示方式
#define Not_Wall (-1)
//用途暂时为判断节点的墙的方向
#define Left (0)
#define Right (1)
#define Up (2)
#define Down (3)


#define Sleep_Time (50)

#define MAX_Row (30)
#define MAX_Column (30)

typedef char Bool;
//typedef int rand_weight;

struct coordinate
{
	//坐标
	unsigned int x;
	unsigned int y;
	//是否被访问过
	Bool isVisit;
	//四周的墙
	Bool left_haveWall;
	Bool right_haveWall;
	Bool up_haveWall;
	Bool down_haveWall;
	//随机生成权
//	rand_weight weight;
};
//迷宫结构体
struct Mazee {
	struct coordinate *mazeNode;
	//Row和Column都不为0
	unsigned int Row;
	unsigned int Column;
};
//标记迷宫出入口
struct mazeInletAndOutlet {
	int X_start;
	int X_finish;
	int Y_start;
	int Y_finish;
};
//求解迷宫
/*struct solvemaze {
	unsigned int x;
	unsigned int y;
	int direction;
};
*/
typedef struct MazeSt *MazeNodeStack;
typedef struct Mazee *Maze;
typedef struct coordinate *MazeNode;
typedef struct mazeInletAndOutlet *InletAndOutlet;
//typedef struct solvemaze *solveMaze;

//迷宫节点栈
struct MazeSt {
	int Capacity;
	int top;
	MazeNode *mazeNode;
};
/*
//返回坐标X轴的值
unsigned int getCoordinateX(MazeNode n) {
	return n->x;
}

//返回坐标Y
unsigned int getCoordinateY(MazeNode n) {
	return n->y;
}

//返回访问信息
Bool isVisit(MazeNode n) {
	return n->isVisit;
}
*/


//移动控制台光标
void gotoxy(int x, int y)
{
	COORD pos;
	pos.X = x - 1;
	pos.Y = y - 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//迷宫坐标对gotoxy坐标的映射
unsigned int MazeXMapGotoX(Maze m, unsigned int x)
{
	return 4 * x - 1;
}
unsigned int MazeYMapGotoY(Maze m, unsigned int y)
{
	return 2 * (m->Column - y + 1);
}
void findGotoXY(Maze m, MazeNode mN) {
	gotoxy(MazeXMapGotoX(m, mN->x), MazeYMapGotoY(m, mN->y));
}

//初始化迷宫节点栈
MazeNodeStack MazeNodeStack_init(int n)
{
	MazeNodeStack mSt = (MazeNodeStack)malloc(sizeof(struct MazeSt));

	if (mSt) {
		mSt->top = -1;
		mSt->Capacity = n;
		mSt->mazeNode = (MazeNode*)malloc(sizeof(MazeNode) * n);
		if (mSt->mazeNode) {
			return mSt;
		}
		else{
			printf("error:function MazeNodeStack_init()->mSt->mazeNode");
			return NULL;
		}
	}
	else{
		printf("error:function MazeNodeStack_init()->mSt");
		return NULL;
	}
}

//入栈
void Push(MazeNodeStack mSt, MazeNode mN)
{
	if(!isFull(mSt))
		return mSt->mazeNode[++mSt->top] = mN;
	else{
		printf("error Push");
		return NULL;
	}
	   
	
}

//出栈
MazeNode Pop(MazeNodeStack mSt) 
{
	if(!isEmpty(mSt))
	        return mSt->mazeNode[mSt->top--];
	else{
		printf("error Pop");
		return NULL;
	}
}

//判断栈是否为空
Bool isEmpty(MazeNodeStack mSt)
{
	return mSt->top == -1;
}
//判断栈是否满
Bool isFull(MazeNodeStack mSt)
{
	return mSt->top == mSt->Capacity - 1;
}

//初始化迷宫,x和y轴初始值为1
Maze Maze_init(int Row, int Column) {
	if (Row == 0 || Column == 0) {
		printf("error:function Maze_init,Row or Column is zero");
		return NULL;
	}

	int i;
	Maze m = (Maze)malloc(sizeof(struct Mazee));
	MazeNode M = (MazeNode)malloc(sizeof(struct coordinate) *Row * Column);
	if (m) {
		m->Column = Column;
		m->Row = Row;
		if (M) {
			for (i = 0; i < Row * Column; i++) {
				M[i].isVisit = False;
				M[i].x = i % Row + 1;
				M[i].y = i / Row + 1;
				//判断节点是否在边上，如果是就赋给Not_Wall
				if (M[i].y == 1) {
					M[i].down_haveWall = Not_Wall;
					M[i].up_haveWall = yes;
				}
				else if (M[i].y == Column) {
					M[i].up_haveWall = Not_Wall;
					M[i].down_haveWall = yes;
				}
				else {
					M[i].up_haveWall = yes;
					M[i].down_haveWall = yes;
				}
				if (M[i].x == 1) {
					M[i].right_haveWall = yes;
					M[i].left_haveWall = Not_Wall;
				}
				else if (M[i].x == Row) {
					M[i].right_haveWall = Not_Wall;
					M[i].left_haveWall = yes;
				}
				else {
					M[i].left_haveWall = yes;
					M[i].right_haveWall = yes;
				}
			}
		}
		else {
			printf("error:function Maze_init()->M");
		}
		m->mazeNode = M;
	}
	else {
		printf("error:function Maze_init()->m");
	}
	return m;
}

//给定坐标找到节点数组下标，可以证明，若该函数是双射的（满射且单射）则需满足下列条件：
//1 <= x <= Row && 1 <= y <= Column && Row > 1
int findIndex(Maze m,unsigned int x,unsigned int y) 
{
	//Maze[i].x = i % Row + 1;
	//Maze[i].y = i / Row + 1;
	unsigned int i = (y - 1)*m->Row + x - 1;
	//括号里的谓词的功能是将输出结果约束在可控范围
	return (i >= 0 && i < m->Row*m->Column) ? i : False;
}

//删除一个墙(避免存在一种情况：目标节点一个方向的墙打通了，但和其共享该墙的相邻节点还没反应过来)
Bool DeleteWall(Maze m,unsigned int x,unsigned int y, int direction)
{
	if (direction == Left && m->mazeNode[findIndex(m, x, y)].left_haveWall == yes) {
		m->mazeNode[findIndex(m, x, y)].left_haveWall = no;
		m->mazeNode[findIndex(m, x - 1, y)].right_haveWall = no;
	}
	else if (direction == Right && m->mazeNode[findIndex(m, x, y)].right_haveWall == yes) {
		m->mazeNode[findIndex(m, x, y)].right_haveWall = no;
		m->mazeNode[findIndex(m, x + 1, y)].left_haveWall = no;
	}
	else if (direction == Up && m->mazeNode[findIndex(m, x, y)].up_haveWall == yes) {
		m->mazeNode[findIndex(m, x, y)].up_haveWall = no;
		m->mazeNode[findIndex(m, x, y + 1)].down_haveWall = no;
	}
	else if (direction == Down && m->mazeNode[findIndex(m, x, y)].down_haveWall == yes) {
		m->mazeNode[findIndex(m, x, y)].down_haveWall = no;
		m->mazeNode[findIndex(m, x, y - 1)].up_haveWall = no;
	}
	else {
		printf("(%u,%u)->%d the wall not exist\n", x, y, direction);
		return False;
	}
	return True;
}

//如果满分是10分这个函数顶多2分。。。判断whichWall参数有多少位（10进制）
int whichWallNum(int whichWall)
{
	int i = 0;
	do { i++; } while (whichWall /= 10);
	return i;
}

//判断迷宫节点四周是否有墙且墙后节点是否未访问,存在就返回一个十进制数保存墙的信息，example：431 表示 下上左
Bool haveNotVisitNodeWall(Maze m,MazeNode mN)
{
	int i = False;

	if (mN->left_haveWall == yes) {
		if (!m->mazeNode[findIndex(m, mN->x - 1, mN->y)].isVisit) 
			i = i * 10 + Left + 1;
	}
	if (mN->right_haveWall == yes) {
		if (!m->mazeNode[findIndex(m, mN->x + 1, mN->y)].isVisit)
			i = i * 10 + Right + 1;
	}
	if (mN->up_haveWall == yes) {
		if (!m->mazeNode[findIndex(m, mN->x, mN->y + 1)].isVisit)
			i = i * 10 + Up + 1;

	}
	if (mN->down_haveWall == yes) {
		if (!m->mazeNode[findIndex(m, mN->x, mN->y - 1)].isVisit)
			i = i * 10 + Down + 1;
	}
	return i;
}

//与上一个函数相反，找到通路的方向
Bool haveNotVisitRoad(Maze m, MazeNode mN)
{
	int i = False;

	if (mN->left_haveWall == no) {
		if (!m->mazeNode[findIndex(m, mN->x - 1, mN->y)].isVisit)
			i = i * 10 + Left + 1;
	}
	if (mN->right_haveWall == no) {
		if (!m->mazeNode[findIndex(m, mN->x + 1, mN->y)].isVisit)
			i = i * 10 + Right + 1;
	}
	if (mN->up_haveWall == no) {
		if (!m->mazeNode[findIndex(m, mN->x, mN->y + 1)].isVisit)
			i = i * 10 + Up + 1;

	}
	if (mN->down_haveWall == no) {
		if (!m->mazeNode[findIndex(m, mN->x, mN->y - 1)].isVisit)
			i = i * 10 + Down + 1;
	}
	return i;
}

/*随机数对墙方向的映射
int randToWall(int r)
{
	return r == 0 ? Left : (r == 1 ? Right : (r == 2 ? Up : (r == 3 ? Down : Not_Wall)));
}
*/

//随机生成迷宫的出入口
InletAndOutlet RandInletAndOutlet(Maze m)
{
	InletAndOutlet io = (InletAndOutlet)malloc(sizeof(struct mazeInletAndOutlet));
	if (io) {
		if (m->Row == m->Column && m->Row == 1) {
			io->X_start = 1;
			io->Y_start = 1;
			io->X_finish = 1;
			io->Y_finish = 1;
			return io;
		}
		io->X_start = rand() % m->Row + 1;
		while ((io->X_finish = rand() % m->Row + 1) == io->X_start);
		io->Y_start = rand() % m->Column + 1;
		while ((io->Y_finish = rand() % m->Column + 1) == io->Y_start);
		return io;
	}

}

//主要函数
Maze createMaze(unsigned int Row, unsigned int Column)
{
	//迷宫入口选择（1，1）节点
	int x = 1, y = 1;
	int whichWall, whichWall_num, WW[4];//whichWall的4位
	int dir;
	Maze m = Maze_init(Row, Column);
	MazeNodeStack mSt = MazeNodeStack_init(Row*Column);
	MazeNode nowNode = &m->mazeNode[findIndex(m, x, y)];

	//直到栈为空
	do {
		//循环入口处进栈就要保证循环结束时的nowNode->isVisit == False
		Push(mSt, nowNode);
		nowNode->isVisit = True;


		//此处应该有循环，目的是直到当前节点打通一个墙或者回溯(忽略这句话2018.6.21)

//		check = CheckRand;
		dir = rand() % 4;


		//删除墙壁然后把下一个节点入栈
a:		if (dir == Left && nowNode->left_haveWall != Not_Wall) {
			if (!m->mazeNode[findIndex(m, x - 1, y)].isVisit) {
				DeleteWall(m, x, y, Left);
				--x;
			}
			else {
				dir = rand() % 3 + 1;
				goto a;
			}
		}
		else if (dir == Right && nowNode->right_haveWall != Not_Wall) {
			if (!m->mazeNode[findIndex(m, x + 1, y)].isVisit) {
				DeleteWall(m, x, y, Right);
				++x;
			}
			else {
				dir = rand() % 2 + 2;
				goto a;
			}
		}
		else if (dir == Up && nowNode->up_haveWall != Not_Wall) {
			if (!m->mazeNode[findIndex(m, x, y + 1)].isVisit) {
				DeleteWall(m, x, y, Up);
				++y;
			}
			else {
				dir = 3;
				goto a;
			}
		}
		else if (dir == Down && nowNode->down_haveWall != Not_Wall) {
			if (!m->mazeNode[findIndex(m, x, y - 1)].isVisit) {
				DeleteWall(m, x, y, Down);
				--y;
			}
			else {
				//接下来写回溯
				//2018.6.19
b:				do {
					//直到找到可以继续探索的节点为止
					nowNode = Pop(mSt);
				} while (!isEmpty(mSt) && !haveNotVisitNodeWall(m, nowNode));
				if (isEmpty(mSt)) {
					break;
				}
				x = nowNode->x; y = nowNode->y;

			}
		}
		//碰到墙壁
		else {
			if (whichWall = haveNotVisitNodeWall(m, nowNode)) {
				whichWall_num = whichWallNum(whichWall);
				dir = rand() % whichWall_num;
				while (whichWall_num--) {
					WW[whichWall_num] = whichWall % 10;
					whichWall /= 10;
				}
				//随机一个未访问节点
				dir = WW[dir] - 1;
				goto a;
			}
			else {
				goto b;
			}
		}

		//下次循环入栈的是破墙后的节点
		nowNode = &m->mazeNode[findIndex(m, x, y)];

	} while (!isEmpty(mSt));

	return m;
}

//打印迷宫
void PrintMaze(Maze m,InletAndOutlet io) {
	int i, j;
	MazeNode nowNode;
	system("cls");
	for (i = m->Column; i > 0; i--) {
		
		printf("█");
		for (j = 1; j <= m->Row; j++) {
			nowNode = &m->mazeNode[findIndex(m, j, i)];
			if (nowNode->up_haveWall) {
				printf("█");
			}
			else
			{
				printf("  ");
			}
			printf("█");
		}
		printf("\n");
		printf("█");
		for (j = 1; j <= m->Row; j++) {
			nowNode = &m->mazeNode[findIndex(m, j, i)];
			if (j == io->X_start && i == io->Y_start) {
				printf("△");
			}
			else if(j == io->X_finish && i == io->Y_finish)
				printf("●");
			else
				printf("  ");
			if (nowNode->right_haveWall) {
				printf("█");
			}
			else
			{
				printf("  ");
			}
		}
		printf("\n");
	}
	printf("█");
	for (i = 0; i < m->Row; i++) {
		printf("█");
		printf("█");
	}
	printf("\n");
}



//求解迷宫
void findRoad(Maze m ,InletAndOutlet io)
{

	int x = io->X_start, y = io->Y_start, i, whichRoad, whichRoad_num, WW[4], dir;
	//当前节点为入口坐标节点
	MazeNode nowNode = &m->mazeNode[findIndex(m, x, y)];
	//节点栈初始化
	MazeNodeStack mSt = MazeNodeStack_init(m->Column *m->Row);
	//初始化每个节点为未访问
	for (i = 0; i < m->Column *m->Row; i++) {
		m->mazeNode[i].isVisit = 0;
	}
	do {
		Push(mSt, nowNode);
		//⭐符号标记走过的路
		nowNode->isVisit = True;
		//当前节点是否有通路
		if (whichRoad = haveNotVisitRoad(m, nowNode)) {

			whichRoad_num = whichWallNum(whichRoad);

			for (i = 0; i < whichRoad_num; i++) {
				WW[i] = whichRoad % 10;
				whichRoad /= 10;
			}
			//随机选一个通路走
			dir = WW[rand() % whichRoad_num] - 1;

			if (dir == Left) {
				nowNode = &m->mazeNode[findIndex(m, nowNode->x - 1, nowNode->y)];
			}
			else if (dir == Right) {
				nowNode = &m->mazeNode[findIndex(m, nowNode->x + 1, nowNode->y)];
			}
			else if (dir == Up) {
				nowNode = &m->mazeNode[findIndex(m, nowNode->x, nowNode->y + 1)];
			}
			else if (dir == Down) {
				nowNode = &m->mazeNode[findIndex(m, nowNode->x, nowNode->y - 1)];
			}
		}
		else {
			do {
				Sleep(Sleep_Time);
				findGotoXY(m, nowNode);
				printf("  ");
				nowNode = Pop(mSt);
			} while (!haveNotVisitRoad(m, nowNode));
		}

		if (nowNode->x == io->X_finish && nowNode->y == io->Y_finish) {
			
			break;
		}
		else if (nowNode->x == io->X_start && nowNode->y == io->Y_start) {
			Sleep(Sleep_Time);
			findGotoXY(m, nowNode);
			printf("△");
		}
		else{

			Sleep(Sleep_Time);
			findGotoXY(m, nowNode);
			printf("☆");
		}


	} while (1);



}







#endif // !MAZE_SC_H
