// AI.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
typedef int(*tFuncpAdd)(int, int);

extern "C" _declspec(dllexport) int HelloWorld(HMODULE Self)
{
	printf("Hello world!\n");
	tFuncpAdd funcpAdd = (tFuncpAdd)GetProcAddress(Self, "Add");
	(*funcpAdd)(1,1);
	return 0;
}

extern "C" _declspec(dllexport) int Add(int x, int y)
{
	printf("%d", x + y);
	return 0;
}

