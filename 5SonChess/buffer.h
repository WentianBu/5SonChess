#pragma once
#include <string.h>

char* Copy(char* strDest, const char* strSrc)//�޸Ĺ����ַ������ƺ����������ĩ�˵�\0
{
	char* strDestCopy = strDest;
	while (*strSrc != '\0') *strDest++ = *strSrc++;
	return strDestCopy;
}

void write(char *c, char *Buffer, int pBW) //�򻺳�����ָ��λ��д���ַ���������д��λ��ָ������ƶ�
{
	Copy(Buffer + pBW, c);
	pBW = pBW + strlen(c);
	if (pBW >= 2048) pBW = 0;
}
