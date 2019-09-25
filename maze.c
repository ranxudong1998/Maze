#include "stdafx.h"
#include <iostream>

#include "stdlib.h"

#include<stdio.h>
#include <tchar.h> 
//#include <conio.h>
#include <windows.h>
#include<graphics.h>
//using namespace std;

#define MAX_ARRAY_ROWS  100
#define MAX_ARRAY_COLS  100
#define MAX_STACK_SIZE  10000
#define EXIT_ROW 12					//出口位置（可修改）
#define EXIT_COL 16
const int ROW = 29;
const int COL = 27;

//定义一个足够大的数组来存放各种迷宫，用值true代表墙，用false代表可走方格
bool maze[MAX_ARRAY_ROWS][MAX_ARRAY_COLS];

//走过的路径需要标记，标记记录在mark数组中
bool mark[MAX_ARRAY_ROWS][MAX_ARRAY_ROWS];

//偏移量结构
struct offsets
{
	short int horiz;
	short int vert;
};

//可以前进的方向：东、东南、南、西南、西、西北, 正北、东北
offsets move[8] = { { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 }, { -1, 0 }, { -1, 1 } };

//入栈元素的数据类型，用于记录走过的路径
struct element
{
	short row;   //记录行号
	short col;   //记录列号
	short dir;   //记录方向编号 
};

/*********************与栈相关的代码开始*********************/
typedef element elementtype;

struct stack
{
	elementtype array[MAX_STACK_SIZE];
	int top;
};

void InitStack(stack *pStack);
bool IsStackEmpty(stack *pStack);
bool IsStackFull(stack *pStack);
void Push(stack *pStack, elementtype e);
void Pop(stack *pStack, elementtype *e);
void TraverseStack(stack *pStack);
/*********************与栈相关的代码开始结束*********************/

//定义一个全局的栈
stack mazeStack;

void ChangeMaze(int rows, int cols);	//演示动画
void DrawPath(int rows, int cols);		//绘制行走路线
void Draw();
//主函数，在迷宫搜索路径
void mazepath();


int _tmain(int argc, _TCHAR* argv[])
{
	//迷宫实例，墙用1表示，可走的路用0表示				（迷宫构建，可修改，用cin传入想创建的迷宫）
	char s[] = "010001100011111"
		"100011011100111"
		"011000011110011"
		"110111101111111"
		"001101110100101"
		"001101110100101"
		"011110011111111"
		"001101101111101"
		"110001101100000"
		"001111100011110"
		"010011111011110";
	int rows = 13; //上面的迷宫有11行，为了表示边界，多加两行
	int cols = 17; //上面的迷宫有15行，为了表示边界，多加两列
	int i, j;

	//上下边界设置为墙
	for (j = 0; j < cols; j++)
	{
		maze[0][j] = true;
		maze[rows - 1][j] = true;
	}
	//左右边界设置为墙
	for (i = 0; i < rows; i++)
	{
		maze[i][0] = true;
		maze[i][cols - 1] = true;
	}

	//生成迷宫
	for (i = 1; i < rows - 1; i++)
		for (j = 1; j < cols - 1; j++)
			if (s[(i - 1)*(cols - 2) + j - 1] == '0')
				maze[i][j] = false;
			else
				maze[i][j] = true;

	//初始化栈
	InitStack(&mazeStack);

	//输出初始迷宫，检测初始化是否完成，测试以上代码用
	printf("测试迷宫是：\n");
	for (i = 1; i<rows - 1; i++)
	{
		for (j = 1; j<cols - 1; j++)
			if (maze[i][j])
				printf("%2c", '1');
			else
				printf("%2c", '0');
		printf("\n");
	}

	//测试交替输出
	//ChangeMaze(rows,cols);

	//完成走迷宫
	mazepath();

	return 0;
}

void mazepath()
{
	int row, col, nextRow, nextCol, dir;
	bool found = false;
	element position;

	int r, c;
	printf("请输入你的入口位置：");
	std::cin >> r >> c;
	while (maze[r][c]) {
		printf("此入口不可进，请输入另一个：");
		std::cin >> r >> c;
	}

	printf("此入口可进。\n");
	position.row = r;
	position.col = c;

	//position.row = 1;				//入口位置（可修改）
	//position.col = 1;
	position.dir = 0;				//向东搜寻
	Push(&mazeStack, position);
	while (!IsStackEmpty(&mazeStack) && !found)
	{
		Pop(&mazeStack, &position);
		row = position.row;
		col = position.col;
		dir = position.dir;			//取出栈顶元素
		while (dir < 8 && !found)							//分别判断八个方向
		{
			nextRow = row + move[dir].horiz;				//计算新位置（当前位置加上偏移量：例从maze[3][3]到maze[2][2]的偏移量就是{-1，-1}）
			nextCol = col + move[dir].vert;
			if (nextRow == EXIT_ROW && nextCol == EXIT_COL) //达到目标位置，找到路径  （目标位置可修改）
				found = true;
			else if (!maze[nextRow][nextCol] && !mark[nextRow][nextCol])	//新位置可达（mark表示走过）
			{
				mark[nextRow][nextCol] = true;				//新位置可达，记录走到新位置
				position.row = row;							//当前位置及下一个尝试方向入栈
				position.col = col;
				position.dir = ++dir;						//++dir表示当前这个方向已经走过，下次出栈以后就从下一个方向走
				Push(&mazeStack, position);
				row = nextRow;								//让新的位置成为当前位置
				col = nextCol;
				dir = 0;									//又重新从东开始试各个方向
			}
			else
			{
				//试探新的方向   换方向
				++dir;
			}
		}
	}
	if (found)
	{
		printf("\n 本迷宫存在路径，1秒钟后开始动画演示：\n");
		Sleep(1000);
		TraverseStack(&mazeStack);
	}
	else
		printf("there is no path to exit\n");
}

/*********************与栈相关的代码开始*********************/

//栈相关操作的定义
//栈初始化
void InitStack(stack *pStack)
{
	pStack->top = 0;
}

//判断栈空
bool IsStackEmpty(stack *pStack)
{
	return pStack->top == 0;
}

//判断栈满
bool IsStackFull(stack *pStack)
{
	return pStack->top >= MAX_STACK_SIZE;
}

//入栈
void Push(stack *pStack, elementtype e)
{
	if (!IsStackFull(pStack))
		pStack->array[pStack->top++] = e;
	else
		exit(1);
}

//出栈
void Pop(stack *pStack, elementtype *e)
{
	if (!IsStackEmpty(pStack))
		*e = pStack->array[--pStack->top];
	else
		printf("stack is empty!\n");
}

//遍历栈
void TraverseStack(stack *pStack)
{
	int i = 0;
	//	for (; i < pStack->top; i++)
	//		printf("第%d步：(%d,%d)\n", i, pStack->array[i].row, pStack->array[i].col);
	//	Sleep(500);
	Draw();
	for (; i < pStack->top; i++) {
		//system("cls");
		DrawPath(pStack->array[i].row, pStack->array[i].col);
		//Draw();
		printf("第%d步：(%d,%d)\n", i, pStack->array[i].row, pStack->array[i].col);
		Sleep(200);
	}

}

/*********************与栈相关的代码结束*********************/


/*********************个人添加代码*********************/

void DrawPath(int row, int col)			//此处传递的row col为当前人物位置，迷宫的行列直接写的12和16，仅适用于当前这个测试迷宫
{
	//initgraph(COL * 20, ROW * 20);
	int i, j;
	for (i = 1; i < 12; i++)
	{
		for (j = 1; j < 16; j++)
			if (i == row && j == col){
				//printf("%2c", '$');
				//setfillstyle(SOLID_FILL, RED);
				setfillstyle(BS_SOLID);// 设置填充样式为固实填充图案
				// 设置自定义图案的填充颜色
				setfillcolor(RED);
				bar(100 + j * 15 - 6, 50 + i * 15 - 6, 100 + j * 15 + 6, 50 + i * 15 + 6);
			}

	}
}

void Draw()
{
	initgraph(COL * 20, ROW * 20);
	int i, j;
	for (i = 1; i < 12; i++)
	{
		for (j = 1; j < 16; j++){
			if (maze[i][j]){
				//setfillstyle(SOLID_FILL, BLACK);/*蓝色为墙壁*/
				setfillstyle(BS_SOLID);// 设置填充样式为固实填充图案
				// 设置自定义图案的填充颜色
				setfillcolor(WHITE);
				bar(100 + j * 15 - 6, 50 + i * 15 - 6, 100 + j * 15 + 6, 50 + i * 15 + 6);
			}
			else{
				/*白色为可走的路*/
				setfillstyle(BS_SOLID);// 设置填充样式为固实填充图案
				// 设置自定义图案的填充颜色
				setfillcolor(BLACK);
				bar(100 + j * 15 - 6, 50 + i * 15 - 6, 100 + j * 15 + 6, 50 + i * 15 + 6);
			}
		}
	}
}
/*********************个人添加代码*结束*********************/



