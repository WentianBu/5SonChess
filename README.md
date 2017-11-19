# 5SonChess
This is a **gobang** C program with AI. This program is the big assignment of UCAS C Program Language Course.

----------------------------------------------------------------

# 五子棋
这是一个带有AI的C语言**五子棋**程序，按照C99标准编写，作为UCAS的C语言大作业。

## 特性
1. 键盘操作交互，大部分情况可以通过方向键来选择选项和移动棋盘光标，无需输入坐标或者选项编号，操作方式更加友好。

2. 完全改变了传统的cls刷新方式。显示引擎通过缓冲器读写、新旧缓冲器对比和定点覆盖输出的方式避免了频繁使用cls命令导致的屏幕闪烁，并提高了程序的运行速度。
*注：传统方法使用cls进行屏幕刷新时，如果改变的内容不多而刷新频率较高，则很容易出现闪烁的现象，影响视觉体验。同时，如果显示内容需要经过较为复杂的运算，反复清屏刷新会耗费较长运行时间，影响程序效率，并可能出现显示加载到一半的不美观画面。*

3. AI模块采用dll动态链接库的方式进行动态加载、显式链接，与主程序独立。方便使用不同方法、不同版本的AI算法进行调试。

4. 支持本地PVP、人机对战和双AI调试模式。双AI调试功能提供**调试统计**模式，可以令两个AI程序快速进行多局游戏并自动统计胜负情况，有利于对AI的棋力进行对比。也可以采用单局调试，对AI的每一步进行观看。

## 不足和开发目标
1. 显示引擎仍然使用ANSI方式处理中文和制表字符，因此略显过时和累赘。将来可能对显示引擎进行升级，完全采用新的架构，并做成一个更加通用的引擎。

2. 功能较为单一，仅支持按照本地标准编写的DLL AI程序。将来可能加入对弈心协议（Yixin-protocol）的支持，使之能够加载弈心引擎。
>Yixin protocol is a protocol derived from Gomocup protocol. It is designed because Gomocup protocol has some limitation. Firstly used by Yixin, Yixin protocol supports more commands than Gomocup protocol. Yixin protocol is fully implemented in Gomoku/Renju GUI Yixin Board.

关于弈心协议的更多内容详见[弈心协议的GitHub页面](https://github.com/accreator/Yixin-protocol)

3. 无法记录对局情况。将来可能加入读写文件的模块，添加保存复盘和读取复盘的功能。复盘文件的格式可能采用xml文件，便于读取和理解。

Wentian Bu

2017-11-09

-------------------------------------------------------
-------------------------------------------------------
# 2017.11.18 修改注记 (Version 1.0.1)

本次主要是少量显示内容的修改，同时思考了下一步开发计划。

--------------------------------------

## 本次修改内容

1. 根据课程提供的规范对五子棋界面程序进行了少量修改，棋盘添加了天元和星点标记，棋盘下部采用A~O、左侧采用1~15标明了坐标。

2. 主程序添加了查阅五子棋规则的功能。课程要求的五子棋为黑方禁手规则，就此编写了rules.html。

## 下一步开发目标

1. 解决最开始编写缓冲器模块时的遗留问题。

    最开始写缓冲器模块时，缓冲器大小固定为32*32。

        
        char *Buffer[32][32] = { NULL };
        char *OldBuffer[32][32] = { NULL };

    此外，很多显示信息的语句都是在编程时直接计算出值之后写入指定位置的缓冲器，例如：

    - 主程序界面的显示功能
    
        ```
        for (int i = 6; i < 26; i++) *(Buffer+160+i) = "——";
	    *(Buffer + 235) = "欢迎来到 五子荣耀！";
	    *(Buffer + 298) = "请使用上下方向键选择功能：";
	    *(Buffer + 397) = "1. 玩家对战";
	    *(Buffer + 461) = "2. 与AI对战";
	    *(Buffer + 525) = "3. 双AI调试";
	    *(Buffer + 589) = "4. 查看规则";
	    *(Buffer + 653) = "5. 退出游戏";
	    *(Buffer + 775) = "Powered By Wentian Bu    Version 1.0.1";
	    for (int i = 6; i < 26; i++) *(Buffer + 800 + i) = "——";
        ```

    - 绘制棋盘的核心模块

        ```
        // 绘制棋盘方格
		*(Buffer + 2 * i * 32 + 2 * j) = "　";
		*(Buffer + 2 * i * 32 + 2 * j + 1) = "│";
		*(Buffer + (2 * i + 1) * 32 + 2 * j) = "—";
		*(Buffer + (2 * i + 1) * 32 + 2 * j + 1) = "┼";

		// 绘制棋盘边界图样
		*(Buffer + 2 * i * 32 + 1) = "┃";
		*(Buffer + 2 * i * 32 + 29) = "┃";
		*(Buffer + (2 * i + 1) * 32 + 1) = "┠";
		*(Buffer + (2 * i + 1) * 32 + 29) = "┨";
		*(Buffer + 32 + 2 * j) = "━";
		*(Buffer + 928 + 2 * j) = "━";
		*(Buffer + 32 + 2 * j + 1) = "┯";
		*(Buffer + 928 + 2 * j + 1) = "┷";
        ```

        ```
        *(Buffer + 33) = "┏";
	    *(Buffer + 61) = "┓";
	    *(Buffer + 929) = "┗";
	    *(Buffer + 957) = "┛";
        ```
    - 刷新显示的核心函数

        ```
        for (int j = 0; j < 32; j++)
		{
			if (*(Buffer + i * 32 + j) != *(OldBuffer + i * 32 + j))
			{
				gotoxy(2*j,i);
				printf("%s", *(Buffer + i * 32 + j));
				*(OldBuffer + i * 32 + j) = *(Buffer + i * 32 + j);
			}
		}
        ```

    这些重要的核心功能模块都是预先计算好的数据，这直接导致了程序难以适应变化的需求。本次在添加了左侧数字坐标后明显觉得左侧空白不足，界面不够美观。但是由于写定的模块太多，修改极其麻烦。将来会对核心模块进行重写和扩充，使之能够适应可变的显示区域。

2. 对于AI算法的考虑：
    
    目前仍然计划棋型评分的做法。结合看过的一些代码，大致可以这样操作：

    1. 对棋盘进行处理，将各种棋子之间的关系转换成易于查找和搜索的向量表供AI使用。

        *注：该向量表每一步棋落子后进行更新，且一直保持到本局结束，这样可以大大减少全盘搜索棋型耗费的时间。*

    2. AI搜索向量表，并给每个位置评分：根据下在该位置所得的分数，进行综合决策。AI可以纵深搜索多步，对某个位置的优劣有更完善的评估。

        *注：为了节约运算时间，尽量减少重复的搜索计算过程，可在内存中保留多个版本的向量表，根据具体棋局走向进行选择保留。还可以综合利用剪枝算法等来减少分支，增加一定时间内的搜索效率。*

    3. 对禁手的理解和检测：检测禁手的有效方法是 **填子法** ，即在某点试着落子并根据定义检查是否存在禁手，尤其是假活三等看似会导致禁手的棋型很容易混淆。对禁手的处理麻烦在于多重禁手，但是实际下棋过程中概率极小，因此能够正确识别并处理两到三层禁手即可。

    4. 容错性：课程要求每一步棋最多计算15秒，超时判负。因此应该设计容错算法，即在15秒以内尽可能多地计算，但在其中任何时刻都应该有一个当前计算量下最佳的落子位置。

    5. 关于裁判函数：当前版本的裁判函数暂时借用了张宁鑫的函数，在各方向上检查连子数。将来完善向量表结构之后，会利用向量表来进行裁判，提高效率。


    Wentian Bu

    2017-11-18