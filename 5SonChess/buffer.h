#pragma once
// buffer.h
// 定义了缓冲器相关的公用宏，以及缓冲器相关的内存操作函数


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

// 棋盘销毁时释放堆内存
// 只需要对新缓冲器释放即可。
void FreeHeapMemory(char **Buffer);
