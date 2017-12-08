#pragma once
// keyboard.h
// 用于接收键盘按键操作，尤其是需要接收方向键、控制键、功能键等非基础ASCII字符时使用

#include "display.h"

// 是否使用JSON文件定义按键功能
#define USE_JSON_PROFILE 0

#define MOUSE

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

// 定义用户操作结构体
typedef struct USER_OPERATE
{
	int type; // 操作类型，CONFIRM/REGRET/EXIT
	place cPlace; //落子位置，不落子时设为无效值{-1,-1}，此外负数均为无效值
}USER_OPERATE;

// 定义按键操作对应的宏返回值，提升程序可读性
#define MOVE_UP 1
#define MOVE_DOWN 2
#define MOVE_LEFT 3
#define MOVE_RIGHT 4

#define EXIT 0 // 退出当前游戏
#define REGRET 5 //悔棋
#define CONFRM 6 //落子

#define INVALID -1 // 无效按键，提示主程序进行处理


USER_OPERATE GetOperate(place CursorPlace,  char ** Buffer, char ** OldBuffer);
// 直接调用该函数获取按键信息，而无须处理不同的按键以及方向键。
// 函数返回以上几种按键结果：光标移动、退出游戏、悔棋、落子以及无效按键。对不同按键的支持通过更改宏来进行。
int GetKey();
