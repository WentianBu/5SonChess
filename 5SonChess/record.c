#include "record.h"

// 创建存储游戏数据的双向链表
// 注意：pp是 指向 “指向ChainNode型结点的指针” 的指针
errno_t CreatRecordChain(ChainNode **pp)
{
	// 申请结点内存空间
	(*pp) = (ChainNode *)malloc(sizeof(ChainNode));
	if ((*pp) == NULL) return 1; //错误处理
	// 初始化该结点
	(*pp)->ID = 1;
	(*pp)->pre = NULL; //前指针置空
	(*pp)->next = NULL; //后面没有结点，后一指针置空
	return 0;
}

// 在链表尾部添加结点
// 链表位置指针将指向新的尾结点
errno_t AddRecordNode(ChainNode ** pp)
{
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
