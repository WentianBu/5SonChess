#include "stdafx.h"
#include "keyboard.h"
#include "LoadAI.h"
#include "record.h"


//显示主菜单并且选择功能，将选择的功能编号返回至主函数
int MainMenu(char **Buffer, char **OldBuffer)
{
	system("cls");
	for (int i = (BUFFER_WIDTH - 20) / 2; i < (BUFFER_WIDTH + 20) / 2; i++)
		*(Buffer + 5 * BUFFER_WIDTH + i) = "—"; // "—"符号的个数为20个且居中显示在第5行
	*(Buffer + 7 * BUFFER_WIDTH + (BUFFER_WIDTH - 10) / 2) = "欢迎来到 五子荣耀！"; // 显示在第7行并居中
	*(Buffer + 9 * BUFFER_WIDTH + (BUFFER_WIDTH - 13) / 2) = "请使用上下方向键选择功能："; //显示在第9行并居中
	*(Buffer + 12 * BUFFER_WIDTH + (BUFFER_WIDTH - 6) / 2) = "1. 玩家对战"; // 显示在第12行并居中
	*(Buffer + 14 * BUFFER_WIDTH + (BUFFER_WIDTH - 6) / 2) = "2. 与AI对战";
	*(Buffer + 16 * BUFFER_WIDTH + (BUFFER_WIDTH - 6) / 2) = "3. 双AI调试";
	*(Buffer + 18 * BUFFER_WIDTH + (BUFFER_WIDTH - 6) / 2) = "4. 查看规则";
	*(Buffer + 20 * BUFFER_WIDTH + (BUFFER_WIDTH - 6) / 2) = "5. 退出游戏";
	*(Buffer + 24 * BUFFER_WIDTH + (BUFFER_WIDTH - 19) / 2) = "Powered By Wentian Bu  Version 1.2.0";
	for (int i = (BUFFER_WIDTH - 20) / 2; i < (BUFFER_WIDTH + 20) / 2; i++)
		*(Buffer + 25 * BUFFER_WIDTH + i) = "—";
	RefreshScreen(OldBuffer, Buffer);
	int CurrentPointer = 1;
	while (1)
	{
		rewind(stdin);
		*(Buffer + 10 * BUFFER_WIDTH + 2 * BUFFER_WIDTH * CurrentPointer + (BUFFER_WIDTH - 12) / 2) = "→";
		*(Buffer + 10 * BUFFER_WIDTH + 2 * BUFFER_WIDTH * CurrentPointer + (BUFFER_WIDTH + 10) / 2) = "←";
		RefreshScreen(OldBuffer, Buffer);
		//读取按键
		int key = GetKey();
		switch (key)
		{
		case MOVE_UP:
		{
			*(Buffer + 10 * BUFFER_WIDTH + 2 * BUFFER_WIDTH * CurrentPointer + (BUFFER_WIDTH - 12) / 2) = "  ";
			*(Buffer + 10 * BUFFER_WIDTH + 2 * BUFFER_WIDTH * CurrentPointer + (BUFFER_WIDTH + 10) / 2) = "  ";
			CurrentPointer--;
			break;
		}
		case MOVE_DOWN:
		{
			*(Buffer + 10 * BUFFER_WIDTH + 2 * BUFFER_WIDTH * CurrentPointer + (BUFFER_WIDTH - 12) / 2) = "  ";
			*(Buffer + 10 * BUFFER_WIDTH + 2 * BUFFER_WIDTH * CurrentPointer + (BUFFER_WIDTH + 10) / 2) = "  ";
			CurrentPointer++;
			break;
		}
		case CONFRM: return CurrentPointer;
		default:
			break;
		}
		//按键循环
		if (CurrentPointer == 6) CurrentPointer = 1;
		else if (CurrentPointer == 0) CurrentPointer = 5;
	}
}

void PVP(char **Buffer, char **OldBuffer)
{
	system("cls");
	int Chess[CHESSBORAD_LINES][CHESSBOARD_WIDTH] = { 0 }; // 记录当前棋盘状态，0为空，1为黑方，2为白方
	unsigned int CurrentPlayer = 0; // 当前选手，0为黑方，1为白方
	place CursorPlace; // 当前光标所指位置
	// 创建记录棋局的链表
	ChainNode *pNode = NULL;
	errno_t err = CreateRecordChain(&pNode);
	if (err != 0)
	{
		MessageBox(NULL, _T("Fatal error 0x00001: Fail to create record chain."), _T("Error"), MB_ICONERROR|MB_SETFOREGROUND);
		return;
	} //处理错误，如果出现则中止游戏并弹出消息框警告
	InitiateBuffer(Buffer);
	RefreshScreen(OldBuffer, Buffer);
	DrawBlankChessboard(Buffer);
	RefreshScreen(OldBuffer, Buffer);
	CursorPlace.x = CursorPlace.y = 7; // 光标初始位于棋盘中心点（棋盘坐标0~14）

	// 移动光标和落子
	int Winner = -1, Round = 1;
	while (Winner == -1 && Round <= MAX_ROUND_NUMBER)
	{
		rewind(stdin);
		DrawCursor(CursorPlace.x, CursorPlace.y, Buffer);
		RefreshScreen(OldBuffer, Buffer);
		int key = GetKey();
		if (key == CONFRM && Chess[CursorPlace.x][CursorPlace.y] == 0) // 落子
		{
			if (CurrentPlayer == 0)
			{
				Chess[CursorPlace.x][CursorPlace.y] = 1;
				*(Buffer + BUFFER_WIDTH * (2 * CursorPlace.x + 1) + (2 * CursorPlace.y + 1) + CENTER_OFFSET) = "●";
				RefreshScreen(OldBuffer, Buffer);
			}
			else if (CurrentPlayer == 1)
			{
				Chess[CursorPlace.x][CursorPlace.y] = 2;
				*(Buffer + BUFFER_WIDTH * (2 * CursorPlace.x + 1) + (2 * CursorPlace.y + 1) + CENTER_OFFSET) = "○";
				RefreshScreen(OldBuffer, Buffer);
			}
			
			//记录棋步
			err = RecordStep(&pNode, Round, 0, CurrentPlayer, CursorPlace);
			if (err != 0)
			{
				MessageBox(NULL, _T("Fatal error 0x00002: Fail to record a step."), _T("Error"), MB_ICONERROR|MB_SETFOREGROUND);
				return;
			} //处理错误，如果出现则中止游戏并弹出消息框警告
			Winner = Check(CursorPlace.x, CursorPlace.y, CurrentPlayer, Chess[0]);
			Round++;
			CurrentPlayer = !CurrentPlayer;
		}
		else if (key == REGRET)
		{
			place LastPlace;
			// 处理悔棋操作
			err = RegretStep(&pNode, &LastPlace);
			if (err != 0)
			{
				MessageBox(NULL, _T("都悔到头了，有意思吗？"), _T("无棋可悔"), MB_ICONWARNING|MB_SETFOREGROUND);
			} //处理无棋可悔的情况，如果出现则弹出消息框警告
			if (err == 0)
			{
				Chess[LastPlace.x][LastPlace.y] = 0;
				Round--;//回合数减1
				CurrentPlayer = !CurrentPlayer;
				CleanCursor(CursorPlace.x, CursorPlace.y, Buffer);
				CursorPlace = LastPlace;
				DrawCursor(CursorPlace.x, CursorPlace.y, Buffer);
				RestoreStyle(Buffer, LastPlace);
				RefreshScreen(OldBuffer, Buffer);
			}
		}
		else if (key == EXIT)
		{
			FreeHeapMemory(Buffer);
			return; // 放弃当前游戏
		}
		else if (key >= 1 && key <= 4) // 处理方向键
		{
			CleanCursor(CursorPlace.x, CursorPlace.y, Buffer);
			switch (key)
			{
			case MOVE_UP: CursorPlace.x--; break;
			case MOVE_DOWN: CursorPlace.x++; break;
			case MOVE_LEFT: CursorPlace.y--; break;
			case MOVE_RIGHT: CursorPlace.y++; break;
			default: break;
			}

			// 光标移出棋盘时的循环
			if (CursorPlace.y == -1) CursorPlace.y = CHESSBOARD_WIDTH - 1;
			if (CursorPlace.y == CHESSBOARD_WIDTH) CursorPlace.y = 0;
			if (CursorPlace.x == -1) CursorPlace.x = CHESSBORAD_LINES - 1;
			if (CursorPlace.x == CHESSBORAD_LINES) CursorPlace.x = 0;
		}
	}
	// 单次游戏结束时的处理
	gotoxy(2 * CENTER_OFFSET, BUFFER_LINES); //光标移到棋盘下端
	switch (Winner)
	{
	case 0: printf("黑方获得胜利！\n"); break;
	case 1: printf("白方获得胜利！\n"); break;
	default:printf("平局！\n");
		break;
	}
	gotoxy(2 * CENTER_OFFSET, BUFFER_LINES+1);
	system("pause");
	DeleteRecordChain(&pNode);
	FreeHeapMemory(Buffer);
	return;
}

void PVE(char **Buffer, char **OldBuffer)
{
	system("cls");
	int AI_Type;
	printf("请选择AI使用的动态链接库...\n");
	HMODULE hDllLib = OpenDLL(&AI_Type); //打开dll文件
	if (hDllLib == 0) return;

	char mode = '0';
	unsigned int Turn; // 当前选手，0为玩家，1为AI
	printf("请选择(F)先手 / (S)后手:\n");
	while (mode != 's'&&mode != 'S'&&mode != 'f'&&mode != 'F')
	{
		rewind(stdin);
		mode = _getch();
	}
	if (mode == 'f' || mode == 'F') Turn = 0;
	else if (mode == 's' || mode == 'S') Turn = 1;

	// 初始化对局
	system("cls");
	int Chess[CHESSBORAD_LINES][CHESSBOARD_WIDTH] = { 0 }; // 记录当前棋盘状态，0为空，1为黑方，2为白方
	unsigned int CurrentPlayer = 0; // 当前选手，0为黑方，1为白方
	place CursorPlace; // 当前光标所指位置
	InitiateBuffer(Buffer);
	RefreshScreen(OldBuffer, Buffer);
	DrawBlankChessboard(Buffer);
	RefreshScreen(OldBuffer, Buffer);
	CursorPlace.x = CursorPlace.y = 7; // 光标初始位于棋盘中心点（棋盘坐标0~14）
	int Winner = -1, Round = 1;

	while (Winner == -1 && Round <= MAX_ROUND_NUMBER)
	{
		if (Turn == 0)
		{
			// 玩家下棋
			rewind(stdin);
			DrawCursor(CursorPlace.x, CursorPlace.y, Buffer);
			RefreshScreen(OldBuffer, Buffer);
			int key = GetKey();
			if (key == CONFRM && Chess[CursorPlace.x][CursorPlace.y] == 0)
			{
				if (CurrentPlayer == 0)
				{
					Chess[CursorPlace.x][CursorPlace.y] = 1;
					*(Buffer + BUFFER_WIDTH * (2 * CursorPlace.x + 1) + (2 * CursorPlace.y + 1) + CENTER_OFFSET) = "●";
					RefreshScreen(OldBuffer, Buffer);
				}
				else if (CurrentPlayer == 1)
				{
					Chess[CursorPlace.x][CursorPlace.y] = 2;
					*(Buffer + BUFFER_WIDTH * (2 * CursorPlace.x + 1) + (2 * CursorPlace.y + 1) + CENTER_OFFSET) = "○";
					RefreshScreen(OldBuffer, Buffer);
				}

				Winner = Check(CursorPlace.x, CursorPlace.y, CurrentPlayer, Chess[0]);
				Round++;
				CurrentPlayer = !CurrentPlayer;
				Turn = !Turn; //切换玩家
			}
			else if (key == EXIT)
			{
				FreeHeapMemory(Buffer);
				return;
			}
			else if (key >= 1 && key <= 4)
			{
				CleanCursor(CursorPlace.x, CursorPlace.y, Buffer);
				switch (key)
				{
				case MOVE_UP: CursorPlace.x--; break;
				case MOVE_DOWN: CursorPlace.x++; break;
				case MOVE_LEFT: CursorPlace.y--; break;
				case MOVE_RIGHT: CursorPlace.y++; break;
				default: break;
				}
				// 光标移出棋盘时的循环
				if (CursorPlace.y == -1) CursorPlace.y = 14;
				if (CursorPlace.y == 15) CursorPlace.y = 0;
				if (CursorPlace.x == -1) CursorPlace.x = 14;
				if (CursorPlace.x == 15) CursorPlace.x = 0;
			}

		}
		else if (Turn == 1)
		{
			// AI下棋
			place AIPlace;
			DrawCursor(CursorPlace.x, CursorPlace.y, Buffer);
			RefreshScreen(OldBuffer, Buffer);
			if (AI_Type == 1)
			{
				tFuncpAI_Zhang funcpAI_Zhang = (tFuncpAI_Zhang)GetProcAddress(hDllLib, "API_Main");
				AIPlace = (*funcpAI_Zhang)(Chess, CurrentPlayer + 1);
			}
			if (CurrentPlayer == 0)
			{
				Chess[AIPlace.x][AIPlace.y] = 1;
				*(Buffer + BUFFER_WIDTH * (2 * AIPlace.x + 1) + (2 * AIPlace.y + 1) + CENTER_OFFSET) = "●";
				CleanCursor(CursorPlace.x, CursorPlace.y, Buffer);
				CursorPlace = AIPlace;
				DrawCursor(CursorPlace.x, CursorPlace.y, Buffer);
				RefreshScreen(OldBuffer, Buffer);
			}
			else if (CurrentPlayer == 1)
			{
				Chess[AIPlace.x][AIPlace.y] = 2;
				*(Buffer + BUFFER_WIDTH * (2 * AIPlace.x + 1) + (2 * AIPlace.y + 1) + CENTER_OFFSET) = "○";
				CleanCursor(CursorPlace.x, CursorPlace.y, Buffer);
				CursorPlace = AIPlace;
				DrawCursor(CursorPlace.x, CursorPlace.y, Buffer);
				RefreshScreen(OldBuffer, Buffer);
			}
			Winner = Check(AIPlace.x, AIPlace.y, CurrentPlayer, Chess[0]);
			Round++;
			CurrentPlayer = !CurrentPlayer;
			Turn = !Turn; //切换玩家
		}
	}
	// 单次游戏结束后的处理
	gotoxy(2 * CENTER_OFFSET, BUFFER_LINES);
	switch (Winner)
	{
	case 0: printf("黑方获得胜利！\n"); break;
	case 1: printf("白方获得胜利！\n"); break;
	default:printf("平局！\n");
		break;
	}
	FreeLibrary(hDllLib);
	FreeHeapMemory(Buffer);
	gotoxy(2 * CENTER_OFFSET, BUFFER_LINES+1);
	system("pause");
	return;
}

void EVE(char **Buffer, char **OldBuffer)
{
	system("cls");
	HMODULE hDllLib1 = NULL, hDllLib2 = NULL;
	int AI_Type1, AI_Type2;
	printf("请选择黑方使用的AI模组...\n");
	hDllLib1 = OpenDLL(&AI_Type1);
	if (hDllLib1 == 0) return;
	printf("请选择白方使用的AI模组...\n");
	hDllLib2 = OpenDLL(&AI_Type2);
	if (hDllLib2 == 0) return;

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
		int Chess[CHESSBORAD_LINES][CHESSBOARD_WIDTH] = { 0 }; // 记录当前棋盘状态，0为空，1为黑方，2为白方
		unsigned int CurrentPlayer = 0; // 当前选手，0为黑方，1为白方
		place CursorPlace; // 当前光标所指位置
		// 注：下面这两句关系到统计调试时运行速度和棋盘是否闪烁
		// 若注释掉则可以明显提高运行速度，且棋盘不闪烁，只有棋子快速变化
		//InitiateBuffer(Buffer);
		//RefreshScreen(OldBuffer, Buffer);
		DrawBlankChessboard(Buffer);
		RefreshScreen(OldBuffer, Buffer);
		CursorPlace.x = CursorPlace.y = 7; // 光标初始位于棋盘中心点（棋盘坐标0~14）

		int Winner = -1, Round = 1;
		while (Winner == -1 && Round <= MAX_ROUND_NUMBER)
		{
			place AIPlace;
			DrawCursor(CursorPlace.x, CursorPlace.y, Buffer);
			RefreshScreen(OldBuffer, Buffer);
			if (CurrentPlayer == 0)
			{
				if (AI_Type1 == 1)
				{
					tFuncpAI_Zhang funcpAI_Zhang = (tFuncpAI_Zhang)GetProcAddress(hDllLib1, "API_Main");
					AIPlace = (*funcpAI_Zhang)(Chess, CurrentPlayer + 1);
				}
				Chess[AIPlace.x][AIPlace.y] = 1;
				*(Buffer + BUFFER_WIDTH * (2 * AIPlace.x + 1) + (2 * AIPlace.y + 1) + CENTER_OFFSET) = "●";
				CleanCursor(CursorPlace.x, CursorPlace.y, Buffer);
				CursorPlace = AIPlace;
				DrawCursor(CursorPlace.x, CursorPlace.y, Buffer);
				RefreshScreen(OldBuffer, Buffer);
			}
			else if (CurrentPlayer == 1)
			{
				if (AI_Type2 == 1)
				{
					tFuncpAI_Zhang funcpAI_Zhang = (tFuncpAI_Zhang)GetProcAddress(hDllLib2, "API_Main");
					AIPlace = (*funcpAI_Zhang)(Chess, CurrentPlayer + 1);
				}
				Chess[AIPlace.x][AIPlace.y] = 2;
				*(Buffer + BUFFER_WIDTH * (2 * AIPlace.x + 1) + (2 * AIPlace.y + 1) + CENTER_OFFSET) = "○";
				CleanCursor(CursorPlace.x, CursorPlace.y, Buffer);
				CursorPlace = AIPlace;
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
		// 单局游戏结束后的处理
		gotoxy(2 * CENTER_OFFSET, BUFFER_LINES);
		switch (Winner)
		{
		case 0: printf("黑方获得胜利！");
			BlackWin++;
			break;
		case 1: printf("白方获得胜利！");
			WhiteWin++;
			break;
		default: printf("平局！");
			NoWin++;
			break;
		}
		_GameNumber--;
		gotoxy(2 * CENTER_OFFSET, BUFFER_LINES + 1);
		printf("已进行%d局，剩余%d局", GameNumber - _GameNumber, _GameNumber);
		gotoxy(2 * CENTER_OFFSET, BUFFER_LINES + 2);
		printf("黑胜%d局，白胜%d局，平局%d局", BlackWin, WhiteWin, NoWin);
	}
	gotoxy(2 * CENTER_OFFSET, BUFFER_LINES + 3);
	printf("共%d局，黑胜%d局，白胜%d局，平局%d局。\n", GameNumber, BlackWin, WhiteWin, NoWin);
	FreeLibrary(hDllLib1);
	FreeLibrary(hDllLib2);
	FreeHeapMemory(Buffer);
	gotoxy(2 * CENTER_OFFSET, BUFFER_LINES + 4);
	system("pause");
	return;
}

void DisplayRule()
{
	ShellExecute(NULL, _T("open"), _T("..\\5SonChess\\rules\\rules.html"), NULL, NULL, SW_SHOW);
	return;
}

int main()
{
	// 为system()函数添加可变参数
	char CMD_Args[255];
	sprintf_s(CMD_Args, 255, "mode con cols=%d lines=%d", 2 * BUFFER_WIDTH - 1, BUFFER_LINES + 8);
	system(CMD_Args);
	system("color F0");
	// Buffer和OldBuffer是二维指针数组
	char *Buffer[BUFFER_LINES][BUFFER_WIDTH] = { NULL };
	char *OldBuffer[BUFFER_LINES][BUFFER_WIDTH] = { NULL };
	while (1)
	{
		InitiateBuffer(Buffer[0]); //Buffer[0]是一个一维指针数组的首地址
		RefreshScreen(OldBuffer[0], Buffer[0]);
		int FunctionChoice = MainMenu(Buffer[0], OldBuffer[0]);
		switch (FunctionChoice)
		{
		case 1: PVP(Buffer[0], OldBuffer[0]); break;
		case 2: PVE(Buffer[0], OldBuffer[0]); break;
		case 3: EVE(Buffer[0], OldBuffer[0]); break;
		case 4: DisplayRule(); break;
		case 5: exit(0); break;
		default: exit(-1); break;
		}
	}

	return 0;
}