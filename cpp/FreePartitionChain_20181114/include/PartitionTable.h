#ifndef PARTITIONTABLE_H
#define PARTITIONTABLE_H

/*
// 枚举：忙碌和空闲
enum STATUS { BUSY, FREE };

// 分区表目类(链表节点)，仅PartitionTable能访问
class PartitionNode {
// 友元，为PartitionTable提供访问权限
friend class PartitionTable;
private:
	int _id;				// ID
	int _size;				// 大小(KB)
	int _startAddr;			// 起始地址
	STATUS _status;		// 进程
	PartitionNode *_front;	// 前驱节点
	PartitionNode *_next;	// 后继节点
	// 构造函数
	PartitionNode(int id, int size, int startAddr, STATUS status,
		PartitionNode *front, PartitionNode *next);
};


// 分区表(实现为分区节点构成的链表结构)
class PartitionTable {
private:
	static int _assignId;		// ID
	static int assignId();		// 自动分配ID
	int _minSize;				// 不再分割的大小
	int _freeNum;				// 空闲分区数目
	int _busyNum;				// 忙碌分区数目
	PartitionNode *_head;		// 头结点
	PartitionNode *_tail;			// 尾节点
	PartitionNode *_nfCurr; 		// 记录循环首次适应算法

public:
	PartitionTable(int allSize, int minSize);	// 构造函数
	~PartitionTable();			// 析构函数
	// 插入分区
	void createNewPartitionNode(PartitionNode *curr, int reqSize);
	int partFirstFit(int reqSize);	// 首次适应算法
	int partNextFit(int reqSize);	// 循环首次适应算法
	int partBestFit(int reqSize);		// 最佳适应算法
	int partWorstFit(int reqSize);	// 最坏适应算法
	int recycle(int id);			// 回收算法
	void show() const;			// 显示
};
*/
#endif // PARTITIONTABLE_H
