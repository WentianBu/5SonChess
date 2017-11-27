#pragma once
// LoadAI.h
// 用于选择和加载DLL模组
// 将来和加载文件相关的函数都会放在这里

#include "stdafx.h"
#include "UnionDefine.h"

// 接口定义
typedef place(*tFuncpAI_Zhang)(int(*)[15], int); // 张宁鑫的旧版API的接口


// 选择并打开dll文件
// 传入指向AI_Type的指针，用于让主函数获取AI的接口类型
// 返回LoadLibrary后的句柄
HMODULE OpenDLL(int *AI_type);