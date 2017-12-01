#pragma once
// display.h
// 五子棋程序中负责显示的函数定义在这个模块中
// display模块是五子棋主程序的核心模块之一

#include "declaration.h"

// 以下是程序显示模块的关键宏定义

#define BUFFER_LINES 32 //缓冲器行数
#define BUFFER_WIDTH 33 //缓冲器宽度

#define CHESSBORAD_LINES 15 //棋盘行数
#define CHESSBOARD_WIDTH 15 //棋盘列数
#define MAX_ROUND_NUMBER CHESSBORAD_LINES*CHESSBOARD_WIDTH // 最大走子数目，用于判断和棋

// 定义：居中偏移量
// 为了应对可变宽度的缓冲器而保持棋盘居中显示，需要在指针上添加一个居中偏移量
#define CENTER_OFFSET (BUFFER_WIDTH - 2 * CHESSBOARD_WIDTH) / 2

// 初始化缓冲器，在其中填充空格（注意：不是赋值为NULL。缓冲器定义时已经赋初值NULL。）
// 传入要初始化的缓冲器的首地址
void InitiateBuffer(char **p);


// 将光标移动到控制台的指定位置
// x是横坐标，y是纵坐标
// 说明：控制台的坐标从0开始计算，即左上角第一个字符的位置是(0,0)
void gotoxy(short x, short y);


//无闪烁地刷新屏幕
//注意：Buffer和OldBuffer是新旧缓冲器数组的首地址，即二位字符指针数组中Buffer[0][0]和OldBuffer[0][0]的地址
void RefreshScreen(char **OldBuffer, char **Buffer);


// 绘制空棋盘
// 说明：传入缓冲器数组的首地址。本函数只修改缓冲器内容，最后需要使用RefreshScreen()来刷新显示。
void DrawBlankChessboard(char **Buffer);

// 清除原有光标
// 说明：本函数一次性清除光标的四个部分。
// 需要传入光标所指位置的坐标(行，列)，以及缓冲器数组的首地址。这里的行/列是相对于棋盘数组而言。
void CleanCursor(place CursorPlace, char **Buffer);

// 绘制棋盘光标
// 说明：本函数一次性绘制光标的四个部分。
// 需要传入光标所指位置的坐标(行，列)，以及缓冲器数组的首地址。这里的行/列是相对于棋盘数组而言。
void DrawCursor(place CursorPlace, char **Buffer);

// 悔棋时恢复棋盘某个落子点的样式
void RestoreStyle(char **Buffer, place Place);

// 棋盘销毁时释放堆内存
// 只需要对新缓冲器释放即可。
void FreeHeapMemory(char **Buffer);
