#include "stdafx.h"
#include "buffer.h"

//初始化缓冲器，在其中填充空格
void InitiateBuffer(char **p)
{
	for (int i = 0; i < BUFFER_LINES * BUFFER_WIDTH; i++)
	{
		*p = "  ";
		p++;
	}
}

//针对display.h中打印坐标时分配在堆中的内存，在棋盘销毁后（单局游戏退出时）要释放内存
void FreeHeapMemory(char **Buffer)
{
	// 释放纵坐标
	for (int i = 0; i < CHESSBORAD_LINES; i++)
	{
		free(*(Buffer + (2 * i + 1) * BUFFER_WIDTH + CENTER_OFFSET));
		*(Buffer + (2 * i + 1) * BUFFER_WIDTH + CENTER_OFFSET) = NULL;
	}
	// 释放横坐标
	for (int i = 0; i < CHESSBOARD_WIDTH; i++)
	{
		free(*(Buffer + 30 * BUFFER_WIDTH + 2 * i + 1 + CENTER_OFFSET));
		*(Buffer + 30 * BUFFER_WIDTH + 2 * i + 1 + CENTER_OFFSET) = NULL;
	}
	return;
}