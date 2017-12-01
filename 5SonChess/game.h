#pragma once
// game.h
// 定义了负责管理单局游戏的相关函数，使程序更加清晰易读，模块化程度更高
// game模块是五子棋主程序的核心模块之一，负责游戏的流程运营

#include "record.h" // 记录棋局需要包含此文件
#include "keyboard.h" // 键盘操作需要该文件
#include "LoadAI.h" // 获取AI接口需要包含此文件


// 定义结构体
typedef struct AIMark
{
	// 说明：用于一次性传递AI模组的标记信息，减少参数量。
	int AI_Type; // AI使用的应用程序接口类型编号，定义在LoadAI.h中
	HMODULE hLib; // AI模组的句柄，主程序加载并确认无误后传递进来
	// 注意：对于人类玩家，以上两个变量均应为0
}AIMark;


// 以下定义游戏管理模块的外部函数

/* 单局游戏管理器
 * 负责单局游戏的管理：初始化游戏，授权玩家操作，处理操作，显示内容管理，调用函数记录棋局和链表管理，判定胜负
 * @return   胜利方（黑方为0，白方为1，平局为2，负值均为错误码，未分出胜负时Winner变量应该为3）
 * 对mode的定义：
 * PVP模式：1
 * PVE模式：2 -（玩家执黑），3 -（AI执黑）
 * EVE模式：4 -（单步），5 -（慢速），6 -（中速），7 -（快速），8 -（一次性）
 */
int GameManager(int mode, AIMark Player1, AIMark Player2, char ** Buffer, char ** OldBuffer);
