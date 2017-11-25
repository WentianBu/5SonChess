#include "keyboard.h"

#if USE_JSON_PROFILE==0
int GetKey()
{
	char key1, key2;
	rewind(stdin);
	key1 = _getch();

	if (key1 == ' '&&SPACE_CONFRM == 1) return CONFRM;
	if (key1 == '\r'&&ENTER_CONFRM == 1) return CONFRM;

	if (key1 == 27 && ESC_EXIT) return EXIT;

	if ((key1 == 'h' || key1 == 'H') && H_REGRET == 1) return REGRET;
	if (key1 == '\b'&&BKSPC_REGRET == 1) return REGRET;

	if (WASD_MOVE == 1)
	{
		switch (key1)
		{
		case 'w':case 'W': return MOVE_UP;
		case 'a':case 'A': return MOVE_LEFT;
		case 's':case 'S': return MOVE_DOWN;
		case 'd':case 'D': return MOVE_RIGHT;
		}
	}

	if (key1 == -32 && UDLR_MOVE == 1)
	{
		key2 = _getch();
		switch (key2)
		{
		case 72: return MOVE_UP;
		case 80: return MOVE_DOWN;
		case 75: return MOVE_LEFT;
		case 77: return MOVE_RIGHT;
		}
	}

	return INVALID;
}


#endif // USE_JSON_PROFILE==0