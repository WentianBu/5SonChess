#pragma once


//初始化缓冲器，在其中填充空格
void InitiateBuffer(char **p)
{
	for (int i = 0; i < 1024; i++)
	{
		*p = "蛤";
		p++;
	}
}

