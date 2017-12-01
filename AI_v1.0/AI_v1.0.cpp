// AI_v1.0.cpp: 定义 DLL 应用程序的导出函数。
// 这是自己设计的第一版AI，无禁手规则。

/*
设计思路：
1. 本版本AI采用定点评分的算法，即针对棋盘上各点搜索落子后所得棋型，综合给出得分，在整个棋盘上取得分最高点进行落子。
2. 应用程序接口仍然采用张宁鑫的旧版接口，主函数为API_Main()
*/

#include "stdafx.h"

#define EXP extern "C" _declspec(dllexport)

typedef struct
{
	// 行列针对二维数组而言
	int x; // 行
	int y; // 列
}place;

struct TypeCounter
{
	unsigned int Active2 = 0; // 连活二：再走一步可以形成活三的连二
	unsigned int Jump2 = 0; // 跳活二：中间有一个空位的活二
	unsigned int JumpJump2 = 0; // 大跳活二：中间有两个空位的活二
	unsigned int Sleep2 = 0; // 眠二：再走一着可以形成眠三的二
	unsigned int Dead2 = 0; // 死二：不能成五的二
	
	unsigned int Active3 = 0; // 活三：两端有威胁的三
	unsigned int Jump3 = 0; // 跳活三：中间夹有一个威胁的活三
	unsigned int JumpJump3 = 0; // 跳三：1、3、5型的三，下一子只能形成冲四
	unsigned int Sleep3 = 0; // 眠三：再走一着可以形成冲四的三
	unsigned int Dead3 = 0; // 死三：不能成五的三

	unsigned int Active4 = 0; // 活四：两端可以成五的四
	unsigned int Rush4 = 0; // 冲四：一个位置可以成五的四
	unsigned int Dead4 = 0; // 死四：不能成五的四

	unsigned int Chicken = 0; // 连五：大吉大利，今晚吃鸡

	unsigned int LongLink = 0; // 长连
};


EXP int PrintVersionInfo()
{
	printf("DLL Name: AI_v1.0.dll\n");
	printf("Version: 0.1.0.171120_base\n");
	printf("Application Interface Type: ZhangNingxin's old edition\n");
	printf("Release Note: 五子棋AI无禁手简易测试版\n");
	printf("已经启用对张宁鑫旧版API的支持。\n");
	printf("AI已经准备就绪。\n\n");
	return 2; // 返回值2表示API是最新定义的版本
}




EXP place API_Main(place LastPlace, int Turn, HMODULE hself)
{
	// Turn的记录标准是黑方（0）,白方（1）

	// 定义返回值
	place Place = { 0,0 };
	// 定义并维护棋型表
	return Place;
}