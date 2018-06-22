#pragma warning(disable:4996)
#include"Maze_sc.h"

int main()
{
	int i, j, k;
	srand((unsigned)time(NULL));
	Maze ML;
	InletAndOutlet io;
	system("color 3e");
/*	m = Maze_init(2, 2);
	m->mazeNode[findIndex(m, 1, 1)].right_haveWall = no;
	m->mazeNode[findIndex(m, 1, 2)].right_haveWall = no;
	m->mazeNode[findIndex(m, 2, 1)].left_haveWall = no;
	m->mazeNode[findIndex(m, 2, 1)].up_haveWall = no;
	m->mazeNode[findIndex(m, 2, 2)].left_haveWall = no;
	m->mazeNode[findIndex(m, 2, 2)].down_haveWall = no;

	PrintMaze(m);
*/
x:	printf("自动生成并求解迷宫：\n△ ：迷宫的入口\n● ：迷宫的出口\n1：生成迷宫          2：生成并求解迷宫\n你的选择是：");
	scanf("%d%*c", &k);
	if (k == 2) {
		printf("请输入迷宫的尺寸（Row & Column小于等于30）：\n");
		scanf("%d%*c%d", &i, &j);
		while (!(i <= MAX_Row && j <= MAX_Column)) {
			printf("迷宫尺寸过大。请重新输入\n");
			scanf("%d%*c%d", &i, &j);
		}
		ML = createMaze(i, j);
		io = RandInletAndOutlet(ML);
		PrintMaze(ML, io);
		/*	for (i = 1; i <= ML->Column; i++) {
				gotoxy(3, 2 * i);
				printf("●");
			}
			for (i = 1; i <= ML->Row; i++) {
				gotoxy(4 * i - 1, 2);
				printf("△");
			}*/

			//findGotoXY(ML, &ML->mazeNode[findIndex(ML, ML->Row, ML->Column)]);
			//printf("△");



			//printf("%d\n", randToWall(NULL));

		findRoad(ML, io);
		gotoxy(1, 2 * (ML->Column + 1));
		printf("求解完成！\n");
		goto y;
	}
	else if(k == 1){
		printf("请输入迷宫的尺寸（Row & Column小于等于30）：\n");
		scanf("%d%*c%d", &i, &j);
		while (!(i <= MAX_Row && j <= MAX_Column)) {
			printf("迷宫尺寸过大。请重新输入\n");
			scanf("%d%*c%d", &i, &j);
		}
		ML = createMaze(i, j);
		io = RandInletAndOutlet(ML);
		PrintMaze(ML, io);
z:		printf("是否求解该迷宫?(1:Yes\\2:No)\n");
		scanf("%d%*c", &k);
		if (k == 1) {
			findRoad(ML, io);
			gotoxy(1, 2 * (ML->Column + 1) + 2);
			printf("求解完成！\n");
			goto y;
		}
		else if(k == 2)
		{
y:			printf("1：继续，2：退出\n");
			scanf("%d%*c", &k);
			if (k == 1) {
				system("cls");
				goto x;
			}
			else if (k == 2) {
				exit(0);
			}
			else {
				printf("输入错误");
				goto y;
			}

		}
		else {
			printf("输入错误");
			goto z;
		}
	}
	else {
		printf("输入错误");
		Sleep(500);
		system("cls");
		goto x;
	}

	system("pause");
	return 0;
}
