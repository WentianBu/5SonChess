#pragma once
// macro.h
// 用于定义整个程序的公用宏

#define BUFFER_LINES 32 //缓冲器行数
#define BUFFER_WIDTH 33 //缓冲器宽度

#define CHESSBORAD_LINES 15 //棋盘行数
#define CHESSBOARD_WIDTH 15 //棋盘列数
#define MAX_ROUND_NUMBER CHESSBORAD_LINES*CHESSBOARD_WIDTH // 最大走子数目，用于判断和棋

// 定义：居中偏移量
// 为了应对可变宽度的缓冲器而保持棋盘居中显示，需要在指针上添加一个居中偏移量
#define CENTER_OFFSET (BUFFER_WIDTH - 2 * CHESSBOARD_WIDTH) / 2