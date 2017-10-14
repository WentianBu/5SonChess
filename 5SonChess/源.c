#include <conio.h>
#include <Windows.h>
#include <stdio.h>
#include "display.h"
#include "buffer.h"
#define BOARDMAX 15


int main()
{
	system("mode con cols=64 lines=32");
	char *Buffer[32][32] = { NULL };
	char *OldBuffer[32][32] = { NULL };
	
	InitiateBuffer(&Buffer[0][0]);

	RefreshScreen(OldBuffer[0], Buffer[0]);

	int x = 16, y = 16;
	Buffer[x-1][y-1] = "©³";
	Buffer[x-1][y+1] = "©·";
	Buffer[x+1][y-1] = "©»";
	Buffer[x+1][y+1] = "©¿";
	RefreshScreen(OldBuffer[0], Buffer[0]);
	char key1, key2;
	while (1)
	{
		rewind(stdin);
		key1 = _getch();
		if(key1==-32)
		Buffer[x - 1][y - 1] = "  ";
		Buffer[x - 1][y + 1] = "  ";
		Buffer[x + 1][y - 1] = "  ";
		Buffer[x + 1][y + 1] = "  ";
		key2 = _getch();
		//switch (key1)
		//{
		//case 224:
			switch (key2)
			{
			case 72: x--; break;
			case 80: x++; break;
			case 75: y--; break;
			case 77: y++; break;
			default: break;
			}
			Buffer[x - 1][y - 1] = "©³";
			Buffer[x - 1][y + 1] = "©·";
			Buffer[x + 1][y - 1] = "©»";
			Buffer[x + 1][y + 1] = "©¿";
			RefreshScreen(OldBuffer[0], Buffer[0]);
			//break;
		//default: break;
		//}
		
		
		/*
		switch (key)
		{
		case 'w': y--;
		case 's': y++;
		case 'a': x--;
		case 'd': x++;
		}
		Buffer[x - 1][y - 1] = "©³";
		Buffer[x - 1][y + 1] = "©·";
		Buffer[x + 1][y - 1] = "©»";
		Buffer[x + 1][y + 1] = "©¿";
		RefreshScreen(OldBuffer[0], Buffer[0]);
		*/
	}

	
	

	
	return 0;
}