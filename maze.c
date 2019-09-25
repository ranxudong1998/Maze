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
#define EXIT_ROW 12					//����λ�ã����޸ģ�
#define EXIT_COL 16
const int ROW = 29;
const int COL = 27;

//����һ���㹻�����������Ÿ����Թ�����ֵtrue����ǽ����false������߷���
bool maze[MAX_ARRAY_ROWS][MAX_ARRAY_COLS];

//�߹���·����Ҫ��ǣ���Ǽ�¼��mark������
bool mark[MAX_ARRAY_ROWS][MAX_ARRAY_ROWS];

//ƫ�����ṹ
struct offsets
{
	short int horiz;
	short int vert;
};

//����ǰ���ķ��򣺶������ϡ��ϡ����ϡ���������, ����������
offsets move[8] = { { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 }, { -1, 0 }, { -1, 1 } };

//��ջԪ�ص��������ͣ����ڼ�¼�߹���·��
struct element
{
	short row;   //��¼�к�
	short col;   //��¼�к�
	short dir;   //��¼������ 
};

/*********************��ջ��صĴ��뿪ʼ*********************/
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
/*********************��ջ��صĴ��뿪ʼ����*********************/

//����һ��ȫ�ֵ�ջ
stack mazeStack;

void ChangeMaze(int rows, int cols);	//��ʾ����
void DrawPath(int rows, int cols);		//��������·��
void Draw();
//�����������Թ�����·��
void mazepath();


int _tmain(int argc, _TCHAR* argv[])
{
	//�Թ�ʵ����ǽ��1��ʾ�����ߵ�·��0��ʾ				���Թ����������޸ģ���cin�����봴�����Թ���
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
	int rows = 13; //������Թ���11�У�Ϊ�˱�ʾ�߽磬�������
	int cols = 17; //������Թ���15�У�Ϊ�˱�ʾ�߽磬�������
	int i, j;

	//���±߽�����Ϊǽ
	for (j = 0; j < cols; j++)
	{
		maze[0][j] = true;
		maze[rows - 1][j] = true;
	}
	//���ұ߽�����Ϊǽ
	for (i = 0; i < rows; i++)
	{
		maze[i][0] = true;
		maze[i][cols - 1] = true;
	}

	//�����Թ�
	for (i = 1; i < rows - 1; i++)
		for (j = 1; j < cols - 1; j++)
			if (s[(i - 1)*(cols - 2) + j - 1] == '0')
				maze[i][j] = false;
			else
				maze[i][j] = true;

	//��ʼ��ջ
	InitStack(&mazeStack);

	//�����ʼ�Թ�������ʼ���Ƿ���ɣ��������ϴ�����
	printf("�����Թ��ǣ�\n");
	for (i = 1; i<rows - 1; i++)
	{
		for (j = 1; j<cols - 1; j++)
			if (maze[i][j])
				printf("%2c", '1');
			else
				printf("%2c", '0');
		printf("\n");
	}

	//���Խ������
	//ChangeMaze(rows,cols);

	//������Թ�
	mazepath();

	return 0;
}

void mazepath()
{
	int row, col, nextRow, nextCol, dir;
	bool found = false;
	element position;

	int r, c;
	printf("������������λ�ã�");
	std::cin >> r >> c;
	while (maze[r][c]) {
		printf("����ڲ��ɽ�����������һ����");
		std::cin >> r >> c;
	}

	printf("����ڿɽ���\n");
	position.row = r;
	position.col = c;

	//position.row = 1;				//���λ�ã����޸ģ�
	//position.col = 1;
	position.dir = 0;				//����Ѱ
	Push(&mazeStack, position);
	while (!IsStackEmpty(&mazeStack) && !found)
	{
		Pop(&mazeStack, &position);
		row = position.row;
		col = position.col;
		dir = position.dir;			//ȡ��ջ��Ԫ��
		while (dir < 8 && !found)							//�ֱ��жϰ˸�����
		{
			nextRow = row + move[dir].horiz;				//������λ�ã���ǰλ�ü���ƫ����������maze[3][3]��maze[2][2]��ƫ��������{-1��-1}��
			nextCol = col + move[dir].vert;
			if (nextRow == EXIT_ROW && nextCol == EXIT_COL) //�ﵽĿ��λ�ã��ҵ�·��  ��Ŀ��λ�ÿ��޸ģ�
				found = true;
			else if (!maze[nextRow][nextCol] && !mark[nextRow][nextCol])	//��λ�ÿɴmark��ʾ�߹���
			{
				mark[nextRow][nextCol] = true;				//��λ�ÿɴ��¼�ߵ���λ��
				position.row = row;							//��ǰλ�ü���һ�����Է�����ջ
				position.col = col;
				position.dir = ++dir;						//++dir��ʾ��ǰ��������Ѿ��߹����´γ�ջ�Ժ�ʹ���һ��������
				Push(&mazeStack, position);
				row = nextRow;								//���µ�λ�ó�Ϊ��ǰλ��
				col = nextCol;
				dir = 0;									//�����´Ӷ���ʼ�Ը�������
			}
			else
			{
				//��̽�µķ���   ������
				++dir;
			}
		}
	}
	if (found)
	{
		printf("\n ���Թ�����·����1���Ӻ�ʼ������ʾ��\n");
		Sleep(1000);
		TraverseStack(&mazeStack);
	}
	else
		printf("there is no path to exit\n");
}

/*********************��ջ��صĴ��뿪ʼ*********************/

//ջ��ز����Ķ���
//ջ��ʼ��
void InitStack(stack *pStack)
{
	pStack->top = 0;
}

//�ж�ջ��
bool IsStackEmpty(stack *pStack)
{
	return pStack->top == 0;
}

//�ж�ջ��
bool IsStackFull(stack *pStack)
{
	return pStack->top >= MAX_STACK_SIZE;
}

//��ջ
void Push(stack *pStack, elementtype e)
{
	if (!IsStackFull(pStack))
		pStack->array[pStack->top++] = e;
	else
		exit(1);
}

//��ջ
void Pop(stack *pStack, elementtype *e)
{
	if (!IsStackEmpty(pStack))
		*e = pStack->array[--pStack->top];
	else
		printf("stack is empty!\n");
}

//����ջ
void TraverseStack(stack *pStack)
{
	int i = 0;
	//	for (; i < pStack->top; i++)
	//		printf("��%d����(%d,%d)\n", i, pStack->array[i].row, pStack->array[i].col);
	//	Sleep(500);
	Draw();
	for (; i < pStack->top; i++) {
		//system("cls");
		DrawPath(pStack->array[i].row, pStack->array[i].col);
		//Draw();
		printf("��%d����(%d,%d)\n", i, pStack->array[i].row, pStack->array[i].col);
		Sleep(200);
	}

}

/*********************��ջ��صĴ������*********************/


/*********************������Ӵ���*********************/

void DrawPath(int row, int col)			//�˴����ݵ�row colΪ��ǰ����λ�ã��Թ�������ֱ��д��12��16���������ڵ�ǰ��������Թ�
{
	//initgraph(COL * 20, ROW * 20);
	int i, j;
	for (i = 1; i < 12; i++)
	{
		for (j = 1; j < 16; j++)
			if (i == row && j == col){
				//printf("%2c", '$');
				//setfillstyle(SOLID_FILL, RED);
				setfillstyle(BS_SOLID);// ���������ʽΪ��ʵ���ͼ��
				// �����Զ���ͼ���������ɫ
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
				//setfillstyle(SOLID_FILL, BLACK);/*��ɫΪǽ��*/
				setfillstyle(BS_SOLID);// ���������ʽΪ��ʵ���ͼ��
				// �����Զ���ͼ���������ɫ
				setfillcolor(WHITE);
				bar(100 + j * 15 - 6, 50 + i * 15 - 6, 100 + j * 15 + 6, 50 + i * 15 + 6);
			}
			else{
				/*��ɫΪ���ߵ�·*/
				setfillstyle(BS_SOLID);// ���������ʽΪ��ʵ���ͼ��
				// �����Զ���ͼ���������ɫ
				setfillcolor(BLACK);
				bar(100 + j * 15 - 6, 50 + i * 15 - 6, 100 + j * 15 + 6, 50 + i * 15 + 6);
			}
		}
	}
}
/*********************������Ӵ���*����*********************/



