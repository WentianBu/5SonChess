#pragma once
// keyboard.h
// 用于接收键盘按键操作，尤其是需要接收方向键、控制键、功能键等非基础ASCII字符时使用

// 是否使用JSON文件定义按键功能
#define USE_JSON_PROFILE 0

// 若不使用JSON文件来定义按键，则自定义程度受到限制。
/*
 *上下左右和WASD控制光标
 *空格和Enter控制落子
 *Esc退出当前游戏
 *Backspace和H悔棋
 */
// 具体功能是否启用使用宏的值定义
// TODO：在此使用宏定义按键功能是否启用

// WASD移动光标
#define WASD_MOVE 1
// 上下左右方向键移动光标
#define UDLR_MOVE 1

// Esc退出当前游戏
#define ESC_EXIT 1

// 空格落子
#define SPACE_CONFRM 1
// ENTER落子
#define ENTER_CONFRM 1

// Backspace悔棋
#define BKSPC_REGRET 1
// H悔棋
#define H_REGRET 1


// 定义按键操作对应的宏返回值，提升程序可读性
#define MOVE_UP 1
#define MOVE_DOWN 2
#define MOVE_LEFT 3
#define MOVE_RIGHT 4

#define EXIT 0 // 退出当前游戏
#define REGRET 5 //悔棋
#define CONFRM 6 //落子

#define INVALID -1 // 无效按键，提示主程序进行处理

#if USE_JSON_PROFILE==0
int GetKey()
{
	char key1, key2;
	rewind(stdin);
	key1 = _getch();
	
	if (key1 == ' '&&SPACE_CONFRM == 1) return CONFRM;
	if (key1 == '\r'&&ENTER_CONFRM == 1) return CONFRM;

	if (key1 == 27 && ESC_EXIT) return EXIT;
	
	if ((key1 == 'h' || key1 == 'H') && H_REGRET == 1) return REGRET;
	if (key1 == '\b'&&BKSPC_REGRET == 1) return REGRET;

	if (WASD_MOVE == 1)
	{
		switch (key1)
		{
		case 'w':case 'W': return MOVE_UP;
		case 'a':case 'A': return MOVE_LEFT;
		case 's':case 'S': return MOVE_DOWN;
		case 'd':case 'D': return MOVE_RIGHT;
		}
	}

	if (key1 == -32 && UDLR_MOVE == 1)
	{
		key2 = _getch();
		switch (key2)
		{
		case 72: return MOVE_UP;
		case 80: return MOVE_DOWN;
		case 75: return MOVE_LEFT;
		case 77: return MOVE_RIGHT;
		}
	}

	return INVALID;
}


#endif // USE_JSON_PROFILE==0


