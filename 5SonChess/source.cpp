#include <stdio.h>
#include <Windows.h>
#include "display.h"

int *InitiateGame()
{
	int a[15][15] = { 0 };
	int (*p)[15];
	p = a;
	return p;
}

void SelectFunction();

void BattleWithAI()
{
	system("cls");
}

void LocalMultiPlayerBattle()
{

}

void DisplayHelpInfo()
{

}

void QuitGame()
{
	system("cls");
	printf("ȷ��Ҫ�˳���Ϸ�𣿣�Y/N��");
	char QuitConfirm;
	rewind(stdin);
	scanf_s("%c", &QuitConfirm, 1);
	if (QuitConfirm == 'Y' || QuitConfirm == 'y') exit(0);
	else SelectFunction();
}



void SelectFunction()
{
	printf("\n\n");
	printf("��ӭ���� ������ҫ\n\n");
	printf("A. ��AI��ս\n");
	printf("B. ����˫�˶�ս\n");
	printf("H. �鿴ָ��\n");
	printf("Q. �˳���Ϸ\n\n");
	printf("��ѡ���ܱ�ţ�");
	char Function;
	scanf_s("%c", &Function, 1);
	switch (Function)
	{
	case 'A':BattleWithAI(); break;
	case 'a':BattleWithAI(); break;
	case 'B':LocalMultiPlayerBattle(); break;
	case 'b':LocalMultiPlayerBattle(); break;
	case 'H':DisplayHelpInfo(); break;
	case 'h':DisplayHelpInfo(); break;
	case 'Q':QuitGame(); break;
	case 'q':QuitGame(); break;
	default: {
		printf("��Ч���롣���������롣");
		system("cls");
		SelectFunction();
	}
			 break;
	}
}


int main()
{
	SelectFunction();
}

