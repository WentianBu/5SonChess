#pragma once
// record.h
// 用于详细记录单局游戏的过程
// 支持悔棋和复盘功能，并可读取复盘

// TODO:
// 游戏中应该记录每一步的用时，尤其是针对AI。悔棋行为不会记录。
// AI的参数设置（应该预留，作为将来的接口）应该在每一步中记录，将来应该支持在局中更改AI的参数并且即时生效，回到游戏。
// AI的参数应该能够在棋盘旁边显示出来
// 复盘文件采用xml格式记录。调用第三方库libxml2实现。复盘文件的存储应该调用WIN32 API来可视化选择
// 主菜单应该添加复盘功能，读取文件再现复盘。复盘文件应该设置为只读。xml文件本身打开时应该是明文并具有可读性。
// 保存时应该先对记录棋局的数据进行散列，然后加密该散列值并记录在文件头。读取时解密该散列值，然后与当前记录的散列值进行比对，如果复盘被更改过则应给出警告。
// 由于是开源的程序，因此每次保存复盘时应该读取或生成一对密钥（公钥和私钥）。散列值通过私钥加密，然后记录在文件头。公钥也会被一同记录在文件头。
// 记录统计调试模式的特殊文件：统计调试模式的文件应该详细记录每一局每一步的操作，文件头注明模式

// 目前先在dev分支开发悔棋支持，然后合并到主分支。将来开发复盘功能时再扩展结构


// 棋子颜色类型
enum ChessType
{
	Black = 0,
	White = 1,
};

// 玩家类型（AI/玩家）
enum PlayerType
{
	Player = 0,
	AI = 1,
};

// 记录每步数据的结构体
struct StepRecord
{
	int Round;
	enum ChessType Chess;
	enum PlayerType Player;
	place Place;
};

/*说明：
 *由于棋局本身的特点，链表的结点变化只会依次进行，不会出现跳跃式的更改
 *因此本链表不提供查找定位函数，也不提供定点插入和删除函数
 *对链表的修改只能在链表尾部进行
 */

// 用于双向非循环链表的结构体
// 头结点存放数据
typedef struct ChainNode
{
	unsigned int ID; // 结点ID，从1开始存储数据，0为头结点
	struct StepRecord step;
	struct ChainNode *pre, *next;
}ChainNode;

// 创建双向链表（头结点的指针均置空）
// 传入指向“指向ChainNode结点的指针”的指针
// 返回值是errno_t型，0代表成功，非0代表出错
errno_t CreateRecordChain(ChainNode **pp);

// 删除双向链表
errno_t DeleteRecordChain(ChainNode **pp);

// 记录一步棋并添加到链表
// 若返回非0则代表添加失败，本局游戏应当中止
errno_t RecordStep(ChainNode ** pp, int Round, enum PlayerType pt, unsigned int player, place Place);

// 悔一步棋并将悔棋的链表结点删除
// 注意：返回值是错误码，悔的棋的位置通过指针传回
// 若返回值不为0代表已经无棋可悔
errno_t RegretStep(ChainNode ** pp, place * pPlace);
