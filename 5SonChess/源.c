#include "stdafx.h"
#include "judge.h"
#define BOARDMAX 15



//显示主菜单并且选择功能，将选择的功能编号返回至主函数
int MainMenu(char **Buffer, char **OldBuffer)
{
	system("cls");
	for (int i = 6; i < 26; i++) *(Buffer+160+i) = "——";
	*(Buffer + 235) = "欢迎来到 五子荣耀！";
	*(Buffer + 298) = "请使用上下方向键选择功能：";
	*(Buffer + 397) = "1. 玩家对战";
	*(Buffer + 461) = "2. 与AI对战";
	*(Buffer + 525) = "3. 双AI调试";
	*(Buffer + 589) = "4. 退出游戏";
	*(Buffer + 711) = "Powered By Wentian Bu    Version 0.1.1";
	for (int i = 6; i < 26; i++) *(Buffer + 736 + i) = "——";
	RefreshScreen(OldBuffer, Buffer);
	int CurrentPointer = 1;
	while (1)
	{
		rewind(stdin);
		*(Buffer + 331 + 64 * CurrentPointer) = "→";
		*(Buffer + 340 + 64 * CurrentPointer) = "←";
		RefreshScreen(OldBuffer, Buffer);
		char key1, key2;
		key1 = _getch();
		if (key1 == -32)
		{
			*(Buffer + 331 + 64 * CurrentPointer) = "  ";
			*(Buffer + 340 + 64 * CurrentPointer) = "  ";
			key2 = _getch();
			switch (key2)
			{
			case 72: CurrentPointer--; break;
			case 80: CurrentPointer++; break;
			default: break;
			}
			if (CurrentPointer == 5) CurrentPointer = 1;
			else if (CurrentPointer == 0) CurrentPointer = 4;
		}
		else if (key1=='\r') return CurrentPointer;
	}
	
}

void PVP(char **Buffer, char **OldBuffer)
{
	system("cls");
	int Chess[15][15] = { 0 }; // 记录当前棋盘状态，0为空，1为黑方，2为白方
	

	unsigned int CurrentPlayer = 0; // 当前选手，0为黑方，1为白方
	struct place
	{
		int x;
		int y;
	}CursorPlace; // 当前光标所指位置
	InitiateBuffer(Buffer);
	RefreshScreen(OldBuffer, Buffer);
	DrawBlankChessboard(Buffer);
	RefreshScreen(OldBuffer, Buffer);
	CursorPlace.x = CursorPlace.y = 7; // 光标初始位于棋盘中心点（棋盘坐标0~14）

	// 移动光标和落子
	char key1, key2;
	int Winner = -1, Round = 1;
	while (Winner == -1 && Round <= 225)
	{
		rewind(stdin);
		DrawCursor(CursorPlace.x, CursorPlace.y, Buffer);
		RefreshScreen(OldBuffer, Buffer);
		key1 = _getch();

		if (key1 == ' ' && Chess[CursorPlace.y][CursorPlace.x] == 0)
		{
			if (CurrentPlayer == 0)
			{
				Chess[CursorPlace.y][CursorPlace.x] = 1;
				*(Buffer + 32 * (2 * CursorPlace.y + 1) + (2 * CursorPlace.x + 1)) = "●";
				RefreshScreen(OldBuffer, Buffer);
			}
			else if (CurrentPlayer == 1)
			{
				Chess[CursorPlace.y][CursorPlace.x] = 2;
				*(Buffer + 32 * (2 * CursorPlace.y + 1) + (2 * CursorPlace.x + 1)) = "○";
				RefreshScreen(OldBuffer, Buffer);
			}
			
			Winner = Check(CursorPlace.y, CursorPlace.x, CurrentPlayer,Chess[0]);
			Round++;
			CurrentPlayer = !CurrentPlayer;
		}
		
		else if (key1 == -32)
		{
			key2 = _getch();
			CleanCursor(CursorPlace.x, CursorPlace.y, Buffer);
			switch (key2)
			{
			case 72: CursorPlace.y--; break;
			case 80: CursorPlace.y++; break;
			case 75: CursorPlace.x--; break;
			case 77: CursorPlace.x++; break;
			default: break;
			}

			// 光标移出棋盘时的循环
			if (CursorPlace.y == -1) CursorPlace.y = 14;
			if (CursorPlace.y == 15) CursorPlace.y = 0;
			if (CursorPlace.x == -1) CursorPlace.x = 14;
			if (CursorPlace.x == 15) CursorPlace.x = 0;
		}
		
	}
	if (Winner == 0)
	{
		gotoxy(63, 0);
		printf("黑方获得胜利！");
		system("pause");
		return;
	}
	else if (Winner == 1)
	{
		gotoxy(63, 0);
		printf("白方获得胜利！");
		system("pause");
		return;
	}
	else
	{
		gotoxy(63, 0);
		printf("平局！");
		system("pause");
		return;
	}
}

void PVE()
{
	;
}

void EVE(char **Buffer, char **OldBuffer)
{
	system("cls");
	printf("正在加载AI_ZhangNingxin.dll\n");

	typedef struct
	{
		int x;
		int y;
	}place; 
	typedef place(*tFuncpAI_Zhang)(int (*)[15], int, HMODULE);
	HMODULE hDllLib = LoadLibrary(_T("AI_ZhangNingxin.dll"));
	FARPROC funcpVersion = GetProcAddress(hDllLib, "PrintVersionInfo"); // 输出DLL信息
	(*funcpVersion)();
	printf("是否进入统计调试模式？(Y/N)\n");
	char mode = _getch();
	int GameNumber = 0, BlackWin = 0, WhiteWin = 0, NoWin = 0;
	if (mode == 'Y' || mode == 'y')
	{
		printf("请输入测试盘数：");
		scanf_s("%d", &GameNumber);
		printf("进入统计调试模式……\n");
	}
	else
	{
		GameNumber = 1;
		printf("请选择调试模式：（T）单步调试  （S）慢速调试（间隔1.0s）  （M）中速调试（间隔0.5s）  （F）快速调试（间隔0.2s）  （D）一次性调试（默认）\n");
		rewind(stdin);
		mode = _getch();
	}
	int _GameNumber = GameNumber;
	system("pause");
	system("cls");
	while (_GameNumber > 0)
	{
		rewind(stdin);
		
		int Chess[15][15] = { 0 }; // 记录当前棋盘状态，0为空，1为黑方，2为白方


		unsigned int CurrentPlayer = 0; // 当前选手，0为黑方，1为白方

		place CursorPlace; // 当前光标所指位置
		InitiateBuffer(Buffer);
		RefreshScreen(OldBuffer, Buffer);
		DrawBlankChessboard(Buffer);
		RefreshScreen(OldBuffer, Buffer);
		CursorPlace.x = CursorPlace.y = 7; // 光标初始位于棋盘中心点（棋盘坐标0~14）

		int Winner = -1, Round = 1;
		while (Winner == -1 && Round <= 225)
		{
			place AIPlace;
			DrawCursor(CursorPlace.x, CursorPlace.y, Buffer);
			RefreshScreen(OldBuffer, Buffer);

			tFuncpAI_Zhang funcpAI_Zhang = (tFuncpAI_Zhang)GetProcAddress(hDllLib, "API_Main");
			AIPlace = (*funcpAI_Zhang)(Chess, CurrentPlayer + 1, hDllLib);
			if (CurrentPlayer == 0)
			{
				Chess[AIPlace.x][AIPlace.y] = 1;
				*(Buffer + 32 * (2 * AIPlace.x + 1) + (2 * AIPlace.y + 1)) = "●";
				CleanCursor(CursorPlace.x, CursorPlace.y, Buffer);
				CursorPlace.y = AIPlace.x;
				CursorPlace.x = AIPlace.y;
				DrawCursor(CursorPlace.x, CursorPlace.y, Buffer);
				RefreshScreen(OldBuffer, Buffer);
			}
			else if (CurrentPlayer == 1)
			{
				Chess[AIPlace.x][AIPlace.y] = 2;
				*(Buffer + 32 * (2 * AIPlace.x + 1) + (2 * AIPlace.y + 1)) = "○";
				CleanCursor(CursorPlace.x, CursorPlace.y, Buffer);
				CursorPlace.y = AIPlace.x;
				CursorPlace.x = AIPlace.y;
				DrawCursor(CursorPlace.x, CursorPlace.y, Buffer);
				RefreshScreen(OldBuffer, Buffer);
			}

			Winner = Check(AIPlace.x, AIPlace.y, CurrentPlayer, Chess[0]);
			Round++;
			CurrentPlayer = !CurrentPlayer;

			switch (mode)
			{
			case 'T':_getch(); break;
			case 't':_getch(); break;
			case 'S':Sleep(1000); break;
			case 's':Sleep(1000); break;
			case 'M':Sleep(500); break;
			case 'm':Sleep(500); break;
			case 'F':Sleep(200); break;
			case 'f':Sleep(200); break;
			default:break;
			}
		}
		if (Winner == 0)
		{
			gotoxy(63, 0);
			printf("黑方获得胜利！");
			BlackWin++;
		}
		else if (Winner == 1)
		{
			gotoxy(63, 0);
			printf("白方获得胜利！");
			WhiteWin++;
		}
		else
		{
			gotoxy(63, 0);
			printf("平局！");
			NoWin++;
		}
		_GameNumber--;
		printf("\n已进行%d局，剩余%d局\n", GameNumber - _GameNumber, _GameNumber);
		printf("黑胜%d局，白胜%d局，平局%d局", BlackWin, WhiteWin, NoWin);
		
		
		
	}
	printf("\n共%d局，黑胜%d局，白胜%d局，平局%d局。\n", GameNumber, BlackWin, WhiteWin, NoWin);
	FreeLibrary(hDllLib);
	system("pause");
	return;
}

int main()
{
	system("mode con cols=63 lines=40");
	system("color F0");
	char *Buffer[32][32] = { NULL };
	char *OldBuffer[32][32] = { NULL };
	while (1)
	{
		InitiateBuffer(&Buffer[0][0]);
		RefreshScreen(OldBuffer[0], Buffer[0]);
		int FunctionChoice = MainMenu(Buffer[0], OldBuffer[0]);
		switch (FunctionChoice)
		{
		case 1: PVP(Buffer[0],OldBuffer[0]); break;
		case 2: PVE(); break;
		case 3: EVE(Buffer[0],OldBuffer[0]); break;
		case 4: exit(0); break;
		default: exit(-1); break;
		}
	}
	
	/*
	int x = 16, y = 16;
	Buffer[x-1][y-1] = "┏";
	Buffer[x-1][y+1] = "┓";
	Buffer[x+1][y-1] = "┗";
	Buffer[x+1][y+1] = "┛";
	RefreshScreen(OldBuffer[0], Buffer[0]);
	
	
	char key1, key2;
	while (1)
	{
		rewind(stdin);
		key1 = _getch();
		if(key1==-32)
		Buffer[x - 1][y - 1] = "  ";
		Buffer[x - 1][y + 1] = "  ";
		Buffer[x + 1][y - 1] = "  ";
		Buffer[x + 1][y + 1] = "  ";
		key2 = _getch();
		//switch (key1)
		//{
		//case 224:
			switch (key2)
			{
			case 72: x--; break;
			case 80: x++; break;
			case 75: y--; break;
			case 77: y++; break;
			default: break;
			}
			Buffer[x - 1][y - 1] = "┏";
			Buffer[x - 1][y + 1] = "┓";
			Buffer[x + 1][y - 1] = "┗";
			Buffer[x + 1][y + 1] = "┛";
			RefreshScreen(OldBuffer[0], Buffer[0]);
			//break;
		//default: break;
		//}
		
		
		
		switch (key)
		{
		case 'w': y--;
		case 's': y++;
		case 'a': x--;
		case 'd': x++;
		}
		Buffer[x - 1][y - 1] = "┏";
		Buffer[x - 1][y + 1] = "┓";
		Buffer[x + 1][y - 1] = "┗";
		Buffer[x + 1][y + 1] = "┛";
		RefreshScreen(OldBuffer[0], Buffer[0]);
		

	}
	*/
	
	

	
	return 0;
}