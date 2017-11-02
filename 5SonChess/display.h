#pragma once
#include "stdafx.h"
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
	gotoxy(62, 31);
	return;
}

// ���ƿ�����
void DrawBlankChessboard(char **Buffer)
{
	
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			// �������̷���
			*(Buffer + 2 * i * 32 + 2 * j) = "��";
			*(Buffer + 2 * i * 32 + 2 * j + 1) = "��";
			*(Buffer + (2 * i + 1) * 32 + 2 * j) = "��";
			*(Buffer + (2 * i + 1) * 32 + 2 * j + 1) = "��";

			// �������̱߽�ͼ��
			*(Buffer + 2 * i * 32 + 1) = "��";
			*(Buffer + 2 * i * 32 + 29) = "��";
			*(Buffer + (2 * i + 1) * 32 + 1) = "��";
			*(Buffer + (2 * i + 1) * 32 + 29) = "��";
			*(Buffer + 32 + 2 * j) = "��";
			*(Buffer + 928 + 2 * j) = "��";
			*(Buffer + 32 + 2 * j + 1) = "��";
			*(Buffer + 928 + 2 * j + 1) = "��";
		}
	}

	// ���������Ľ�ͼ��
	*(Buffer + 33) = "��";
	*(Buffer + 61) = "��";
	*(Buffer + 929) = "��";
	*(Buffer + 957) = "��";


	// ���̵��ϱߺ����ұ�����
	for (int j = 0; j < 32; j++) *(Buffer + j) = "��";
	for (int i = 0; i < 32; i++)
	{
		*(Buffer + i * 32) = "��";
		*(Buffer + i * 32 + 31) = "��";
	}
	return;
}

// ���ԭ�й��
void CleanCursor(int x, int y, char **Buffer)
{
	*(Buffer + 32 * (2 * y) + (2 * x)) = "��";
	*(Buffer + 32 * (2 * y) + (2 * x + 2)) = "��";
	*(Buffer + 32 * (2 * y + 2) + (2 * x)) = "��";
	*(Buffer + 32 * (2 * y + 2) + (2 * x + 2)) = "��";
	return;
}

// �������̹��
void DrawCursor(int x, int y, char **Buffer)
{
	*(Buffer + 32 * (2 * y) + (2 * x)) = "��";
	*(Buffer + 32 * (2 * y) + (2 * x + 2)) = "��";
	*(Buffer + 32 * (2 * y + 2) + (2 * x)) = "��";
	*(Buffer + 32 * (2 * y + 2) + (2 * x + 2)) = "��";
	return;
}