#pragma once
#include "stdafx.h"
#include "buffer.h"

// 将光标移动到指定位置
// x是横坐标，y是纵坐标
void gotoxy(int x, int y)
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
void RefreshScreen(char **OldBuffer, char **Buffer)
{
	//i行，j列
	for (int i=0; i < 32; i++)
	{
		
		for (int j = 0; j < 32; j++)
		{
			if (*(Buffer + i * 32 + j) != *(OldBuffer + i * 32 + j))
			{
				gotoxy(2*j,i);
				printf("%s", *(Buffer + i * 32 + j));
				*(OldBuffer + i * 32 + j) = *(Buffer + i * 32 + j);
			}
		}
	}
	gotoxy(62, 31);
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
			*(Buffer + 2 * i * 32 + 2 * j) = "　";
			*(Buffer + 2 * i * 32 + 2 * j + 1) = "│";
			*(Buffer + (2 * i + 1) * 32 + 2 * j) = "—";
			*(Buffer + (2 * i + 1) * 32 + 2 * j + 1) = "┼";

			// 绘制棋盘边界图样
			*(Buffer + 2 * i * 32 + 1) = "┃";
			*(Buffer + 2 * i * 32 + 29) = "┃";
			*(Buffer + (2 * i + 1) * 32 + 1) = "┠";
			*(Buffer + (2 * i + 1) * 32 + 29) = "┨";
			*(Buffer + 32 + 2 * j) = "━";
			*(Buffer + 928 + 2 * j) = "━";
			*(Buffer + 32 + 2 * j + 1) = "┯";
			*(Buffer + 928 + 2 * j + 1) = "┷";
		}
	}

	// 绘制棋盘四角图样
	*(Buffer + 33) = "┏";
	*(Buffer + 61) = "┓";
	*(Buffer + 929) = "┗";
	*(Buffer + 957) = "┛";


	// 棋盘的上边和左右边留空
	for (int j = 0; j < 32; j++) *(Buffer + j) = "　";
	for (int i = 0; i < 32; i++)
	{
		*(Buffer + i * 32) = "　";
		*(Buffer + i * 32 + 31) = "　";
	}
	return;
}

// 清除原有光标
void CleanCursor(int x, int y, char **Buffer)
{
	*(Buffer + 32 * (2 * y) + (2 * x)) = "　";
	*(Buffer + 32 * (2 * y) + (2 * x + 2)) = "　";
	*(Buffer + 32 * (2 * y + 2) + (2 * x)) = "　";
	*(Buffer + 32 * (2 * y + 2) + (2 * x + 2)) = "　";
	return;
}

// 绘制棋盘光标
void DrawCursor(int x, int y, char **Buffer)
{
	*(Buffer + 32 * (2 * y) + (2 * x)) = "┌";
	*(Buffer + 32 * (2 * y) + (2 * x + 2)) = "┐";
	*(Buffer + 32 * (2 * y + 2) + (2 * x)) = "└";
	*(Buffer + 32 * (2 * y + 2) + (2 * x + 2)) = "┘";
	return;
}