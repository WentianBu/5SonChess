#pragma once
#include "stdafx.h"
#include "buffer.h"

// 将光标移动到指定位置
// x是横坐标，y是纵坐标
void gotoxy(short x, short y)
{
	COORD coord = { x, y };
	/*
	COORD是Windows API中定义的一种结构，表示一个字符在控制台屏幕上的坐标。其定义为：
	typedef struct _COORD {
	SHORT X; // horizontal coordinate
	SHORT Y; // vertical coordinate
	} COORD;
	*/
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


//无闪烁地刷新屏幕
//Buffer和OldBuffer是缓冲器数组的首地址
//算法说明：通过对比新旧缓冲器中的内容来确定哪些地方发生了更改，然后将光标移动到发生改变的地方覆盖输出
void RefreshScreen(char **OldBuffer, char **Buffer)
{
	//i行，j列
	for (int i=0; i < BUFFER_LINES; i++)
	{
		for (int j = 0; j < BUFFER_WIDTH; j++)
		{
			if (*(Buffer + i * BUFFER_WIDTH + j) != *(OldBuffer + i * BUFFER_WIDTH + j))
			{
				gotoxy(2 * j, i);
				printf("%s", *(Buffer + i * BUFFER_WIDTH + j));
				*(OldBuffer + i * BUFFER_WIDTH + j) = *(Buffer + i * BUFFER_WIDTH + j);
			}
		}
	}
	gotoxy(2 * (BUFFER_WIDTH - 1), BUFFER_LINES - 1);
	return;
}

// 绘制空棋盘
void DrawBlankChessboard(char **Buffer)
{
	
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			// 绘制棋盘方格
			// 说明：该算法绘制的棋盘会在棋盘上边多一行"│"，左边多一列"─"，需要额外清除这部分缓冲器
			*(Buffer + 2 * i * BUFFER_WIDTH + 2 * j + CENTER_OFFSET) = "　";
			*(Buffer + 2 * i * BUFFER_WIDTH + 2 * j +CENTER_OFFSET+1) = "│";
			*(Buffer + (2 * i + 1) * BUFFER_WIDTH + 2 * j + CENTER_OFFSET) = "─";
			*(Buffer + (2 * i + 1) * BUFFER_WIDTH + 2 * j +CENTER_OFFSET+1) = "┼";

			// 绘制棋盘边界图样
			*(Buffer + 2 * i * BUFFER_WIDTH + CENTER_OFFSET + 1) = "┃";
			*(Buffer + 2 * i * BUFFER_WIDTH + CENTER_OFFSET + 29) = "┃";
			*(Buffer + (2 * i + 1) * BUFFER_WIDTH + CENTER_OFFSET + 1) = "┠";
			*(Buffer + (2 * i + 1) * BUFFER_WIDTH + CENTER_OFFSET + 29) = "┨";
			*(Buffer + BUFFER_WIDTH + 2 * j + CENTER_OFFSET) = "━";
			*(Buffer + 29*BUFFER_WIDTH + 2 * j + CENTER_OFFSET) = "━";
			*(Buffer + BUFFER_WIDTH + 2 * j + CENTER_OFFSET + 1) = "┯";
			*(Buffer + 29*BUFFER_WIDTH + 2 * j + CENTER_OFFSET + 1) = "┷";
		}
	}

	// 绘制棋盘四角图样
	*(Buffer + BUFFER_WIDTH*1 + CENTER_OFFSET+1) = "┏";
	*(Buffer + BUFFER_WIDTH*1 + CENTER_OFFSET+29) = "┓";
	*(Buffer + BUFFER_WIDTH*29 + CENTER_OFFSET+1) = "┗";
	*(Buffer + BUFFER_WIDTH*29+CENTER_OFFSET+29) = "┛";

	// 绘制天元和星点
	*(Buffer + BUFFER_WIDTH*15 + CENTER_OFFSET+15) = "╋";
	*(Buffer + BUFFER_WIDTH*7 + CENTER_OFFSET+7) = "╋";
	*(Buffer + BUFFER_WIDTH*7 + CENTER_OFFSET+23) = "╋";
	*(Buffer + BUFFER_WIDTH*23 + CENTER_OFFSET+7) = "╋";
	*(Buffer + BUFFER_WIDTH*23 + CENTER_OFFSET+23) = "╋";
	
	// 棋盘上边留空
	for (int j = 0; j < BUFFER_WIDTH; j++)
		*(Buffer + j + CENTER_OFFSET) = "　";
	// 棋盘左侧留空
	for (int i = 0; i < BUFFER_LINES; i++) // 警告：此处i<BUFFER_LINES而不是BUFFER_WIDTH，否则会产生访问冲突
		*(Buffer + i * BUFFER_WIDTH + CENTER_OFFSET) = "　";
	
	// 绘制坐标
	// 这个做法巨TM Low，有机会改一下
	*(Buffer + (2 * 0 + 1) * BUFFER_WIDTH + CENTER_OFFSET) = "15";
	*(Buffer + (2 * 1 + 1) * BUFFER_WIDTH + CENTER_OFFSET) = "14";
	*(Buffer + (2 * 2 + 1) * BUFFER_WIDTH + CENTER_OFFSET) = "13";
	*(Buffer + (2 * 3 + 1) * BUFFER_WIDTH + CENTER_OFFSET) = "12";
	*(Buffer + (2 * 4 + 1) * BUFFER_WIDTH + CENTER_OFFSET) = "11";
	*(Buffer + (2 * 5 + 1) * BUFFER_WIDTH + CENTER_OFFSET) = "10";
	*(Buffer + (2 * 6 + 1) * BUFFER_WIDTH + CENTER_OFFSET) = " 9";
	*(Buffer + (2 * 7 + 1) * BUFFER_WIDTH + CENTER_OFFSET) = " 8";
	*(Buffer + (2 * 8 + 1) * BUFFER_WIDTH + CENTER_OFFSET) = " 7";
	*(Buffer + (2 * 9 + 1) * BUFFER_WIDTH + CENTER_OFFSET) = " 6";
	*(Buffer + (2 * 10 + 1) * BUFFER_WIDTH + CENTER_OFFSET) = " 5";
	*(Buffer + (2 * 11 + 1) * BUFFER_WIDTH + CENTER_OFFSET) = " 4";
	*(Buffer + (2 * 12 + 1) * BUFFER_WIDTH + CENTER_OFFSET) = " 3";
	*(Buffer + (2 * 13 + 1) * BUFFER_WIDTH + CENTER_OFFSET) = " 2";
	*(Buffer + (2 * 14 + 1) * BUFFER_WIDTH + CENTER_OFFSET) = " 1";

	*(Buffer + 30 * BUFFER_WIDTH + 2 * 0 + 1 + CENTER_OFFSET) = "A";
	*(Buffer + 30 * BUFFER_WIDTH + 2 * 1 + 1 + CENTER_OFFSET) = "B";
	*(Buffer + 30 * BUFFER_WIDTH + 2 * 2 + 1 + CENTER_OFFSET) = "C";
	*(Buffer + 30 * BUFFER_WIDTH + 2 * 3 + 1 + CENTER_OFFSET) = "D";
	*(Buffer + 30 * BUFFER_WIDTH + 2 * 4 + 1 + CENTER_OFFSET) = "E";
	*(Buffer + 30 * BUFFER_WIDTH + 2 * 5 + 1 + CENTER_OFFSET) = "F";
	*(Buffer + 30 * BUFFER_WIDTH + 2 * 6 + 1 + CENTER_OFFSET) = "G";
	*(Buffer + 30 * BUFFER_WIDTH + 2 * 7 + 1 + CENTER_OFFSET) = "H";
	*(Buffer + 30 * BUFFER_WIDTH + 2 * 8 + 1 + CENTER_OFFSET) = "I";
	*(Buffer + 30 * BUFFER_WIDTH + 2 * 9 + 1 + CENTER_OFFSET) = "J";
	*(Buffer + 30 * BUFFER_WIDTH + 2 * 10 + 1 + CENTER_OFFSET) = "K";
	*(Buffer + 30 * BUFFER_WIDTH + 2 * 11 + 1 + CENTER_OFFSET) = "L";
	*(Buffer + 30 * BUFFER_WIDTH + 2 * 12 + 1 + CENTER_OFFSET) = "M";
	*(Buffer + 30 * BUFFER_WIDTH + 2 * 13 + 1 + CENTER_OFFSET) = "N";
	*(Buffer + 30 * BUFFER_WIDTH + 2 * 14 + 1 + CENTER_OFFSET) = "O";

	return;
}

// 清除原有光标
void CleanCursor(int x, int y, char **Buffer)
{
	// x为行，y为列
	*(Buffer + BUFFER_WIDTH * (2 * x) + (2 * y)+CENTER_OFFSET) = "　";
	*(Buffer + BUFFER_WIDTH * (2 * x) + (2 * y + 2)+CENTER_OFFSET) = "　";
	*(Buffer + BUFFER_WIDTH * (2 * x + 2) + (2 * y)+CENTER_OFFSET) = "　";
	*(Buffer + BUFFER_WIDTH * (2 * x + 2) + (2 * y + 2)+CENTER_OFFSET) = "　";
	return;
}

// 绘制棋盘光标
void DrawCursor(int x, int y, char **Buffer)
{
	// x为行，y为列
	*(Buffer + BUFFER_WIDTH * (2 * x) + (2 * y)+CENTER_OFFSET) = "┌";
	*(Buffer + BUFFER_WIDTH * (2 * x) + (2 * y + 2)+CENTER_OFFSET) = "┐";
	*(Buffer + BUFFER_WIDTH * (2 * x + 2) + (2 * y)+CENTER_OFFSET) = "└";
	*(Buffer + BUFFER_WIDTH * (2 * x + 2) + (2 * y + 2)+CENTER_OFFSET) = "┘";
	return;
}