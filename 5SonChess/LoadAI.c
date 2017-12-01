#include "stdafx.h"
#include "LoadAI.h"

// 加载dll
HMODULE OpenDLL(int *AI_type)
{
	OPENFILENAME DLL = { 0 };
	TCHAR DLLName[MAX_PATH] = { 0 };
	DLL.lStructSize = sizeof(OPENFILENAME);//结构体大小  
	DLL.hwndOwner = NULL;//拥有着窗口句柄，为NULL表示对话框是非模态的，实际应用中一般都要有这个句柄  
	DLL.lpstrFilter = TEXT("AI动态链接库(*.dll)\0*.dll\0所有文件(*.*)\0*.*\0\0");//设置过滤  
	DLL.nFilterIndex = 1;//过滤器索引  
	DLL.lpstrFile = DLLName;//接收返回的文件名，注意第一个字符需要为NULL  
	DLL.nMaxFile = sizeof(DLLName);//缓冲区长度  
	DLL.lpstrInitialDir = NULL;//初始目录为默认  
	DLL.lpstrTitle = TEXT("请选择AI使用的动态链接库");//使用系统默认标题留空即可  
	DLL.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;//文件、目录必须存在，隐藏只读选项  

	HMODULE hDllLib = NULL;
	FARPROC funcpVersion = NULL;
	int FirstTimeOpen = 1; // 首次打开文件的标记
	int UserCancel = 0;
	do
	{
		if (FirstTimeOpen == 0) printf("无法识别%ls，请重新选择AI模组。\n", DLLName);
		UserCancel = GetOpenFileName(&DLL);
		if (UserCancel == 0) return 0;// 用户取消则返回0
		printf("正在加载%ls...\n", DLLName);
		FirstTimeOpen = 0;
		hDllLib = LoadLibrary(DLLName);
		if (hDllLib != 0) funcpVersion = GetProcAddress(hDllLib, "PrintVersionInfo"); // 查找握手函数
	} while (funcpVersion == NULL);
	(*AI_type) = (*funcpVersion)();// 输出dll信息
	return hDllLib;
}
