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

	// 绘制天元和星点
	*(Buffer + 495) = "╬";
	*(Buffer + 231) = "╬";
	*(Buffer + 247) = "╬";
	*(Buffer + 743) = "╬";
	*(Buffer + 759) = "╬";

	// 棋盘的上边和左右边留空
	for (int j = 0; j < 32; j++) *(Buffer + j) = "　";
	for (int i = 0; i < 32; i++)
	{
		*(Buffer + i * 32) = "　";
		*(Buffer + i * 32 + 31) = "　";
	}
	
	// 绘制坐标

	*(Buffer + (2 * 0 + 1) * 32) = "15";
	*(Buffer + (2 * 1 + 1) * 32) = "14";
	*(Buffer + (2 * 2 + 1) * 32) = "13";
	*(Buffer + (2 * 3 + 1) * 32) = "12";
	*(Buffer + (2 * 4 + 1) * 32) = "11";
	*(Buffer + (2 * 5 + 1) * 32) = "10";
	*(Buffer + (2 * 6 + 1) * 32) = " 9";
	*(Buffer + (2 * 7 + 1) * 32) = " 8";
	*(Buffer + (2 * 8 + 1) * 32) = " 7";
	*(Buffer + (2 * 9 + 1) * 32) = " 6";
	*(Buffer + (2 * 10 + 1) * 32) = " 5";
	*(Buffer + (2 * 11 + 1) * 32) = " 4";
	*(Buffer + (2 * 12 + 1) * 32) = " 3";
	*(Buffer + (2 * 13 + 1) * 32) = " 2";
	*(Buffer + (2 * 14 + 1) * 32) = " 1";

	*(Buffer + 960 + 2 * 0 + 1) = "A";
	*(Buffer + 960 + 2 * 1 + 1) = "B";
	*(Buffer + 960 + 2 * 2 + 1) = "C";
	*(Buffer + 960 + 2 * 3 + 1) = "D";
	*(Buffer + 960 + 2 * 4 + 1) = "E";
	*(Buffer + 960 + 2 * 5 + 1) = "F";
	*(Buffer + 960 + 2 * 6 + 1) = "G";
	*(Buffer + 960 + 2 * 7 + 1) = "H";
	*(Buffer + 960 + 2 * 8 + 1) = "I";
	*(Buffer + 960 + 2 * 9 + 1) = "J";
	*(Buffer + 960 + 2 * 10 + 1) = "K";
	*(Buffer + 960 + 2 * 11 + 1) = "L";
	*(Buffer + 960 + 2 * 12 + 1) = "M";
	*(Buffer + 960 + 2 * 13 + 1) = "N";
	*(Buffer + 960 + 2 * 14 + 1) = "O";



	return;
}

// 清除原有光标
void CleanCursor(int x, int y, char **Buffer)
{
	// x为行，y为列
	*(Buffer + 32 * (2 * x) + (2 * y)) = "　";
	*(Buffer + 32 * (2 * x) + (2 * y + 2)) = "　";
	*(Buffer + 32 * (2 * x + 2) + (2 * y)) = "　";
	*(Buffer + 32 * (2 * x + 2) + (2 * y + 2)) = "　";
	return;
}

// 绘制棋盘光标
void DrawCursor(int x, int y, char **Buffer)
{
	// x为行，y为列
	*(Buffer + 32 * (2 * x) + (2 * y)) = "┌";
	*(Buffer + 32 * (2 * x) + (2 * y + 2)) = "┐";
	*(Buffer + 32 * (2 * x + 2) + (2 * y)) = "└";
	*(Buffer + 32 * (2 * x + 2) + (2 * y + 2)) = "┘";
	return;
}