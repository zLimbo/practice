#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <ctime>

using namespace std;

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


/* 分区节点构造函数
 *
 *
*/
PartitionNode::PartitionNode(int id, int size, int startAddr,
		STATUS status = FREE,
		PartitionNode *front = nullptr,
		PartitionNode *next = nullptr):
	_id(id), _size(size), _startAddr(startAddr), _status(status),
	_front(front), _next(next)  {	 }



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
	int partFirstFit(int reqSize);	// 首次适应算法
	int partNextFit(int reqSize);	// 循环首次适应算法
	int recycle(int id);			// 回收算法
	void show() const;			// 显示
};


// ID 自増分配
int PartitionTable::_assignId = 0;
int PartitionTable::assignId() { return ++_assignId; }


/* 构造函数
 *  allSize：总地址空间的大小，后面分区由此分配
 *  minSize：不再分割的大小
 *  _head 和 _tail 指向两个空分区
*/
PartitionTable::PartitionTable(int allSize, int minSize):
	_minSize(minSize), _freeNum(1), _busyNum(0)
{
	_head = new PartitionNode(-1, 0, 0, BUSY);
	_tail = new PartitionNode(-1, 0, 0, BUSY);
	PartitionNode *partition0 = new PartitionNode(0, allSize, 0, FREE, _head, _tail);
	_head->_next = partition0;
	_tail->_front = partition0;
	_nfCurr = _head;
}


/* 析构函数
 *
 *
*/
PartitionTable::~PartitionTable() {
	PartitionNode *curr = _head->_next;
	while (curr != _tail) {
		_head->_next = curr->_next;
		delete curr;
		curr = _head->_next;
	}
	delete _head;
	delete _tail;
}


/* 首次适应算法
 * reqSize: 进程申请空间大小（kb）
 *
*/
int PartitionTable::partFirstFit(int reqSize) {
	PartitionNode *curr = _head->_next;
	// 查找满足条件的第一个节点
	while (curr != _tail && (curr->_size < reqSize || curr->_status == BUSY)) {
		curr = curr->_next;
	}
	// 没有满足的分区，返回-1，表示不存在该分区
	if (curr == _tail) return -1;
	// 当前分区是否切割
	if (_minSize <  curr->_size - reqSize ) {
		// 当前分区分割
		// 建立新分区节点插入分区表中
		int newStartAddr = curr->_startAddr + reqSize;
		int newSize = curr->_size - reqSize;
		PartitionNode *newPartition = new PartitionNode(
				assignId(), newSize, newStartAddr, FREE, curr, curr->_next);
		curr->_next->_front = newPartition;
		curr->_next = newPartition;
		// 更新当前分区大小
		curr->_size = reqSize;
	} else {
		--_freeNum;
	}
	// 当前分区表状态为忙碌
	curr->_status = BUSY;
	++_busyNum;
	// 返回当前节点
	return curr->_id;
}


/* 循环首次适应算法
 * reqSize: 进程申请空间大小（kb）
 *
*/
int PartitionTable::partNextFit(int reqSize) {
	// 查找当前节点
	_nfCurr = _nfCurr->_next;
	// 如果到链表末尾，则回到链表首端
	if (_nfCurr == _tail) _nfCurr = _head->_next;
	while (_nfCurr != _tail && (_nfCurr->_size < reqSize || _nfCurr->_status == BUSY)) {
		_nfCurr = _nfCurr->_next;
	}
	// 没有满足的分区，返回-1，表示不存在该分区
	if (_nfCurr == _tail) {
		_nfCurr = _head;
		return -1;
	}
	// 当前分区是否切割
	if (_minSize <  _nfCurr->_size - reqSize ) {
		// 当前分区分割
		// 建立新分区节点插入分区表中
		int newStartAddr = _nfCurr->_startAddr + reqSize;
		int newSize = _nfCurr->_size - reqSize;
		PartitionNode *newPartition = new PartitionNode(
				assignId(), newSize, newStartAddr, FREE, _nfCurr, _nfCurr->_next);
		_nfCurr->_next->_front = newPartition;
		_nfCurr->_next = newPartition;
		// 更新当前分区大小
		_nfCurr->_size = reqSize;
	} else {
		--_freeNum;
	}
	// 当前分区表状态为忙碌
	_nfCurr->_status = BUSY;
	++_busyNum;
	// 返回当前节点
	return _nfCurr->_id;
}


/* 回收分区
 * id: 要回收分区的分区号
 *
*/
int PartitionTable::recycle(int id) {
	PartitionNode *curr = _head->_next;
	// 查找 分区号为 id 的分区
	while (curr != _tail && curr->_id != id) {
		curr = curr->_next;
	}
	// 未找到则退出
	if (curr == _tail) return -1;
	// 重新建一个新节点
	int newId = assignId();
	int newSize = curr->_size;
	int newStartAddr = curr->_startAddr;
	// 新节点的前驱和后继
	PartitionNode *front = curr->_front;
	PartitionNode *next = curr->_next;
	// 前驱分区空闲，则合并
	while (front->_status == FREE) {
		newSize += curr->_front->_size;
		newStartAddr = curr->_front->_startAddr;
		front = front->_front;
		delete front->_next;
		front->_next = nullptr;
		--_freeNum;
	}
	// 后继节点空闲，则合并
	while (next->_status == FREE) {
		newSize += curr->_next->_size;
		next = next->_next;
		if (next->_front) {
			delete next->_front;
		}
		--_freeNum;
	}
	// 析构旧节点
	delete curr;
	--_busyNum;
	// 创建新节点
	curr = new PartitionNode(
			newId, newSize, newStartAddr, FREE, front, next);
	front->_next = curr;
	next->_front = curr;
	++_freeNum;
	return curr->_id;
}

/* 显示进程表
 *
 *
*/
void PartitionTable::show() const {
	PartitionNode *curr = _head->_next;
	printf("\n【总分区 %d 个，空闲分区 %d 个，忙碌分区 %d 个】\n",
			_freeNum+_busyNum, _freeNum, _busyNum);
	printf("\n【当前循环辅助指针指向分区 %d, ", _nfCurr->_id);
	printf("最小分区大小为 %d】\n", _minSize);
	printf("-------------------------------------------------------------------\n");
	printf("%8s%20s%20s%18s\n", "分区号", "分区大小(KB)", "分区始址(K)", "状态\n");
	while (curr != _tail) {
		printf("%5d%20d%20d", curr->_id, curr->_size, curr->_startAddr);
		printf("%20s\n", (curr->_status == FREE ? "空闲" : "忙碌"));
		curr = curr->_next;
	}
	printf("-------------------------------------------------------------------\n");
}


/*
 * 测试函数
 *
*/
int main() {
	// 建立分区表,总地址大小为1024KB,最小切割大小为12
	PartitionTable pT(10240, 12);
	pT.show();
	//srand(unsigned(time(0)));
	vector<int> busyPartition;
	cout << "随机分配过程:" << endl;
	for (int i = 0; i < 1000; ++i) {
		if (rand() % 3 != 0) {
			int reqSize = rand() % 100 + 1;
			int partitionId = pT.partNextFit(reqSize);
			if (partitionId != -1) {
				printf("获得分区 id = %d, 大小为 %d\n", partitionId, reqSize);
				busyPartition.push_back(partitionId);
			}
		} else if (!busyPartition.empty()){
			int i = rand() % busyPartition.size();
			pT.recycle(busyPartition[i]);
			printf("回收分区 id = %d\n", busyPartition[i]);
			busyPartition.erase(busyPartition.begin()+i);
		}
	}
	pT.show();
	while (true) {
		int partitionNo;
		int reqSize;
		int reqKind;
		cout << "【1 首次适应算法分配资源】\n"
				"【2 循环首次适应算法分配资源】\n"
				"【3 回收资源】" << endl;
		cout << "请输入：" ;
		cin >> reqKind;
		if (reqKind == 1 || reqKind == 2) {
			cout << "申请资源大小：";
			cin >> reqSize;
			partitionNo = (reqKind == 1 ? pT.partFirstFit(reqSize) : pT.partNextFit(reqSize));

			if (partitionNo == -1) {
			cout << "申请失败，没有这么大小的分区快" << endl;
			} else {
				cout << "申请成功，分区号为" << partitionNo << endl;
			}
		} else if (reqKind == 3) {
			cout << "回收分区号: ";
			cin >> partitionNo;
			if (pT.recycle(partitionNo) == -1) {
				cout << "回收失败，无该分区" << endl;
			}
		}
		pT.show();
	}
	return 0;
}
