#pragma once
// 临时调试用裁判函数
// 检查有无连成五子



int Check(int x, int y, int Person, int *iPosition)// 落子x，y后检查
{
	int numx = 1, numy = 1, nums = 1, numt = 1;
	int iPersonInTurn = Person + 1;
	int iWinner = 0; // 胜利者

	for (int i = 1; i <5; i++)//向下方向上检查
	{
		if (*(iPosition+15*(x + i)+y) == iPersonInTurn)
		{
			if (x + i >= 14)
			{
				break;
			}
			numx++;
		}
		else
			break;
	}
	for (int i = 1; i < 5; i++)//向上方向检查
	{
		if (*(iPosition+15*(x-i)+y) == iPersonInTurn)
		{
			if (x <= i)
			{
				break;
			}
			numx++;
		}
		else
			break;
	}
	for (int i = 1; i <5; i++)//向左方向上检查
	{


		if (*(iPosition+15*x+y-i) == iPersonInTurn)
		{
			if (y <= i)
			{
				break;
			}
			numy++;
		}
		else
			break;
	}
	for (int i = 1; i < 5; i++)//向右方向检查
	{


		if (*(iPosition+15*x+y+i) == iPersonInTurn)
		{
			if (y + i >= 14)
			{
				break;
			}
			numy++;
		}
		else
			break;
	}
	for (int i = 1; i < 5; i++)//向左上方向检查
	{


		if (*(iPosition+15*(x-i)+y-i) == iPersonInTurn)
		{
			if (x <= i)
			{
				break;
			}
			nums++;
		}
		else
			break;

	}
	for (int i = 1; i < 5; i++)//向右下方向检查
	{


		if (*(iPosition+15*(x+i)+y+i) == iPersonInTurn)
		{
			if (x + i >= 14)
			{
				break;
			}
			nums++;
		}
		else
			break;
	}
	for (int i = 1; i <5; i++)//在右上方向上检查
	{


		if (*(iPosition+15*(x-i)+y+i) == iPersonInTurn)
		{
			if (x <= i)
			{
				break;
			}
			else if (y + i >= 14)
			{
				break;
			}
			numt++;
		}
		else
			break;
	}
	for (int i = 1; i < 5; i++)//向左下方向检查
	{


		if (*(iPosition+15*(x+i)+y-i) == iPersonInTurn)
		{
			if (x + i >= 14)
			{
				break;
			}
			else if (y <= i)
			{
				break;
			}
			numt++;
		}
		else
			break;
	}
	if (numx >= 5 || numy >= 5 || nums >= 5 || numt >= 5)
	{
		iWinner = iPersonInTurn;
	}
	else
	{
		iWinner = 0;
	}
	return iWinner - 1;
}