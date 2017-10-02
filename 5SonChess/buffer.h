#pragma once
#include <string.h>

char* Copy(char* strDest, const char* strSrc)//修改过的字符串复制函数，会忽略末端的\0
{
	char* strDestCopy = strDest;
	while (*strSrc != '\0') *strDest++ = *strSrc++;
	return strDestCopy;
}

void write(char *c, char *Buffer, int pBW) //向缓冲器的指定位点写入字符串，并将写入位点指针向后移动
{
	Copy(Buffer + pBW, c);
	pBW = pBW + strlen(c);
	if (pBW >= 2048) pBW = 0;
}
