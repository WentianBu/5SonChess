#include "stdafx.h"
#include "record.h"

// 创建存储游戏数据的双向链表
// 注意：pp是 指向 “指向ChainNode型结点的指针” 的指针
errno_t CreateRecordChain(ChainNode **pp)
{
	// 申请结点内存空间
	(*pp) = (ChainNode *)malloc(sizeof(ChainNode));
	if ((*pp) == NULL) return 1; //错误处理
	// 初始化该结点
	(*pp)->ID = 0;
	(*pp)->pre = NULL; //前指针置空
	(*pp)->next = NULL; //后面没有结点，后一指针置空
	return 0;
}

// 删除整个链表
errno_t DeleteRecordChain(ChainNode **pp)
{
	// 检查是否指向链表尾结点，若没有则将指针移到尾结点
	while ((*pp)->next != NULL)
		(*pp) = (*pp)->next;
	// 从尾结点开始逐个释放空间并将指针前移直到头结点
	ChainNode *q = (*pp)->pre;
	while (q != NULL)
	{
		free(*pp);
		(*pp) = q;
		q = (*pp)->pre;
	}
	free(*pp); //链表头结点已经被删除
	(*pp) = NULL; //链表指针置空以免出现野指针
	return 0;
}

// 在链表尾部添加结点
// 链表位置指针将指向新的尾结点
// 静态函数不能被外部访问
static errno_t AddRecordNode(ChainNode **pp)
{
	// 检查是否指向链表尾结点，若没有则将指针移到尾结点
	while ((*pp)->next != NULL)
		(*pp) = (*pp)->next;
	//申请内存空间
	ChainNode *q = (ChainNode *)malloc(sizeof(ChainNode));
	if (q == NULL) return 1; //申请出错返回1
	// 初始化结点
	q->ID = ((*pp)->ID) + 1; // 新结点的ID号+1
	(*pp)->next = q; //原尾结点的后指针指向新的结点
	q->pre = (*pp); // 新结点的前指针指向原尾结点
	q->next = NULL; // 新结点的后指针置空
	// 链表位置指针指向新的尾结点
	(*pp) = q;
	return 0;
}

// 从链表尾部删除一个结点
// 注意：头结点不存放数据，不能被该函数删除。试图删除头结点将会返回错误码2。删除头结点需要使用删除整个链表的函数。
// 链表位置指针将指向上一个结点
// 静态函数不能被外部访问
static errno_t DeleteRecordNode(ChainNode **pp)
{
	// 检查是否指向链表尾结点，若没有则将指针移到尾结点
	while ((*pp)->next != NULL)
		(*pp) = (*pp)->next;
	ChainNode *q = (*pp)->pre; //定义临时指针指向上一个结点
	if (q == NULL) return 2; //试图删除头结点返回错误码2
	free(*pp); // 释放被删除的结点所占的内存空间
	(*pp) = q; // 指针指向上一个结点
	(*pp)->next = NULL; // 上一个结点的后指针置空
	return 0;
}

// 记录一步棋并添加到链表
errno_t RecordStep(ChainNode **pp, int Round, enum PlayerType pt, unsigned int player, place Place)
{
	errno_t err = AddRecordNode(pp);// 链表添加结点
	if (err != 0) return 1;//添加结点失败，本局游戏应该中止
	// 指针已经指向新的结点，给结点赋值
	((*pp)->step).Round = Round;
	((*pp)->step).Player = pt;
	((*pp)->step).Chess = player;
	((*pp)->step).Place = Place;
	return 0;
}

// 悔一步棋并将悔棋的链表结点删除
// 注意：返回值是错误码，悔的棋的位置通过指针传回
errno_t RegretStep(ChainNode **pp, place *pPlace)
{
	// 检查指针是否在链表尾，若不是则移动到链表尾
	while ((*pp)->next != NULL)
		(*pp) = (*pp)->next;
	// 读取链表数据：上一步落子位置
	*pPlace = ((*pp)->step).Place;
	errno_t err = DeleteRecordNode(pp);
	if (err == 2) return 2; // 已经是头结点，无棋可悔
	return 0;
}
