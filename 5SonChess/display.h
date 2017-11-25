#pragma once
#include "stdafx.h"
#include "buffer.h"

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
void CleanCursor(int x, int y, char **Buffer);

// 绘制棋盘光标
// 说明：本函数一次性绘制光标的四个部分。
// 需要传入光标所指位置的坐标(行，列)，以及缓冲器数组的首地址。这里的行/列是相对于棋盘数组而言。
void DrawCursor(int x, int y, char **Buffer);