#include "stdafx.h"
#include "operate.h"
#include "LoadAI.h"
#include "record.h"
#include "game.h"

//显示主菜单并且选择功能，将选择的功能编号返回至主函数
int MainMenu(char **Buffer, char **OldBuffer)
{
	CleanScreen(Buffer, OldBuffer);
	for (int i = (BUFFER_WIDTH - 20) / 2; i < (BUFFER_WIDTH + 20) / 2; i++)
		*(Buffer + 5 * BUFFER_WIDTH + i) = "—"; // "—"符号的个数为20个且居中显示在第5行
	*(Buffer + 7 * BUFFER_WIDTH + (BUFFER_WIDTH - 10) / 2) = "欢迎来到 五子荣耀！"; // 显示在第7行并居中
	*(Buffer + 9 * BUFFER_WIDTH + (BUFFER_WIDTH - 13) / 2) = "请使用上下方向键选择功能："; //显示在第9行并居中
	*(Buffer + 12 * BUFFER_WIDTH + (BUFFER_WIDTH - 6) / 2) = "1. 玩家对战"; // 显示在第12行并居中
	*(Buffer + 14 * BUFFER_WIDTH + (BUFFER_WIDTH - 6) / 2) = "2. 与AI对战";
	*(Buffer + 16 * BUFFER_WIDTH + (BUFFER_WIDTH - 6) / 2) = "3. 双AI调试";
	*(Buffer + 18 * BUFFER_WIDTH + (BUFFER_WIDTH - 6) / 2) = "4. 查看规则";
	*(Buffer + 20 * BUFFER_WIDTH + (BUFFER_WIDTH - 6) / 2) = "5. 退出游戏";
	*(Buffer + 24 * BUFFER_WIDTH + (BUFFER_WIDTH - 18) / 2) = "Powered By Wentian Bu  Version 1.2.2";
	for (int i = (BUFFER_WIDTH - 20) / 2; i < (BUFFER_WIDTH + 20) / 2; i++)
		*(Buffer + 25 * BUFFER_WIDTH + i) = "—";
	RefreshScreen(OldBuffer, Buffer);
	return ListChoose(Buffer, OldBuffer, 12, 5, 6, 5, 1);
}

void PVP(char **Buffer, char **OldBuffer)
{
	CleanScreen(Buffer, OldBuffer);
	AIMark Person = { API_PERSON,0 };
	int Winner = GameManager(1, Person, Person, Buffer, OldBuffer);
	gotoxy(2 * CENTER_OFFSET, BUFFER_LINES); //光标移到棋盘下端
	switch (Winner)
	{
	case 0: printf("黑方获得胜利！\n"); break;
	case 1: printf("白方获得胜利！\n"); break;
	case 2: printf("平局！\n"); break;
	case 4: printf("已放弃本局游戏。\n"); break;
	default:
		break;
	}
	FreeHeapMemory(Buffer);
	gotoxy(2 * CENTER_OFFSET, BUFFER_LINES + 1);
	printf("按任意键回到主菜单...");
	_getch();
	return;
}

void PVE(char **Buffer, char **OldBuffer)
{
	CleanScreen(Buffer, OldBuffer);
	gotoxy(0, 0);
	AIMark Person = { API_PERSON,0 };
	AIMark DLL;
	int Winner;
	printf("请选择AI使用的动态链接库...\n");
	DLL.hLib = OpenDLL(&(DLL.AI_Type)); //打开dll文件
	if (DLL.hLib == 0) return;

	char choice = '0';
	unsigned int mode; // 传递给游戏管理器的模式代号
	printf("请选择(F)先手 / (S)后手:\n");
	while (choice != 's'&&choice != 'S'&&choice != 'f'&&choice != 'F')
	{
		rewind(stdin);
		choice = _getch();
	}
	if (choice == 'f' || choice == 'F')
	{
		mode = 2;
		CleanScreen(Buffer, OldBuffer);
		Winner = GameManager(mode, Person, DLL, Buffer, OldBuffer);
	}
	else if (choice == 's' || choice == 'S')
	{
		mode = 3;
		CleanScreen(Buffer, OldBuffer);
		Winner = GameManager(mode, DLL, Person, Buffer, OldBuffer);
	}

	// 单次游戏结束后的处理
	gotoxy(2 * CENTER_OFFSET, BUFFER_LINES);
	switch (Winner)
	{
	case 0: printf("黑方获得胜利！\n"); break;
	case 1: printf("白方获得胜利！\n"); break;
	case 2: printf("平局！\n"); break;
	case 4: printf("已放弃本局游戏。\n"); break;
	default:
		break;
	}
	FreeLibrary(DLL.hLib);
	FreeHeapMemory(Buffer);
	gotoxy(2 * CENTER_OFFSET, BUFFER_LINES + 1);
	printf("按任意键回到主菜单...");
	_getch();
	return;
}

void EVE(char **Buffer, char **OldBuffer)
{
	CleanScreen(Buffer, OldBuffer);
	gotoxy(0, 0);
	AIMark DLL1, DLL2;
	printf("请选择黑方使用的AI模组...\n");
	DLL1.hLib = OpenDLL(&DLL1.AI_Type);
	if (DLL1.hLib == 0) return;
	printf("请选择白方使用的AI模组...\n");
	DLL2.hLib = OpenDLL(&DLL2.AI_Type);
	if (DLL2.hLib == 0) return;

	printf("是否进入统计调试模式？(Y/N)\n");
	char choice = _getch();
	int GameNumber = 0;
	if (choice == 'Y' || choice == 'y')
	{
		printf("请输入测试盘数：");
		rewind(stdin);
		scanf_s("%d", &GameNumber);
		printf("进入统计调试模式...\n");
	}
	else
	{
		GameNumber = 1;
		printf("请选择调试模式：（T）单步调试  （S）慢速调试（间隔1.0s）  （M）中速调试（间隔0.5s）  （F）快速调试（间隔0.2s）  （D）一次性调试（默认）\n");
		rewind(stdin);
		choice = _getch();
	}

	int mode = 8; // 传给游戏管理器的模式代码
	switch (choice)
	{
	case 'T':case 't':mode = 4; break;
	case 'S':case 's':mode = 5; break;
	case 'M':case 'm':mode = 6; break;
	case 'F':case 'f':mode = 7; break;
	default:break;
	}
	char TestAgain = 'N';
	do
	{
		int BlackWin = 0, WhiteWin = 0, NoWin = 0;
		int _GameNumber = GameNumber;

		printf("按任意键开始...\n");
		_getch();
		CleanScreen(Buffer, OldBuffer);
		if (TestAgain == 'Y' || TestAgain == 'y')
		{
			InitiateBuffer(Buffer);
			RefreshScreen(OldBuffer, Buffer);
			DrawBlankChessboard(Buffer);
			RefreshScreen(OldBuffer, Buffer);
		}
		while (_GameNumber > 0)
		{
			rewind(stdin);
			int Winner = GameManager(mode, DLL1, DLL2, Buffer, OldBuffer);
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
			case 2: printf("平局！");
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
		gotoxy(2 * CENTER_OFFSET, BUFFER_LINES + 4);
		printf("再次测试？（Y/N）");
		rewind(stdin);
		TestAgain = _getch();
	} while (TestAgain == 'Y' || TestAgain == 'y');
	FreeLibrary(DLL1.hLib);
	FreeLibrary(DLL2.hLib);
	FreeHeapMemory(Buffer);
	gotoxy(2 * CENTER_OFFSET, BUFFER_LINES + 5);
	printf("按任意键回到主菜单...\n");
	_getch();
	return;
}

void DisplayRule()
{
	ShellExecute(NULL, _T("open"), _T("..\\5SonChess\\rules\\rules.html"), NULL, NULL, SW_SHOW);
	return;
}

int main()
{
	// 获取控制台相关句柄，用于调用控制台API实现各种功能
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hWindow = GetConsoleWindow();

	// 设置窗口大小
	SMALL_RECT WindowAttr = { 0,0,2 * BUFFER_WIDTH - 1,BUFFER_HEIGHT - 1 };
	SetConsoleWindowInfo(hOut, TRUE, &WindowAttr);

	// 设置缓冲区大小
	// 注意：这里两个值都要比窗口大小的值小1才能恰好匹配。
	COORD coord = { 2 * BUFFER_WIDTH,BUFFER_HEIGHT};
	SetConsoleScreenBufferSize(hOut, coord);

	// 禁用调节窗口大小功能
	// 禁用后会出现滚动条，因此同时也禁用了垂直滚动条
	SetWindowLong(hWindow, GWL_STYLE, GetWindowLong(hWindow,GWL_STYLE)&~WS_SIZEBOX&~WS_VSCROLL);

	// 禁用控制台窗口最大化按钮
	DeleteMenu(GetSystemMenu(hWindow, FALSE), SC_MAXIMIZE, MF_BYCOMMAND);
	DrawMenuBar(hWindow);

	// 隐藏控制台闪烁的光标
	CONSOLE_CURSOR_INFO ConsoleCursorInfo;
	GetConsoleCursorInfo(hOut, &ConsoleCursorInfo);
	ConsoleCursorInfo.bVisible = 0;
	SetConsoleCursorInfo(hOut, &ConsoleCursorInfo);
	
	// 设置窗口颜色
	// 背景色灰色（如果添加BACKGROUND_INTENSITY则是亮白色）
	// 前景色黑色
	SetConsoleTextAttribute(hOut, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);

	// 设置窗口标题
	SetConsoleTitle(_T("五子荣耀"));

	//禁用快速编辑模式并允许鼠标操作
	DWORD dwPrevMode = 0;
	GetConsoleMode(hOut, &dwPrevMode);
	SetConsoleMode(hIn, dwPrevMode | ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_MOUSE_INPUT&~ENABLE_QUICK_EDIT_MODE);

	// 开始进入程序
	char *Buffer[BUFFER_HEIGHT][BUFFER_WIDTH] = { NULL };
	char *OldBuffer[BUFFER_HEIGHT][BUFFER_WIDTH] = { NULL };
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
		case 5:
		{
			gotoxy(0, BUFFER_HEIGHT-1);
			exit(0);
		}
		default: exit(-1); break;
		}
	}

	return 0;
}