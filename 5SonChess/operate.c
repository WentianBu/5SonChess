#include "stdafx.h"
#include "operate.h"


USER_OPERATE GetOperate(place CursorPlace,char **Buffer,char **OldBuffer)
{
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	INPUT_RECORD input;
	DWORD res; // 接受ReadConsoleInput最后一个指针的值
#ifdef MOUSE
	COORD pos; // 鼠标所在的缓冲器位坐标
#endif // MOUSE
	// CurtPlc是鼠标当前所指位置相对棋盘的坐标，鼠标不在棋盘上时其值无效。
	place CurtPlc = { -1,-1 };
	USER_OPERATE Operate = { INVALID,{-1,-1} };
	// place CursorPlace = *pCursorPlace;

	rewind(stdin);
	while (1)
	{
		ReadConsoleInput(hIn, &input, 1, &res);
	
		//以下为鼠标操作部分，为了调试方便已经屏蔽该功能。
#ifdef MOUSE
		pos = input.Event.MouseEvent.dwMousePosition;

#ifdef DEBUG
		gotoxy(2 * CENTER_OFFSET, BUFFER_LINES - 1);
		printf("X = %4d, Y = %4d", (int)pos.X, (int)pos.Y);
#endif // DEBUG

		// 算法进行识别，计算出鼠标所指位置相对于棋盘数组的坐标
		if (pos.X >= 31 && pos.X <= 89 && pos.Y >= 1 && pos.Y <= 30) // 鼠标处于识别区域内
		{
			CurtPlc.x = (int)((pos.Y - 1) / 2);
			CurtPlc.y = (int)((pos.X - 31) / 4);
			// 如果CurtPlc相对CursorPlace发生了变化，且仍在识别区内，则重绘光标
			if (CurtPlc.x != CursorPlace.x || CurtPlc.y != CursorPlace.y)
			{
				CleanCursor(CursorPlace,Buffer);
				CursorPlace = CurtPlc;
				DrawCursor(CursorPlace, Buffer);
				RefreshScreen(OldBuffer, Buffer);
			}
		}
		else
		{
			CurtPlc.x = CurtPlc.y = -1;
		}

#ifdef DEBUG
		gotoxy(2 * CENTER_OFFSET, BUFFER_LINES);
		printf("X = %4d, Y = %4d", CurtPlc.x, CurtPlc.y);
#endif // DEBUG
		
		if (input.EventType == MOUSE_EVENT)    //如果当前为鼠标事件  
		{
			if (input.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) //左键按下
			{
				Operate.type = CONFRM;
				Operate.cPlace = CursorPlace;

#ifdef DEBUG
		gotoxy(2 * CENTER_OFFSET, BUFFER_LINES+1);
		printf("落子");
#endif // DEBUG

				return Operate;
			}

			if (input.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) //右键按下
			{
				Operate.type = REGRET;

#ifdef DEBUG
		gotoxy(2 * CENTER_OFFSET, BUFFER_LINES+1);
		printf("悔棋");
#endif // DEBUG
				return Operate;
			}
		}
#endif // MOUSE

		
		if (input.EventType == KEY_EVENT&&input.Event.KeyEvent.bKeyDown == TRUE) //如果是键盘事件
		{
			WORD keyCode = input.Event.KeyEvent.wVirtualKeyCode;

			if (keyCode == VK_SPACE&&SPACE_CONFRM)
			{
				Operate.type = CONFRM;
				Operate.cPlace = CursorPlace;
				return Operate;
			}
			if (keyCode == VK_RETURN&&ENTER_CONFRM)
			{
				Operate.type = CONFRM;
				Operate.cPlace = CursorPlace;
				return Operate;
			}

			if (keyCode == VK_ESCAPE && ESC_EXIT)
			{
				Operate.type = EXIT;
				return Operate;
			}
			if (keyCode == 72 && H_REGRET)
			{
				Operate.type = REGRET;
				Operate.cPlace = CursorPlace;
				return Operate;
			}
			if (keyCode == VK_BACK&&BKSPC_REGRET) 
			{
				Operate.type = REGRET;
				Operate.cPlace = CursorPlace;
				return Operate;
			}


			if (WASD_MOVE)
			{
				switch (keyCode)
				{
				case 87: // W键
				{
					CleanCursor(CursorPlace, Buffer);
					CursorPlace.x--;
					break;
				}
				case 65: // A键
				{
					CleanCursor(CursorPlace, Buffer);
					CursorPlace.y--;
					break;
				}
				case 83: // S键
				{
					CleanCursor(CursorPlace, Buffer);
					CursorPlace.x++;
					break;
				}
				case 68: // D键
				{
					CleanCursor(CursorPlace, Buffer);
					CursorPlace.y++;
					break;
				}
				}
			}
			
			if (UDLR_MOVE == 1)
			{
				switch (keyCode)
				{
				case VK_UP:
				{
					CleanCursor(CursorPlace, Buffer);
					CursorPlace.x--;
					break;
				}
				case VK_DOWN:
				{
					CleanCursor(CursorPlace, Buffer);
					CursorPlace.x++;
					break;
				}
				case VK_LEFT:
				{
					CleanCursor(CursorPlace, Buffer);
					CursorPlace.y--;
					break;
				}
				case VK_RIGHT:
				{
					CleanCursor(CursorPlace, Buffer);
					CursorPlace.y++;
					break;
				}
				}
				
			}

			if (CursorPlace.y == -1) CursorPlace.y = CHESSBOARD_WIDTH - 1;
			if (CursorPlace.y == CHESSBOARD_WIDTH) CursorPlace.y = 0;
			if (CursorPlace.x == -1) CursorPlace.x = CHESSBORAD_LINES - 1;
			if (CursorPlace.x == CHESSBORAD_LINES) CursorPlace.x = 0;
			DrawCursor(CursorPlace, Buffer);
			RefreshScreen(OldBuffer, Buffer);
		}
	}
	Operate.type = INVALID;
	return Operate;
}


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
		case 'w':case 'W':return MOVE_UP;
		case 'a':case 'A':return MOVE_LEFT;
		case 's':case 'S':return MOVE_DOWN;
		case 'd':case 'D':return MOVE_RIGHT;
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
