// AI_ZhangNingxin.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

#define EXP extern "C" _declspec(dllexport)

#define LianWu 1000000
#define HuoSi 100000
#define ChongSi 1000//中间无空格的四连子
#define MianSi 900//中间有空格的四连子
#define HuoSan 1000
#define MianSan 100
#define HuoEr 100
#define MianEr 10
#define HuoYi 10
#define MianYi 1

typedef struct
{
	int x; // 列
	int y; // 行
}place;

int iPValue = 0;//当前位置的权值

int xTwoSidesColor[8];//各个方向的连子旁边的颜色
int yTwoSidesColor[8];
int sTwoSidesColor[8];
int tTwoSidesColor[8];

EXP int PrintVersionInfo()
{
	printf("DLL Name: AI_ZhangNingxin.dll\n");
	printf("Version: 1.0.0\n");
	printf("Application Interface Type: ZhangNingxin's old edition\n");
	printf("Release Note: 张宁鑫的半成品智障五子棋AI程序，于2017年11月2日改编成DLL。\n");
	printf("已经启用对张宁鑫旧版API的支持。\n");
	printf("AI已经准备就绪。\n\n");
	return 1; // 返回值1表示API是张宁鑫老版本
}

EXP int xCount(int x, int y, int iPosition[15][15], int iPersonInTurn)
{
	int iNumx = 1;
	for (int i = 1; i <5; i++)//向下方向上检查
	{
		if (iPosition[x + i][y] == iPersonInTurn)
		{
			if (x + i >= 15)
			{
				break;
			}
			iNumx++;
		}
		else
			break;
	}
	for (int i = 1; i < 5; i++)//向上方向检查
	{


		if (iPosition[x - i][y] == iPersonInTurn)
		{
			if (x <= i - 1)
			{
				break;
			}
			iNumx++;
		}
		else
			break;
	}
	return iNumx;
}
EXP int yCount(int x, int y, int iPosition[15][15], int iPersonInTurn)
{
	int iNumy = 1;
	for (int i = 1; i <5; i++)//向左方向上检查
	{


		if (iPosition[x][y - i] == iPersonInTurn)
		{
			if (y <= i - 1)
			{
				break;
			}
			iNumy++;
		}
		else
			break;
	}
	for (int i = 1; i < 5; i++)//向右方向检查
	{


		if (iPosition[x][y + i] == iPersonInTurn)
		{
			if (y + i >= 15)
			{
				break;
			}
			iNumy++;
		}
		else
			break;
	}
	return iNumy;
}
EXP int sCount(int x, int y, int iPosition[15][15], int iPersonInTurn)
{
	int iNums = 1;
	for (int i = 1; i < 5; i++)//向左上方向检查
	{


		if (iPosition[x - i][y - i] == iPersonInTurn)
		{
			if (x <= i - 1)
			{
				break;
			}
			else if (y <= i - 1)
			{
				break;
			}
			iNums++;
		}
		else
			break;

	}
	for (int i = 1; i < 5; i++)//向右下方向检查
	{


		if (iPosition[x + i][y + i] == iPersonInTurn)
		{
			if (x + i >= 15)
			{
				break;
			}
			else if (y + i >= 15)
			{
				break;
			}
			iNums++;
		}
		else
			break;
	}
	return iNums;
}
EXP int tCount(int x, int y, int iPosition[15][15], int iPersonInTurn)
{
	int iNumt = 1;
	for (int i = 1; i <5; i++)//在右上方向上检查
	{


		if (iPosition[x - i][y + i] == iPersonInTurn)
		{
			if (x <= i - 1)
			{
				break;
			}
			else if (y + i >= 15)
			{
				break;
			}
			iNumt++;
		}
		else
			break;
	}
	for (int i = 1; i < 5; i++)//向左下方向检查
	{


		if (iPosition[x + i][y - i] == iPersonInTurn)
		{
			if (x + i >= 15)
			{
				break;
			}
			else if (y <= i - 1)
			{
				break;
			}
			iNumt++;
		}
		else
			break;
	}
	return iNumt;
}

EXP void xAiCheckTwoSides(int x, int y, int iPosition[15][15], int iPersonInTurn)//x方向上连子两边的颜色
{
	for (int i = 0; i<5; x++)
	{
		if (iPosition[x + i][y] != iPersonInTurn)
		{
			xTwoSidesColor[0] = iPosition[x + i][y];
			xTwoSidesColor[2] = iPosition[x + i + 1][y];
			xTwoSidesColor[4] = iPosition[x + i + 2][y];
			xTwoSidesColor[6] = iPosition[x + i + 3][y];
			break;
		}
	}
	for (int i = 0; i<5; x++)
	{
		if (iPosition[x - i][y] != iPersonInTurn)
		{
			xTwoSidesColor[1] = iPosition[x - i][y];
			xTwoSidesColor[3] = iPosition[x - i - 1][y];
			xTwoSidesColor[5] = iPosition[x - i - 2][y];
			xTwoSidesColor[7] = iPosition[x - i - 3][y];
			break;
		}
	}
}
EXP void yAiCheckTwoSides(int x, int y, int iPosition[15][15], int iPersonInTurn)//y方向上连子两边的颜色
{
	for (int i = 0; i<5; x++)
	{
		if (iPosition[x][y + i] != iPersonInTurn)
		{
			yTwoSidesColor[0] = iPosition[x][y + i];
			yTwoSidesColor[2] = iPosition[x][y + i + 1];
			yTwoSidesColor[4] = iPosition[x][y + i + 2];
			yTwoSidesColor[6] = iPosition[x][y + i + 3];
			break;
		}
	}
	for (int i = 0; i<5; x++)
	{
		if (iPosition[x][y - i] != iPersonInTurn)
		{
			yTwoSidesColor[1] = iPosition[x][y - i];
			yTwoSidesColor[3] = iPosition[x][y - i - 1];
			yTwoSidesColor[5] = iPosition[x][y - i - 2];
			yTwoSidesColor[7] = iPosition[x][y - i - 3];
			break;
		}
	}
}
EXP void sAiCheckTwoSides(int x, int y, int iPosition[15][15], int iPersonInTurn)//s方向上连子两边的颜色
{
	for (int i = 0; i<5; x++)
	{
		if (iPosition[x + i][y + i] != iPersonInTurn)
		{
			sTwoSidesColor[0] = iPosition[x + i][y + i];
			sTwoSidesColor[2] = iPosition[x + i + 1][y + i + 1];
			sTwoSidesColor[4] = iPosition[x + i + 2][y + i + 2];
			sTwoSidesColor[6] = iPosition[x + i + 3][y + i + 3];
			break;
		}
	}
	for (int i = 0; i<5; x++)
	{
		if (iPosition[x - i][y - i] != iPersonInTurn)
		{
			sTwoSidesColor[1] = iPosition[x - i][y - i];
			sTwoSidesColor[3] = iPosition[x - i - 1][y - i - 1];
			sTwoSidesColor[5] = iPosition[x - i - 2][y - i - 2];
			sTwoSidesColor[7] = iPosition[x - i - 3][y - i - 3];
			break;
		}
	}
}
EXP void tAiCheckTwoSides(int x, int y, int iPosition[15][15], int iPersonInTurn)
{
	for (int i = 0; i<5; x++)
	{
		if (iPosition[x + i][y - i] != iPersonInTurn)
		{
			tTwoSidesColor[0] = iPosition[x + i][y - i];
			tTwoSidesColor[2] = iPosition[x + i + 1][y - i - 1];
			tTwoSidesColor[4] = iPosition[x + i + 2][y - i - 2];
			tTwoSidesColor[6] = iPosition[x + i + 3][y - i - 3];
			break;
		}

	}
	for (int i = 0; i<5; x++)
	{
		if (iPosition[x - i][y + i] != iPersonInTurn)
		{
			tTwoSidesColor[1] = iPosition[x - i][y + i];
			tTwoSidesColor[3] = iPosition[x - i - 1][y + i + 1];
			tTwoSidesColor[5] = iPosition[x - i - 2][y + i + 2];
			tTwoSidesColor[7] = iPosition[x - i - 3][y + i + 3];
			break;
		}
	}
}

EXP int AiCheck(int x, int y, int iPosition[15][15], int iPersonInTurn, int iNum, int TwoSidesColor[])
{
	int iValue;
	if (iNum == 5)//连子为5的所有情况
	{
		iValue = LianWu;
	}
	else if (iNum == 4)//连子为4的所有情况
	{
		if (TwoSidesColor[0] == 0 && TwoSidesColor[1] == 0)
		{
			iValue = HuoSi;
		}
		else if (TwoSidesColor[0] != 0 && TwoSidesColor[1] != 0)
		{
			iValue = MianYi;
		}
		else
		{
			iValue = ChongSi;
		}

	}
	else if (iNum == 3)//连子为3的所有情况
	{
		if (TwoSidesColor[0] == 0 && TwoSidesColor[1] == 0)
		{
			if (TwoSidesColor[2] == 3 - iPersonInTurn&&TwoSidesColor[3] == 3 - iPersonInTurn)
			{
				iValue = MianSan;
			}
			else if (TwoSidesColor[2] == 0 || TwoSidesColor[3] == 0)
			{
				iValue = HuoSan;
			}
			else if (TwoSidesColor[2] == iPersonInTurn)
			{
				if (TwoSidesColor[4] != 3 - iPersonInTurn)
				{
					iValue = ChongSi;
				}
				else
				{
					iValue = MianSi;
				}
			}
			else if (TwoSidesColor[3] == iPersonInTurn)
			{
				if (TwoSidesColor[5] != 3 - iPersonInTurn)
				{
					iValue = ChongSi;
				}
				else
				{
					iValue = MianSi;
				}
			}
		}
		if (TwoSidesColor[0] == 3 - iPersonInTurn&&TwoSidesColor[1] == 0)
		{
			if (TwoSidesColor[3] == iPersonInTurn)
			{
				iValue = MianSi;
			}
			else if (TwoSidesColor[3] = 0)
			{
				iValue = MianSan;
			}
			else
			{
				iValue = MianYi;
			}
		}
		if (TwoSidesColor[1] == 3 - iPersonInTurn&&TwoSidesColor[0] == 0)
		{
			if (TwoSidesColor[2] == iPersonInTurn)
			{
				iValue = MianSi;
			}
			else if (TwoSidesColor[2] = 0)
			{
				iValue = MianSan;
			}
			else
			{
				iValue = MianYi;
			}
		}
		else
		{
			iValue = MianYi;
		}
	}
	else if (iNum == 2)
	{
		if (TwoSidesColor[1] == 3 - iPersonInTurn&&TwoSidesColor[0] == 3 - iPersonInTurn)
		{
			iValue = MianYi;
		}
		else if (TwoSidesColor[1] == 3 - iPersonInTurn&&TwoSidesColor[0] == 0)
		{
			if (TwoSidesColor[2] == 3 - iPersonInTurn)
			{
				iValue = MianYi;
			}
			else if (TwoSidesColor[2] == 0)
			{
				if (TwoSidesColor[4] != 3 - iPersonInTurn)
				{
					iValue = MianEr;
				}
				else
				{
					iValue = MianYi;
				}
			}
			else
			{
				if (TwoSidesColor[4] == iPersonInTurn)
				{
					iValue = MianSi;
				}
				else if (TwoSidesColor[4] == 0)
				{
					iValue = MianSan;
				}
				else
				{
					iValue = MianYi;
				}
			}
		}
		else if (TwoSidesColor[1] == 0 && TwoSidesColor[0] == 3 - iPersonInTurn)
		{
			if (TwoSidesColor[3] == 3 - iPersonInTurn)
			{
				iValue = MianYi;
			}
			else if (TwoSidesColor[3] == 0)
			{
				if (TwoSidesColor[5] != iPersonInTurn)
				{
					iValue = MianEr;
				}
				else
				{
					iValue = MianYi;
				}
			}
			else
			{
				if (TwoSidesColor[5] == iPersonInTurn)
				{
					iValue = MianSi;
				}
				else if (TwoSidesColor[5] == 0)
				{
					iValue = MianSan;
				}
				else
				{
					iValue = MianYi;
				}
			}
		}
		else
		{
			if (TwoSidesColor[2] == 3 - iPersonInTurn&&TwoSidesColor[3] == 3 - iPersonInTurn)
			{
				iValue = MianYi;
			}
			else if (TwoSidesColor[2] == 3 - iPersonInTurn&&TwoSidesColor[3] == 0)
			{
				if (TwoSidesColor[5] == 0)
				{
					iValue = HuoEr;
				}
				else if (TwoSidesColor[5] == 3 - iPersonInTurn)
				{
					iValue = HuoEr;
				}
				else
				{
					iValue = MianSan;
				}
			}
			else if (TwoSidesColor[2] == 0 && TwoSidesColor[3] == 3 - iPersonInTurn)
			{
				if (TwoSidesColor[4] == 0)
				{
					iValue = HuoEr;
				}
				else if (TwoSidesColor[4] == 3 - iPersonInTurn)
				{
					iValue = HuoEr;
				}
				else
				{
					iValue = MianSan;
				}
			}
			else
			{
				if (TwoSidesColor[4] == iPersonInTurn || TwoSidesColor[5] == iPersonInTurn)
				{
					iValue = MianSan;
				}
				else
				{
					iValue = HuoEr;
				}
			}
		}
	}
	else
	{
		if (TwoSidesColor[0] == 3 - iPersonInTurn&&TwoSidesColor[1] == 3 - iPersonInTurn)
		{
			iValue = MianYi;
		}
		else if (TwoSidesColor[0] == 3 - iPersonInTurn&&TwoSidesColor[1] == 0)
		{
			if (TwoSidesColor[3] == 3 - iPersonInTurn)
			{
				iValue = MianYi;
			}
			else if (TwoSidesColor[3] == iPersonInTurn)
			{
				if (TwoSidesColor[5] == 3 - iPersonInTurn)
				{
					iValue = MianYi;
				}
				else if (TwoSidesColor[5] == iPersonInTurn)
				{
					if (TwoSidesColor[7] == 3 - iPersonInTurn)
					{
						iValue = MianYi;
					}
					else if (TwoSidesColor[7] == iPersonInTurn)
					{
						iValue = ChongSi;
					}
					else
					{
						iValue = MianSan;
					}
				}
				else
				{
					if (TwoSidesColor[7] == 3 - iPersonInTurn)
					{
						iValue = MianYi;
					}
					else if (TwoSidesColor[7] == iPersonInTurn)
					{
						iValue = MianSan;
					}
					else
					{
						iValue = MianYi;
					}
				}
			}
			else
			{
				if (TwoSidesColor[5] == 3 - iPersonInTurn)
				{
					iValue = MianYi;
				}
				else if (TwoSidesColor[5] == iPersonInTurn)
				{
					if (TwoSidesColor[7] == iPersonInTurn)
					{
						iValue = MianSan;
					}
					else if (TwoSidesColor[7] == 3 - iPersonInTurn)
					{
						iValue = MianYi;
					}
					else
					{
						iValue = MianEr;
					}
				}
				else
				{
					iValue = MianYi;
				}
			}
		}
		else if (TwoSidesColor[1] == 3 - iPersonInTurn&&TwoSidesColor[0] == 0)
		{
			if (TwoSidesColor[2] == 3 - iPersonInTurn)
			{
				iValue = MianYi;
			}
			else if (TwoSidesColor[2] == iPersonInTurn)
			{
				if (TwoSidesColor[4] == 3 - iPersonInTurn)
				{
					iValue = MianYi;
				}
				else if (TwoSidesColor[4] == iPersonInTurn)
				{
					if (TwoSidesColor[6] == 3 - iPersonInTurn)
					{
						iValue = MianYi;
					}
					else if (TwoSidesColor[6] == iPersonInTurn)
					{
						iValue = ChongSi;
					}
					else
					{
						iValue = MianSan;
					}
				}
				else
				{
					if (TwoSidesColor[6] == 3 - iPersonInTurn)
					{
						iValue = MianYi;
					}
					else if (TwoSidesColor[6] == iPersonInTurn)
					{
						iValue = MianSan;
					}
					else
					{
						iValue = MianEr;
					}
				}
			}
			else
			{
				if (TwoSidesColor[4] == 3 - iPersonInTurn)
				{
					iValue = MianYi;
				}
				else if (TwoSidesColor[4] == iPersonInTurn)
				{
					if (TwoSidesColor[6] == iPersonInTurn)
					{
						iValue = MianSan;
					}
					else if (TwoSidesColor[6] == 3 - iPersonInTurn)
					{
						iValue = MianYi;
					}
					else
					{
						iValue = MianEr;
					}
				}
				else
				{
					iValue = MianYi;
				}
			}
		}
		else
		{
			if (TwoSidesColor[2] == 3 - iPersonInTurn&&TwoSidesColor[3] == 3 - iPersonInTurn)
			{
				iValue = MianYi;
			}
			else if (TwoSidesColor[2] == 3 - iPersonInTurn&&TwoSidesColor[3] == 0)
			{
				if (TwoSidesColor[5] == 3 - iPersonInTurn)
				{
					iValue = MianYi;
				}
				else if (TwoSidesColor[5] == iPersonInTurn)
				{
					if (TwoSidesColor[7] == iPersonInTurn)
					{
						iValue = MianSan;
					}
					else if (TwoSidesColor[7] == 3 - iPersonInTurn)
					{
						iValue = MianYi;
					}
					else
					{
						iValue = MianEr;
					}

				}
				else
				{
					if (TwoSidesColor[7] == iPersonInTurn)
					{
						iValue = MianYi;
					}
					else if (TwoSidesColor[7] == 3 - iPersonInTurn)
					{
						iValue = MianYi;
					}
					else
					{
						iValue = MianYi;
					}
				}
			}
			else if (TwoSidesColor[3] == 3 - iPersonInTurn&&TwoSidesColor[2] == 0)
			{
				if (TwoSidesColor[4] == 3 - iPersonInTurn)
				{
					iValue = MianYi;
				}
				else if (TwoSidesColor[4] == iPersonInTurn)
				{
					if (TwoSidesColor[6] == iPersonInTurn)
					{
						iValue = MianSan;
					}
					else if (TwoSidesColor[6] == 3 - iPersonInTurn)
					{
						iValue = MianYi;
					}
					else
					{
						iValue = MianEr;
					}

				}
				else
				{
					if (TwoSidesColor[6] == iPersonInTurn)
					{
						iValue = MianYi;
					}
					else if (TwoSidesColor[6] == 3 - iPersonInTurn)
					{
						iValue = MianYi;
					}
					else
					{
						iValue = MianYi;
					}
				}
			}
			else
			{
				if (TwoSidesColor[4] == 3 - iPersonInTurn&&TwoSidesColor[5] == 3 - iPersonInTurn)
				{
					iValue = MianYi;
				}
				else if (TwoSidesColor[4] == 3 - iPersonInTurn&&TwoSidesColor[5] == 0)
				{
					if (TwoSidesColor[7] == 3 - iPersonInTurn)
					{
						iValue = MianYi;
					}
					else if (TwoSidesColor[7] == iPersonInTurn)
					{
						iValue = MianYi;
					}
					else
					{
						iValue = MianYi;
					}
				}
				else if (TwoSidesColor[5] == 3 - iPersonInTurn&&TwoSidesColor[4] == 0)
				{
					if (TwoSidesColor[6] == 3 - iPersonInTurn)
					{
						iValue = MianYi;
					}
					else if (TwoSidesColor[6] == iPersonInTurn)
					{
						iValue = MianYi;
					}
					else
					{
						iValue = MianYi;
					}
				}
				else
				{
					if (TwoSidesColor[6] == iPersonInTurn || TwoSidesColor[7] == iPersonInTurn)
					{
						iValue = MianYi;
					}
					else
					{
						iValue = MianYi;
					}
				}
			}
		}

	}
	return iValue;
}

EXP int AiDecision(int x, int y, int iPosition[15][15], int iPersonInTurn, HMODULE hSelf)
{
	/*	int iNumx = MianYi;
	int iNumy = MianYi;
	int iNums = MianYi;
	int iNumt = MianYi;*/
	typedef int(*tFuncpReturnFourInt)(int, int, int (*)[15] , int);
	typedef int(*tFuncpAiCheck)(int, int, int (*)[15], int, int, int *);

	tFuncpReturnFourInt funcpxCount = (tFuncpReturnFourInt)GetProcAddress(hSelf, "xCount");
	tFuncpReturnFourInt funcpyCount = (tFuncpReturnFourInt)GetProcAddress(hSelf, "yCount");
	tFuncpReturnFourInt funcpsCount = (tFuncpReturnFourInt)GetProcAddress(hSelf, "sCount");
	tFuncpReturnFourInt funcptCount = (tFuncpReturnFourInt)GetProcAddress(hSelf, "tCount");

	tFuncpReturnFourInt funcpxAiCheckTwoSides = (tFuncpReturnFourInt)GetProcAddress(hSelf, "xAiCheckTwoSides");
	tFuncpReturnFourInt funcpyAiCheckTwoSides = (tFuncpReturnFourInt)GetProcAddress(hSelf, "yAiCheckTwoSides");
	tFuncpReturnFourInt funcpsAiCheckTwoSides = (tFuncpReturnFourInt)GetProcAddress(hSelf, "sAiCheckTwoSides");
	tFuncpReturnFourInt funcptAiCheckTwoSides = (tFuncpReturnFourInt)GetProcAddress(hSelf, "tAiCheckTwoSides");
	
	tFuncpAiCheck funcpAiCheck = (tFuncpAiCheck)GetProcAddress(hSelf, "AiCheck");

	(*funcpxCount)(x, y, iPosition, iPersonInTurn);
	(*funcpyCount)(x, y, iPosition, iPersonInTurn);
	(*funcpsCount)(x, y, iPosition, iPersonInTurn);
	(*funcptCount)(x, y, iPosition, iPersonInTurn);

	(*funcpxAiCheckTwoSides)(x, y, iPosition, iPersonInTurn);
	(*funcpyAiCheckTwoSides)(x, y, iPosition, iPersonInTurn);
	(*funcpsAiCheckTwoSides)(x, y, iPosition, iPersonInTurn);
	(*funcptAiCheckTwoSides)(x, y, iPosition, iPersonInTurn);

	iPValue = (*funcpAiCheck)(x, y, iPosition, iPersonInTurn, (*funcpxCount)(x, y, iPosition, iPersonInTurn), xTwoSidesColor)
		+ (*funcpAiCheck)(x, y, iPosition, iPersonInTurn, (*funcpyCount)(x, y, iPosition, iPersonInTurn), yTwoSidesColor)
		+ (*funcpAiCheck)(x, y, iPosition, iPersonInTurn, (*funcpsCount)(x, y, iPosition, iPersonInTurn), sTwoSidesColor)
		+ (*funcpAiCheck)(x, y, iPosition, iPersonInTurn, (*funcptCount)(x, y, iPosition, iPersonInTurn), tTwoSidesColor);

	return iPValue; // 针对单方计分的总分
}

EXP place API_Main(int iPosition[15][15], int Person, HMODULE hSelf)
{
	// Person的记录标准是1和2，故主函数调用传值应当加1

	// 定义返回值
	place Place;
	Place.x = 0;
	Place.y = 0;

	typedef int(*tFuncpAiDecision)(int, int, int (*)[15], int, HMODULE);
	tFuncpAiDecision funcpAiDecision = (tFuncpAiDecision)GetProcAddress(hSelf, "AiDecision");

	int MAXScore = -1;
	place *p = NULL;
	int k = 0;

	const int iPositionValue[15][15] = {
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
		{ 0,1,2,2,2,2,2,2,2,2,2,2,2,1,0 },
		{ 0,1,2,3,3,3,3,3,3,3,3,3,2,1,0 },
		{ 0,1,2,3,4,4,4,4,4,4,4,3,2,1,0 },
		{ 0,1,2,3,4,5,5,5,5,5,4,3,2,1,0 },
		{ 0,1,2,3,4,5,6,6,6,5,4,3,2,1,0 },
		{ 0,1,2,3,4,5,6,7,6,5,4,3,2,1,0 },
		{ 0,1,2,3,4,5,6,6,6,5,4,3,2,1,0 },
		{ 0,1,2,3,4,5,5,5,5,5,4,3,2,1,0 },
		{ 0,1,2,3,4,4,4,4,4,4,4,3,2,1,0 },
		{ 0,1,2,3,3,3,3,3,3,3,3,3,2,1,0 },
		{ 0,1,2,2,2,2,2,2,2,2,2,2,2,1,0 },
		{ 0,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	};
	// 循环判断每一个位置计算总分
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (iPosition[i][j] == 0)
			{
				int CurrentScore = (*funcpAiDecision)(i, j, iPosition, Person, hSelf) + (*funcpAiDecision)(i, j, iPosition, 3 - Person, hSelf) / 2 + iPositionValue[i][j];
				if (CurrentScore == MAXScore)
				{
					p = (place *)realloc(p, sizeof(place)*(k+1));
					p[k].x = i;
					p[k].y = j;
					k++;
				}
				else if (CurrentScore > MAXScore)
				{
					if (p != NULL) free(p);
					p = NULL;
					k = 0;
					p = (place *)malloc(sizeof(place)*(k + 1));
					p[k].x = i;
					p[k].y = j;
					k++;
					MAXScore = CurrentScore;
				}
			}
		}
	}

	// 从已有的存储最大分数点的序列中随机取一个作为落子点

	srand((int)clock());
	int choice = rand() % k;
	Place = p[choice];
	free(p);

	return Place;
}
