#pragma once
#include <conio.h>
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include "buffer.h"

// ������ƶ���ָ��λ��
// x�Ǻ����꣬y��������
void gotoxy(int x, int y)
{
	COORD coord = { x, y };
	/*
	COORD��Windows API�ж����һ�ֽṹ����ʾһ���ַ��ڿ���̨��Ļ�ϵ����ꡣ�䶨��Ϊ��
	typedef struct _COORD {
	SHORT X; // horizontal coordinate
	SHORT Y; // vertical coordinate
	} COORD;
	*/
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


//����˸��ˢ����Ļ
void RefreshScreen(char **OldBuffer, char **Buffer)
{
	//i�У�j��
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
