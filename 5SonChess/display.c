#include "stdafx.h"
#include "display.h"

// 将光标移动到指定位置
// x是横坐标，y是纵坐标

//初始化缓冲器，在其中填充空格
void InitiateBuffer(char **p)
{
	for (int i = 0; i < BUFFER_LINES * BUFFER_WIDTH; i++)
	{
		*p = "  ";
		p++;
	}
}


void gotoxy(short x, short y)
{
	COORD coord = { x, y };
	/*
	COORD是Windows API中定义的一种结构，表示一个字符在控制台屏幕上的坐标。其定义为：
	typedef struct _COORD {
	SHORT X; // horizontal coordinate
	SHORT Y; // vertical coordinate
	} COORD;
	*/
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


//无闪烁地刷新屏幕
//Buffer和OldBuffer是缓冲器数组的首地址
//算法说明：通过对比新旧缓冲器中的内容来确定哪些地方发生了更改，然后将光标移动到发生改变的地方覆盖输出
void RefreshScreen(char **OldBuffer, char **Buffer)
{
	//i行，j列
	for (int i = 0; i < BUFFER_LINES; i++)
	{
		for (int j = 0; j < BUFFER_WIDTH; j++)
		{
			if (*(Buffer + i * BUFFER_WIDTH + j) != *(OldBuffer + i * BUFFER_WIDTH + j))
			{
				gotoxy(2 * j, i);
				printf("%s", *(Buffer + i * BUFFER_WIDTH + j));
				*(OldBuffer + i * BUFFER_WIDTH + j) = *(Buffer + i * BUFFER_WIDTH + j);
			}
		}
	}
	gotoxy(2 * (BUFFER_WIDTH - 1), BUFFER_LINES - 1);
	return;
}

// 绘制空棋盘
void DrawBlankChessboard(char **Buffer)
{

	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			// 绘制棋盘方格
			// 说明：该算法绘制的棋盘会在棋盘上边多一行"│"，左边多一列"─"，需要额外清除这部分缓冲器
			*(Buffer + 2 * i * BUFFER_WIDTH + 2 * j + CENTER_OFFSET) = "　";
			*(Buffer + 2 * i * BUFFER_WIDTH + 2 * j + CENTER_OFFSET + 1) = "│";
			*(Buffer + (2 * i + 1) * BUFFER_WIDTH + 2 * j + CENTER_OFFSET) = "─";
			*(Buffer + (2 * i + 1) * BUFFER_WIDTH + 2 * j + CENTER_OFFSET + 1) = "┼";

			// 绘制棋盘边界图样
			*(Buffer + 2 * i * BUFFER_WIDTH + CENTER_OFFSET + 1) = "┃";
			*(Buffer + 2 * i * BUFFER_WIDTH + CENTER_OFFSET + 29) = "┃";
			*(Buffer + (2 * i + 1) * BUFFER_WIDTH + CENTER_OFFSET + 1) = "┠";
			*(Buffer + (2 * i + 1) * BUFFER_WIDTH + CENTER_OFFSET + 29) = "┨";
			*(Buffer + BUFFER_WIDTH + 2 * j + CENTER_OFFSET) = "━";
			*(Buffer + 29 * BUFFER_WIDTH + 2 * j + CENTER_OFFSET) = "━";
			*(Buffer + BUFFER_WIDTH + 2 * j + CENTER_OFFSET + 1) = "┯";
			*(Buffer + 29 * BUFFER_WIDTH + 2 * j + CENTER_OFFSET + 1) = "┷";
		}
	}

	// 绘制棋盘四角图样
	*(Buffer + BUFFER_WIDTH * 1 + CENTER_OFFSET + 1) = "┏";
	*(Buffer + BUFFER_WIDTH * 1 + CENTER_OFFSET + 29) = "┓";
	*(Buffer + BUFFER_WIDTH * 29 + CENTER_OFFSET + 1) = "┗";
	*(Buffer + BUFFER_WIDTH * 29 + CENTER_OFFSET + 29) = "┛";

	// 绘制天元和星点
	*(Buffer + BUFFER_WIDTH * 15 + CENTER_OFFSET + 15) = "╋";
	*(Buffer + BUFFER_WIDTH * 7 + CENTER_OFFSET + 7) = "╋";
	*(Buffer + BUFFER_WIDTH * 7 + CENTER_OFFSET + 23) = "╋";
	*(Buffer + BUFFER_WIDTH * 23 + CENTER_OFFSET + 7) = "╋";
	*(Buffer + BUFFER_WIDTH * 23 + CENTER_OFFSET + 23) = "╋";

	// 棋盘上边留空
	for (int j = 0; j < BUFFER_WIDTH; j++)
		*(Buffer + j + CENTER_OFFSET) = "　";
	// 棋盘左侧留空
	for (int i = 0; i < BUFFER_LINES; i++) // 警告：此处i<BUFFER_LINES而不是BUFFER_WIDTH，否则会产生访问冲突
		*(Buffer + i * BUFFER_WIDTH + CENTER_OFFSET) = "　";

	// 特别注意！
	// 使用这个malloc的方法来循环打印坐标，需要在棋盘销毁时（单局游戏退出后）释放内存
	// 针对这些存储坐标的缓冲器的内存释放函数定义在 buffer.c 中
	// 绘制纵坐标
	for (int i = 0; i < CHESSBORAD_LINES; i++)
	{
		*(Buffer + (2 * i + 1) * BUFFER_WIDTH + CENTER_OFFSET) = (char *)malloc(3 * sizeof(char));
		if (*(Buffer + (2 * i + 1) * BUFFER_WIDTH + CENTER_OFFSET) == NULL) exit(-2);
		sprintf_s(*(Buffer + (2 * i + 1) * BUFFER_WIDTH + CENTER_OFFSET), 3, "%2d", 15 - i);
	}
	// 绘制横坐标
	for (int i = 0; i < CHESSBOARD_WIDTH; i++)
	{
		*(Buffer + 30 * BUFFER_WIDTH + 2 * i + 1 + CENTER_OFFSET) = (char *)malloc(2 * sizeof(char));
		if (*(Buffer + 30 * BUFFER_WIDTH + 2 * i + 1 + CENTER_OFFSET) == NULL) exit(-2);
		(*(Buffer + 30 * BUFFER_WIDTH + 2 * i + 1 + CENTER_OFFSET))[0] = 'A' + i;
		(*(Buffer + 30 * BUFFER_WIDTH + 2 * i + 1 + CENTER_OFFSET))[1] = '\0';
	}

	/* 原来的对每个缓冲器单独赋值的做法太Low，希望采用循环语句进行赋值。
	 * 最开始采用方法1所述的方法，但是运行时不能正常显示坐标，反而全部显示"烫烫烫"。
	 * 调试时发现每次字符串都已经正常赋给了对应的缓冲器指针，但是最后输出不对。
	*/
	// 找到问题的缘由！
	/*
	此处几种解决方式都不好使，原因在于：
	*(Buffer + 30 * BUFFER_WIDTH + 2 * i + 1 + CENTER_OFFSET)是char *型的指针变量，下文简称指针p
	方法1：定义数组char str[2]，每次更改str[0]的值，在str[1]填充'\0'，并每次将指针p指向该数组首地址，即：p=str;
	这种方法的问题在于，p指向了str[]的首地址，但str[]是自动变量，在这个函数退出后就会被释放，因此在打印时p已经指向了无效的内存区域，出现 烫烫烫 也是自然的了。
	方法2：定义数组static char str[2]，其他同上。
	这种方法不会烫烫烫，但是由于数组str[]最后的值是"O\0"，故下边所有的坐标都是O。
	方法3：使用strcpy()函数，每次将str[]的值复制到指针p指向的内存区域。
	这种方法的问题在于，p最开始初始化指向的是常量区的"  "，没有分配内存空间，因此这样直接copy会报错：写入时发生访问权限冲突。
	*/
	/*
	static char str[2];
	str[1] = '\0';
	for (int i = 0; i < 15; i++)
	{
	str[0] = 'A' + i;
	*(Buffer + 30 * BUFFER_WIDTH + 2 * i + 1 + CENTER_OFFSET) = str;
	}
	*/
	/*
	char str[2];
	str[1] = '\0';
	for (int i = 0; i < 15; i++)
	{
	str[0] = 'A' + i;
	strcpy_s(*(Buffer + 30 * BUFFER_WIDTH + 2 * i + 1 + CENTER_OFFSET), 2, str);
	}
	*/
	return;
}

// 清除原有光标
void CleanCursor(place CursorPlace, char **Buffer)
{
	// x为行，y为列
	*(Buffer + BUFFER_WIDTH * (2 * CursorPlace.x) + (2 * CursorPlace.y) + CENTER_OFFSET) = "　";
	*(Buffer + BUFFER_WIDTH * (2 * CursorPlace.x) + (2 * CursorPlace.y + 2) + CENTER_OFFSET) = "　";
	*(Buffer + BUFFER_WIDTH * (2 * CursorPlace.x + 2) + (2 * CursorPlace.y) + CENTER_OFFSET) = "　";
	*(Buffer + BUFFER_WIDTH * (2 * CursorPlace.x + 2) + (2 * CursorPlace.y + 2) + CENTER_OFFSET) = "　";
	return;
}

// 绘制棋盘光标
void DrawCursor(place CursorPlace, char **Buffer)
{
	// x为行，y为列
	*(Buffer + BUFFER_WIDTH * (2 * CursorPlace.x) + (2 * CursorPlace.y) + CENTER_OFFSET) = "┌";
	*(Buffer + BUFFER_WIDTH * (2 * CursorPlace.x) + (2 * CursorPlace.y + 2) + CENTER_OFFSET) = "┐";
	*(Buffer + BUFFER_WIDTH * (2 * CursorPlace.x + 2) + (2 * CursorPlace.y) + CENTER_OFFSET) = "└";
	*(Buffer + BUFFER_WIDTH * (2 * CursorPlace.x + 2) + (2 * CursorPlace.y + 2) + CENTER_OFFSET) = "┘";
	return;
}

void RestoreStyle(char **Buffer, place Place)
{
	if (Place.x == 0) // 棋盘第一行的情况
	{
		switch (Place.y)
		{
		case 0: *(Buffer + BUFFER_WIDTH * 1 + CENTER_OFFSET + 1) = "┏"; return;
		case 14: *(Buffer + BUFFER_WIDTH * 1 + CENTER_OFFSET + 29) = "┓"; return;
		default: *(Buffer + BUFFER_WIDTH * 1 + CENTER_OFFSET + 2* Place.y + 1) = "┯"; return;
		}
	}
	else if (Place.x == 14) //棋盘最后一行的情况
	{
		switch (Place.y)
		{
		case 0: *(Buffer + BUFFER_WIDTH * 29 + CENTER_OFFSET + 1) = "┗"; return;
		case 14: *(Buffer + BUFFER_WIDTH * 29 + CENTER_OFFSET + 29) = "┛"; return;
		default: *(Buffer + BUFFER_WIDTH * 29 + CENTER_OFFSET + 2 * Place.y + 1) = "┷"; return;
		}
	}
	else if (Place.x == 3) // 上面一行星点
	{
		if (Place.y == 3 || Place.y == 11)
		{
			*(Buffer + BUFFER_WIDTH * 7 + CENTER_OFFSET + 2 * Place.y + 1) = "╋";
			return;
		}
		else
		{
			*(Buffer + BUFFER_WIDTH * 7 + CENTER_OFFSET + 2 * Place.y + 1) = "┼";
			return;
		}
	}
	else if (Place.x == 11) // 下面一行星点
	{
		if (Place.y == 3 || Place.y == 11)
		{
			*(Buffer + BUFFER_WIDTH * 23 + CENTER_OFFSET + 2 * Place.y + 1) = "╋";
			return;
		}
		else
		{
			*(Buffer + BUFFER_WIDTH * 23 + CENTER_OFFSET + 2 * Place.y + 1) = "┼";
			return;
		}
	}
	else if (Place.x == 7 && Place.y == 7) //天元
	{
		*(Buffer + BUFFER_WIDTH * 15 + CENTER_OFFSET + 15) = "╋";
		return;
	}
	else //中间的情况
	{
		switch (Place.y)
		{
		case 0: *(Buffer + BUFFER_WIDTH * (2*Place.x+1) + CENTER_OFFSET + 1) = "┠"; return;
		case 14: *(Buffer + BUFFER_WIDTH * (2 * Place.x + 1) + CENTER_OFFSET + 29) = "┨"; return;
		default: *(Buffer + BUFFER_WIDTH * (2 * Place.x + 1) + CENTER_OFFSET + 2 * Place.y + 1) = "┼"; return;
		}
	}
}


//针对display.h中打印坐标时分配在堆中的内存，在棋盘销毁后（单局游戏退出时）要释放内存
void FreeHeapMemory(char **Buffer)
{
	// 释放纵坐标
	for (int i = 0; i < CHESSBORAD_LINES; i++)
	{
		free(*(Buffer + (2 * i + 1) * BUFFER_WIDTH + CENTER_OFFSET));
		*(Buffer + (2 * i + 1) * BUFFER_WIDTH + CENTER_OFFSET) = NULL;
	}
	// 释放横坐标
	for (int i = 0; i < CHESSBOARD_WIDTH; i++)
	{
		free(*(Buffer + 30 * BUFFER_WIDTH + 2 * i + 1 + CENTER_OFFSET));
		*(Buffer + 30 * BUFFER_WIDTH + 2 * i + 1 + CENTER_OFFSET) = NULL;
	}
	return;
}


// 列表选择功能，用于纵向列表选择光标的打印和选择，自动居中
int ListChoose(char ** Buffer, char ** OldBuffer, unsigned int LineNum, unsigned int ItemNum, int LeftCenterOffset, int RightCenterOffset, int StartPointer)
{
	int CurrentPointer = StartPointer;
	while (1)
	{
		*(Buffer + LineNum * BUFFER_WIDTH + 2 * BUFFER_WIDTH * (CurrentPointer - 1) + BUFFER_WIDTH / 2 - LeftCenterOffset) = "→";
		*(Buffer + LineNum * BUFFER_WIDTH + 2 * BUFFER_WIDTH * (CurrentPointer - 1) + BUFFER_WIDTH / 2 + RightCenterOffset) = "←";
		RefreshScreen(OldBuffer, Buffer);
		rewind(stdin);
		int key = GetKey();
		switch (key)
		{
		case MOVE_UP:
		{
			*(Buffer + LineNum * BUFFER_WIDTH + 2 * BUFFER_WIDTH * (CurrentPointer - 1) + BUFFER_WIDTH / 2 - LeftCenterOffset) = "  ";
			*(Buffer + LineNum * BUFFER_WIDTH + 2 * BUFFER_WIDTH * (CurrentPointer - 1) + BUFFER_WIDTH / 2 + RightCenterOffset) = "  ";
			CurrentPointer--;
			break;
		}
		case MOVE_DOWN:
		{
			*(Buffer + LineNum * BUFFER_WIDTH + 2 * BUFFER_WIDTH * (CurrentPointer - 1) + BUFFER_WIDTH / 2 - LeftCenterOffset) = "  ";
			*(Buffer + LineNum * BUFFER_WIDTH + 2 * BUFFER_WIDTH * (CurrentPointer - 1) + BUFFER_WIDTH / 2 + RightCenterOffset) = "  ";
			CurrentPointer++;
			break;
		}
		case CONFRM: return CurrentPointer;
		default:
			break;
		}
		//按键循环
		if (CurrentPointer == ItemNum+1) CurrentPointer = 1;
		else if (CurrentPointer == 0) CurrentPointer = ItemNum;
	}
	
	return 0;
}
