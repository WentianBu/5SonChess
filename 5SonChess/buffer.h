#pragma once
#include "macro.h"
//初始化缓冲器，在其中填充空格
void InitiateBuffer(char **p)
{
	for (int i = 0; i < BUFFER_LINES * BUFFER_WIDTH; i++)
	{
		*p = "  ";
		p++;
	}
}

