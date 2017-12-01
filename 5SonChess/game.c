#include "stdafx.h"
#include "game.h"

// 以下定义静态全局变量，这些变量仅供单局游戏使用，且仅在本模块中起效

// 记录当前棋盘状态，0为空，1为黑方，2为白方
static int Chess[CHESSBORAD_LINES][CHESSBOARD_WIDTH];

//当前选手（黑方为0，白方为1）
// 说明：在单局游戏中，仅通过黑方和白方来标识双方玩家。AI和人类玩家本质上还是通过各自的接口操作，只不过与主程序交互的方式不太一样
static unsigned int CurrentPlayer;

//当前光标指示的位置
static place CursorPlace;

//记录棋步的链表的指针
static ChainNode *pNode;


// 执行单局游戏的初始化操作
// 返回非0值则初始化操作失败，应该销毁当前游戏并返回主菜单
static errno_t InitiateGame(char ** Buffer, char ** OldBuffer)
{
	for (int i = 0; i < CHESSBORAD_LINES; i++)
	{
		for (int j = 0; j < CHESSBOARD_WIDTH; j++)
		{
			Chess[i][j] = 0;
		}
	} // 注意：静态变量，这里必须用循环强行初始化。如果怕麻烦这里不初始化，只在定义时初始化，第二局会出bug 
	CurrentPlayer = 0;
	pNode = NULL;
	int Winner = 3;

	// 创建记录棋局的链表

	errno_t err = CreateRecordChain(&pNode);
	if (err != 0)
	{
		MessageBox(NULL, _T("Fatal error 0x00001: 创建记录链表失败。"), _T("Error"), MB_ICONERROR | MB_SETFOREGROUND);
		return 1;
	} //处理错误，如果出现则返回错误码并弹出消息框警告

	InitiateBuffer(Buffer);
	DrawBlankChessboard(Buffer);
	CursorPlace.x = CursorPlace.y = 7; // 光标初始位于棋盘中心点（棋盘坐标0~14）
	DrawCursor(CursorPlace, Buffer);
	RefreshScreen(OldBuffer, Buffer);
	return 0;
}

/* 单步管理器
 * 负责管理主程序与玩家或者AI的接口，处理操作
 * 返回值表示操作类型：0代表落子某点，此时落子位置通过指针*Place传出；1代表悔棋，由游戏管理器根据当前模式处理，2代表玩家结束游戏
 * 只负责在移动光标时绘制光标，棋子样式的绘制由游戏管理器处理
 */
static int StepManager(char **Buffer, char **OldBuffer, AIMark PlayerMark, place *pPlace)
{
	if (PlayerMark.AI_Type == 0) //玩家落子
	{
		rewind(stdin);
		int key = INVALID;
		do
		{
			rewind(stdin);
			key = GetKey();
			switch (key)
			{
			case MOVE_UP:
			{
				CleanCursor(CursorPlace, Buffer);
				CursorPlace.x--;
				break;
			}
			case MOVE_DOWN:
			{
				CleanCursor(CursorPlace, Buffer);
				CursorPlace.x++;
				break;
			}
			case MOVE_LEFT:
			{
				CleanCursor(CursorPlace, Buffer);
				CursorPlace.y--;
				break;
			}
			case MOVE_RIGHT:
			{
				CleanCursor(CursorPlace, Buffer);
				CursorPlace.y++;
				break;
			}
			case CONFRM:
			{
				if (Chess[CursorPlace.x][CursorPlace.y] == 0)
				{
					// 当前棋盘位置无棋子，正常完成落子操作
					*pPlace = CursorPlace;
					return 0;
				}
				else
				{
					// 当前位置有棋子，落子失败继续循环
					break;
				}

			}
			case REGRET: return 1;
			case EXIT: return 2;
			default:
				break;
			}
			// 光标移出棋盘时的循环
			if (CursorPlace.y == -1) CursorPlace.y = CHESSBOARD_WIDTH - 1;
			if (CursorPlace.y == CHESSBOARD_WIDTH) CursorPlace.y = 0;
			if (CursorPlace.x == -1) CursorPlace.x = CHESSBORAD_LINES - 1;
			if (CursorPlace.x == CHESSBORAD_LINES) CursorPlace.x = 0;
			// 绘制光标
			DrawCursor(CursorPlace, Buffer);
			RefreshScreen(OldBuffer, Buffer);
		} while (1); //用户操作如果是有效的落子、悔棋或者退出，则已经return；能够执行到这里，直接继续循环判断即可
	}
	else // AI落子
	{
		if (PlayerMark.AI_Type == 1)
		{
			tFuncpAI_Zhang funcpAI_Zhang = (tFuncpAI_Zhang)GetProcAddress(PlayerMark.hLib, "API_Main");
			*pPlace = (*funcpAI_Zhang)(Chess, CurrentPlayer + 1);
		}
		// TODO: 在这里添加if分支或者完全改成switch语句来添加对不同API的支持

		CleanCursor(CursorPlace, Buffer);
		CursorPlace = *pPlace;
		DrawCursor(CursorPlace, Buffer);
		RefreshScreen(OldBuffer, Buffer);
		return 0;
	}
}

/* 单局游戏管理器
* 负责单局游戏的管理：初始化游戏，授权玩家操作，处理操作，显示内容管理，调用函数记录棋局和链表管理，判定胜负
* 返回胜利方（黑方为0，白方为1，平局为2，负值均为错误码，未分出胜负时Winner变量应该为3，玩家退出则返回4）
* 对mode的定义：
* PVP模式：1
* PVE模式：2 -（玩家执黑），3 -（AI执黑）
* EVE模式：4 -（单步），5 -（慢速），6 -（中速），7 -（快速），8 -（一次性）
* 说明：统计调试模式由主程序负责处理，这种情况下主程序传入mode值为8
*/
int GameManager(int mode, AIMark Player1, AIMark Player2, char **Buffer, char **OldBuffer)
{
	errno_t error = InitiateGame(Buffer, OldBuffer); //初始化游戏
	if (error != 0) return -1; //错误码-1代表初始化游戏失败
	// operate是单步管理器返回的操作，初始值为-1
	int Winner = 3, Round = 1, operate = -1;
	place Place;
	while (Winner == 3 && Round <= MAX_ROUND_NUMBER)
	{
		operate = -1;
		// 从单步管理器获取当前授权玩家的操作
		if (CurrentPlayer == 0)
			operate = StepManager(Buffer, OldBuffer, Player1, &Place);
		else
			operate = StepManager(Buffer, OldBuffer, Player2, &Place);
		// 处理操作
		switch (operate)
		{
		case 0: //有效落子操作
		{
			Chess[Place.x][Place.y] = CurrentPlayer + 1;
			*(Buffer + BUFFER_WIDTH * (2 * CursorPlace.x + 1) + (2 * CursorPlace.y + 1) + CENTER_OFFSET) = CurrentPlayer ? "○" : "●";
			RefreshScreen(OldBuffer, Buffer);
			//向链表添加记录
			error = RecordStep(&pNode, Round, 0, CurrentPlayer, Place);
			if (error != 0)
			{
				MessageBox(NULL, _T("Fatal error 0x00002: 记录棋步失败。"), _T("Error"), MB_ICONERROR | MB_SETFOREGROUND);
				DeleteRecordChain(&pNode);
				return -2;
			} //错误码-2代表添加记录失败
			Winner = Check(CursorPlace.x, CursorPlace.y, CurrentPlayer, Chess[0]);
			// 这是个历史遗留问题，目前采用的裁判函数如果Winner=-1代表无人获胜
			if (Winner == -1) Winner = 3;
			Round++;
			CurrentPlayer = !CurrentPlayer;
			// 处理EVE时定义的间隔时间
			switch (mode)
			{
			case 4: _getch(); break;
			case 5: Sleep(1000); break;
			case 6: Sleep(500); break;
			case 7: Sleep(200); break;
			default:
				break;// 其他情况都是立即进行下一步处理
			}
			break;
		}
		case 1: // 悔棋操作
		{
			place LastPlace;
			// 试图从链表中取出上一步的位置
			error = RegretStep(&pNode, &LastPlace);
			if (error != 0)
			{
				MessageBox(NULL, _T("都悔到头了，有意思吗？"), _T("无棋可悔"), MB_ICONWARNING | MB_SETFOREGROUND);
				break;
			} //处理无棋可悔的情况，如果出现则弹出消息框警告，跳出switch判断，不进行任何操作
			else
			{
				Chess[LastPlace.x][LastPlace.y] = 0;
				Round--;//回合数减1
				CurrentPlayer = !CurrentPlayer;
				CleanCursor(CursorPlace, Buffer);
				CursorPlace = LastPlace;
				DrawCursor(CursorPlace, Buffer);
				RestoreStyle(Buffer, LastPlace);
				RefreshScreen(OldBuffer, Buffer);
			}
			if (mode == 2 || mode == 3) // 如果PVE模式下，则悔棋应该一次悔两步
			{
				error = RegretStep(&pNode, &LastPlace);
				if (error != 0)
				{
					MessageBox(NULL, _T("都悔到头了，有意思吗？"), _T("无棋可悔"), MB_ICONWARNING | MB_SETFOREGROUND);
					break;
				}
				else
				{
					Chess[LastPlace.x][LastPlace.y] = 0;
					Round--;
					CurrentPlayer = !CurrentPlayer;
					CleanCursor(CursorPlace, Buffer);
					CursorPlace = LastPlace;
					DrawCursor(CursorPlace, Buffer);
					RestoreStyle(Buffer, LastPlace);
					RefreshScreen(OldBuffer, Buffer);
				}
			}
			break;
		}
		case 2:
		{
			DeleteRecordChain(&pNode);
			return 4; // 玩家选择退出游戏
		}
		default:
			break;
		}
	}
	// 退出while循环代表游戏正常结束，需要返回胜利情况
	DeleteRecordChain(&pNode);
	return Winner;
}