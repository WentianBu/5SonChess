#pragma once
#include <conio.h>
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include "buffer.h"

//��ʼ��������
void InitiateBuffer(char *Buffer, int pBW)
{
	for (int i = 0; i < 1024; i++) write("��", Buffer, pBW);
	return;
}


// ������ƶ���ָ��λ��
void gotoxy(int x, int y)
{
	COORD coord = { x, y };
	/*COORD��Windows API�ж����һ�ֽṹ����ʾһ���ַ��ڿ���̨��Ļ�ϵ����ꡣ�䶨��Ϊ��

	typedef struct _COORD {

	SHORT X; // horizontal coordinate

	SHORT Y; // vertical coordinate
	} COORD;*/

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// �ڿ���̨��ָ��λ�����һ���ַ���
void ShowAtPlace(char *ch, int x, int y)
{
	gotoxy(x, y);
	printf("%s", ch);
	return;
}

// ����˸��ˢ����ʾ����
void RefreshScreen(char *Buffer, char *OldBuffer)
{
	for (int i = 0; i < 2048; i=i+2)
	{
		if (*(Buffer+i)!=*(OldBuffer+i)||*(Buffer+i+1)!=*(OldBuffer+i+1))
		{
			i
		}
	} // ���ֱȶԻ������е����ݣ�������ͬ��ˢ��
	for (int i = 0; i < 2048; i++) *(OldBuffer + i) = *(Buffer + i); // ����ǰ��Ļ����װ��OldBuffer��
}

