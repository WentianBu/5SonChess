#pragma once
// declaration.h
// 用于声明程序中广泛使用的结构、宏、全局变量等

// 落子位置
// 行、列针对Chess[][]数组而言
typedef struct place
{
	int x; // 行
	int y; // 列
}place;