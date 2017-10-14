#pragma once
#include <conio.h>
#include <Windows.h>
#include <stdio.h>
#include <string.h>
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
	gotoxy(63, 31);
}
