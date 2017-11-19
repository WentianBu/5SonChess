#include "stdafx.h"
#include "judge.h"
typedef struct
{
	int x; // 行
	int y; // 列
}place;



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
	*(Buffer + 589) = "4. 查看规则";
	*(Buffer + 653) = "5. 退出游戏";
	*(Buffer + 775) = "Powered By Wentian Bu    Version 1.0.1";
	for (int i = 6; i < 26; i++) *(Buffer + 800 + i) = "——";
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
			if (CurrentPointer == 6) CurrentPointer = 1;
			else if (CurrentPointer == 0) CurrentPointer = 5;
		}
		else if (key1=='\r') return CurrentPointer;
	}
	
}

void PVP(char **Buffer, char **OldBuffer)
{
	system("cls");
	int Chess[15][15] = { 0 }; // 记录当前棋盘状态，0为空，1为黑方，2为白方
	unsigned int CurrentPlayer = 0; // 当前选手，0为黑方，1为白方
	place CursorPlace; // 当前光标所指位置
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

		if (key1 == ' ' && Chess[CursorPlace.x][CursorPlace.y] == 0)
		{
			if (CurrentPlayer == 0)
			{
				Chess[CursorPlace.x][CursorPlace.y] = 1;
				*(Buffer + 32 * (2 * CursorPlace.x + 1) + (2 * CursorPlace.y + 1)) = "●";
				RefreshScreen(OldBuffer, Buffer);
			}
			else if (CurrentPlayer == 1)
			{
				Chess[CursorPlace.x][CursorPlace.y] = 2;
				*(Buffer + 32 * (2 * CursorPlace.x + 1) + (2 * CursorPlace.y + 1)) = "○";
				RefreshScreen(OldBuffer, Buffer);
			}
			
			Winner = Check(CursorPlace.x, CursorPlace.y, CurrentPlayer,Chess[0]);
			Round++;
			CurrentPlayer = !CurrentPlayer;
		}
		
		else if (key1 == -32)
		{
			key2 = _getch();
			CleanCursor(CursorPlace.x, CursorPlace.y, Buffer);
			switch (key2)
			{
			case 72: CursorPlace.x--; break;
			case 80: CursorPlace.x++; break;
			case 75: CursorPlace.y--; break;
			case 77: CursorPlace.y++; break;
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

void PVE(char **Buffer, char **OldBuffer)
{
	system("cls");
	typedef place(*tFuncpAI_Zhang)(int(*)[15], int, HMODULE); // 针对张宁鑫的旧版API提供支持

	OPENFILENAME DLL = { 0 };
	TCHAR DLLName[MAX_PATH] = { 0 };
	DLL.lStructSize = sizeof(OPENFILENAME);//结构体大小  
	DLL.hwndOwner = NULL;//拥有着窗口句柄，为NULL表示对话框是非模态的，实际应用中一般都要有这个句柄  
	DLL.lpstrFilter = TEXT("AI动态链接库(*.dll)\0*.dll\0所有文件(*.*)\0*.*\0\0");//设置过滤  
	DLL.nFilterIndex = 1;//过滤器索引  
	DLL.lpstrFile = DLLName;//接收返回的文件名，注意第一个字符需要为NULL  
	DLL.nMaxFile = sizeof(DLLName);//缓冲区长度  
	DLL.lpstrInitialDir = NULL;//初始目录为默认  
	DLL.lpstrTitle = TEXT("请选择AI使用的动态链接库");//使用系统默认标题留空即可  
	DLL.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;//文件、目录必须存在，隐藏只读选项  

	HMODULE hDllLib = NULL;
	FARPROC funcpVersion = NULL;
	int FirstTimeOpen = 1; // 首次打开文件的标记
	int UserCancel = 0;
	printf("请选择AI使用的动态链接库...\n");
	do
	{
		if (FirstTimeOpen == 0) printf("无法识别%ls，请重新选择AI模组。\n", DLLName);
		UserCancel = GetOpenFileName(&DLL);
		if (UserCancel == 0) return;
		printf("正在加载%ls...\n", DLLName);
		FirstTimeOpen = 0;
		hDllLib = LoadLibrary(DLLName);
		if (hDllLib != 0) funcpVersion = GetProcAddress(hDllLib, "PrintVersionInfo"); // 输出DLL信息
	} while (funcpVersion == NULL);
	int AI_Type1 = (*funcpVersion)();

	char mode = '0';
	unsigned int Turn; // 当前选手，0为玩家，1为AI
	printf("请选择(F)先手 / (S)后手:\n");
	while (mode!='s'&&mode!='S'&&mode!='f'&&mode !='F')
	{
		rewind(stdin);
		mode = _getch();
	}
	if (mode == 'f' || mode == 'F') Turn = 0;
	else if (mode == 's' || mode == 'S') Turn = 1; 

	// 初始化对局
	system("cls");
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
		if (Turn == 0)
		{
			// 玩家下棋
			char key1, key2;
			rewind(stdin);
			DrawCursor(CursorPlace.x, CursorPlace.y, Buffer);
			RefreshScreen(OldBuffer, Buffer);
			key1 = _getch();
			if (key1 == ' ' && Chess[CursorPlace.x][CursorPlace.y] == 0)
			{
				if (CurrentPlayer == 0)
				{
					Chess[CursorPlace.x][CursorPlace.y] = 1;
					*(Buffer + 32 * (2 * CursorPlace.x + 1) + (2 * CursorPlace.y + 1)) = "●";
					RefreshScreen(OldBuffer, Buffer);
				}
				else if (CurrentPlayer == 1)
				{
					Chess[CursorPlace.x][CursorPlace.y] = 2;
					*(Buffer + 32 * (2 * CursorPlace.x + 1) + (2 * CursorPlace.y + 1)) = "○";
					RefreshScreen(OldBuffer, Buffer);
				}

				Winner = Check(CursorPlace.x, CursorPlace.y, CurrentPlayer, Chess[0]);
				Round++;
				CurrentPlayer = !CurrentPlayer;
				Turn = !Turn; //切换玩家
			}
			else if (key1 == -32)
			{
				key2 = _getch();
				CleanCursor(CursorPlace.x, CursorPlace.y, Buffer);
				switch (key2)
				{
				case 72: CursorPlace.x--; break;
				case 80: CursorPlace.x++; break;
				case 75: CursorPlace.y--; break;
				case 77: CursorPlace.y++; break;
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

			tFuncpAI_Zhang funcpAI_Zhang = (tFuncpAI_Zhang)GetProcAddress(hDllLib, "API_Main");
			AIPlace = (*funcpAI_Zhang)(Chess, CurrentPlayer + 1, hDllLib);
			if (CurrentPlayer == 0)
			{
				Chess[AIPlace.x][AIPlace.y] = 1;
				*(Buffer + 32 * (2 * AIPlace.x + 1) + (2 * AIPlace.y + 1)) = "●";
				CleanCursor(CursorPlace.x, CursorPlace.y, Buffer);
				CursorPlace = AIPlace;
				DrawCursor(CursorPlace.x, CursorPlace.y, Buffer);
				RefreshScreen(OldBuffer, Buffer);
			}
			else if (CurrentPlayer == 1)
			{
				Chess[AIPlace.x][AIPlace.y] = 2;
				*(Buffer + 32 * (2 * AIPlace.x + 1) + (2 * AIPlace.y + 1)) = "○";
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
	if (Winner == 0)
	{
		gotoxy(63, 0);
		printf("黑方获得胜利！");
	}
	else if (Winner == 1)
	{
		gotoxy(63, 0);
		printf("白方获得胜利！");
	}
	else
	{
		gotoxy(63, 0);
		printf("平局！");
	}
	FreeLibrary(hDllLib);
	system("pause");
	return;
}

void EVE(char **Buffer, char **OldBuffer)
{
	system("cls");
	typedef place(*tFuncpAI_Zhang)(int (*)[15], int, HMODULE); // 针对张宁鑫的旧版API提供支持

	OPENFILENAME DLL1 = { 0 }, DLL2 = { 0 };
	TCHAR DLL1Name[MAX_PATH] = { 0 }, DLL2Name[MAX_PATH] = { 0 }; // 接收两个DLL的文件名
	{
		DLL1.lStructSize = sizeof(OPENFILENAME);//结构体大小  
		DLL1.hwndOwner = NULL;//拥有着窗口句柄，为NULL表示对话框是非模态的，实际应用中一般都要有这个句柄  
		DLL1.lpstrFilter = TEXT("AI动态链接库(*.dll)\0*.dll\0所有文件(*.*)\0*.*\0\0");//设置过滤  
		DLL1.nFilterIndex = 1;//过滤器索引  
		DLL1.lpstrFile = DLL1Name;//接收返回的文件名，注意第一个字符需要为NULL  
		DLL1.nMaxFile = sizeof(DLL1Name);//缓冲区长度  
		DLL1.lpstrInitialDir = NULL;//初始目录为默认  
		DLL1.lpstrTitle = TEXT("请选择黑方使用的AI动态链接库");//使用系统默认标题留空即可  
		DLL1.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;//文件、目录必须存在，隐藏只读选项  

		DLL2.lStructSize = sizeof(OPENFILENAME);//结构体大小  
		DLL2.hwndOwner = NULL;//拥有着窗口句柄，为NULL表示对话框是非模态的，实际应用中一般都要有这个句柄  
		DLL2.lpstrFilter = TEXT("AI动态链接库(*.dll)\0*.dll\0所有文件(*.*)\0*.*\0\0");//设置过滤  
		DLL2.nFilterIndex = 1;//过滤器索引  
		DLL2.lpstrFile = DLL2Name;//接收返回的文件名，注意第一个字符需要为NULL  
		DLL2.nMaxFile = sizeof(DLL2Name);//缓冲区长度  
		DLL2.lpstrInitialDir = NULL;//初始目录为默认  
		DLL2.lpstrTitle = TEXT("请选择黑方使用的AI动态链接库");//使用系统默认标题留空即可  
		DLL2.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;//文件、目录必须存在，隐藏只读选项 
	} // 初始化两个 OPENFILENAME 结构体
	printf("请选择黑方使用的AI模组...\n");
	HMODULE hDllLib1 = NULL, hDllLib2 = NULL;
	FARPROC funcpVersion = NULL;
	int FirstTimeOpen = 1; // 首次打开文件的标记
	int UserCancel = 0;
	do
	{
		if (FirstTimeOpen == 0) printf("无法识别%ls，请重新选择AI模组。\n", DLL1Name);
		UserCancel = GetOpenFileName(&DLL1);
		if (UserCancel == 0) return;
		printf("正在加载%ls...\n", DLL1Name);
		FirstTimeOpen = 0;
		hDllLib1 = LoadLibrary(DLL1Name);
		if (hDllLib1 != 0) funcpVersion = GetProcAddress(hDllLib1, "PrintVersionInfo"); // 输出DLL信息
	} while (funcpVersion == NULL);
	int AI_Type1 = (*funcpVersion)();

	printf("\n\n请选择白方使用的AI模组...\n");
	// 初始化加载时所用的变量
	funcpVersion = NULL;
	FirstTimeOpen = 1;
	UserCancel = 0;
	do
	{
		if (FirstTimeOpen == 0) printf("无法识别%ls，请重新选择AI模组。\n", DLL2Name);
		UserCancel = GetOpenFileName(&DLL2);
		if (UserCancel == 0) return;
		printf("正在加载%ls...\n", DLL2Name);
		FirstTimeOpen = 0;
		hDllLib2 = LoadLibrary(DLL2Name);
		if (hDllLib2 != 0) funcpVersion = GetProcAddress(hDllLib2, "PrintVersionInfo"); // 输出DLL信息
	} while (funcpVersion == NULL);
	int AI_Type2 = (*funcpVersion)();


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



			if (CurrentPlayer == 0)
			{
				if (AI_Type1 == 1)
				{
					tFuncpAI_Zhang funcpAI_Zhang = (tFuncpAI_Zhang)GetProcAddress(hDllLib1, "API_Main");
					AIPlace = (*funcpAI_Zhang)(Chess, CurrentPlayer + 1, hDllLib1);
				}
				Chess[AIPlace.x][AIPlace.y] = 1;
				*(Buffer + 32 * (2 * AIPlace.x + 1) + (2 * AIPlace.y + 1)) = "●";
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
					AIPlace = (*funcpAI_Zhang)(Chess, CurrentPlayer + 1, hDllLib2);
				}
				Chess[AIPlace.x][AIPlace.y] = 2;
				*(Buffer + 32 * (2 * AIPlace.x + 1) + (2 * AIPlace.y + 1)) = "○";
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
	FreeLibrary(hDllLib1);
	FreeLibrary(hDllLib2);
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
		case 2: PVE(Buffer[0],OldBuffer[0]); break;
		case 3: EVE(Buffer[0],OldBuffer[0]); break;
		case 4: DisplayRule(); break;
		case 5: exit(0); break;
		default: exit(-1); break;
		}
	}

	return 0;
}