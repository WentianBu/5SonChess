#pragma once


//��ʼ�������������������ո�
void InitiateBuffer(char **p)
{
	for (int i = 0; i < 1024; i++)
	{
		*p = "��";
		p++;
	}
}

