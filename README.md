# 5SonChess
This is a **gobang** C program with AI. This program is the big assignment of UCAS C Program Language Course.



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
