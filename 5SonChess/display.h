#pragma once
#include <conio.h>
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include "buffer.h"

//初始化缓冲器
void InitiateBuffer(char *Buffer, int pBW)
{
	for (int i = 0; i < 1024; i++) write("　", Buffer, pBW);
	return;
}


// 将光标移动到指定位置
void gotoxy(int x, int y)
{
	COORD coord = { x, y };
	/*COORD是Windows API中定义的一种结构，表示一个字符在控制台屏幕上的坐标。其定义为：

	typedef struct _COORD {

	SHORT X; // horizontal coordinate

	SHORT Y; // vertical coordinate
	} COORD;*/

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// 在控制台的指定位置输出一个字符串
void ShowAtPlace(char *ch, int x, int y)
{
	gotoxy(x, y);
	printf("%s", ch);
	return;
}

// 不闪烁地刷新显示内容
void RefreshScreen(char *Buffer, char *OldBuffer)
{
	for (int i = 0; i < 2048; i=i+2)
	{
		if (*(Buffer+i)!=*(OldBuffer+i)||*(Buffer+i+1)!=*(OldBuffer+i+1))
		{
			i
		}
	} // 逐字比对缓冲器中的内容，并将不同处刷新
	for (int i = 0; i < 2048; i++) *(OldBuffer + i) = *(Buffer + i); // 将当前屏幕内容装入OldBuffer中
}

