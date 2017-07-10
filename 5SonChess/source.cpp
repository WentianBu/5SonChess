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
	printf("确定要退出游戏吗？（Y/N）");
	char QuitConfirm;
	rewind(stdin);
	scanf_s("%c", &QuitConfirm, 1);
	if (QuitConfirm == 'Y' || QuitConfirm == 'y') exit(0);
	else SelectFunction();
}



void SelectFunction()
{
	printf("\n\n");
	printf("欢迎来到 五子荣耀\n\n");
	printf("A. 与AI对战\n");
	printf("B. 本地双人对战\n");
	printf("H. 查看指南\n");
	printf("Q. 退出游戏\n\n");
	printf("请选择功能编号：");
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
		printf("无效输入。请重新输入。");
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

