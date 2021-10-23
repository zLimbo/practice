#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <ctime>

using namespace std;

// ö�٣�æµ�Ϳ���
enum STATUS { BUSY, FREE };

// ������Ŀ��(����ڵ�)����PartitionTable�ܷ���
class PartitionNode {
// ��Ԫ��ΪPartitionTable�ṩ����Ȩ��
friend class PartitionTable;
private:
	int _id;				// ID
	int _size;				// ��С(KB)
	int _startAddr;			// ��ʼ��ַ
	STATUS _status;		// ����
	PartitionNode *_front;	// ǰ���ڵ�
	PartitionNode *_next;	// ��̽ڵ�
	// ���캯��
	PartitionNode(int id, int size, int startAddr, STATUS status,
		PartitionNode *front, PartitionNode *next);
};


/* �����ڵ㹹�캯��
 *
 *
*/
PartitionNode::PartitionNode(int id, int size, int startAddr,
		STATUS status = FREE,
		PartitionNode *front = nullptr,
		PartitionNode *next = nullptr):
	_id(id), _size(size), _startAddr(startAddr), _status(status),
	_front(front), _next(next)  {	 }



// ������(ʵ��Ϊ�����ڵ㹹�ɵ�����ṹ)
class PartitionTable {
private:
	static int _assignId;		// ID
	static int assignId();		// �Զ�����ID
	int _minSize;				// ���ٷָ�Ĵ�С
	int _freeNum;				// ���з�����Ŀ
	int _busyNum;				// æµ������Ŀ
	PartitionNode *_head;		// ͷ���
	PartitionNode *_tail;			// β�ڵ�
	PartitionNode *_nfCurr; 		// ��¼ѭ���״���Ӧ�㷨

public:
	PartitionTable(int allSize, int minSize);	// ���캯��
	~PartitionTable();			// ��������
	// �������
	int partFirstFit(int reqSize);	// �״���Ӧ�㷨
	int partNextFit(int reqSize);	// ѭ���״���Ӧ�㷨
	int recycle(int id);			// �����㷨
	void show() const;			// ��ʾ
};


// ID �ԉ�����
int PartitionTable::_assignId = 0;
int PartitionTable::assignId() { return ++_assignId; }


/* ���캯��
 *  allSize���ܵ�ַ�ռ�Ĵ�С����������ɴ˷���
 *  minSize�����ٷָ�Ĵ�С
 *  _head �� _tail ָ�������շ���
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


/* ��������
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


/* �״���Ӧ�㷨
 * reqSize: ��������ռ��С��kb��
 *
*/
int PartitionTable::partFirstFit(int reqSize) {
	PartitionNode *curr = _head->_next;
	// �������������ĵ�һ���ڵ�
	while (curr != _tail && (curr->_size < reqSize || curr->_status == BUSY)) {
		curr = curr->_next;
	}
	// û������ķ���������-1����ʾ�����ڸ÷���
	if (curr == _tail) return -1;
	// ��ǰ�����Ƿ��и�
	if (_minSize <  curr->_size - reqSize ) {
		// ��ǰ�����ָ�
		// �����·����ڵ�����������
		int newStartAddr = curr->_startAddr + reqSize;
		int newSize = curr->_size - reqSize;
		PartitionNode *newPartition = new PartitionNode(
				assignId(), newSize, newStartAddr, FREE, curr, curr->_next);
		curr->_next->_front = newPartition;
		curr->_next = newPartition;
		// ���µ�ǰ������С
		curr->_size = reqSize;
	} else {
		--_freeNum;
	}
	// ��ǰ������״̬Ϊæµ
	curr->_status = BUSY;
	++_busyNum;
	// ���ص�ǰ�ڵ�
	return curr->_id;
}


/* ѭ���״���Ӧ�㷨
 * reqSize: ��������ռ��С��kb��
 *
*/
int PartitionTable::partNextFit(int reqSize) {
	// ���ҵ�ǰ�ڵ�
	_nfCurr = _nfCurr->_next;
	// ���������ĩβ����ص������׶�
	if (_nfCurr == _tail) _nfCurr = _head->_next;
	while (_nfCurr != _tail && (_nfCurr->_size < reqSize || _nfCurr->_status == BUSY)) {
		_nfCurr = _nfCurr->_next;
	}
	// û������ķ���������-1����ʾ�����ڸ÷���
	if (_nfCurr == _tail) {
		_nfCurr = _head;
		return -1;
	}
	// ��ǰ�����Ƿ��и�
	if (_minSize <  _nfCurr->_size - reqSize ) {
		// ��ǰ�����ָ�
		// �����·����ڵ�����������
		int newStartAddr = _nfCurr->_startAddr + reqSize;
		int newSize = _nfCurr->_size - reqSize;
		PartitionNode *newPartition = new PartitionNode(
				assignId(), newSize, newStartAddr, FREE, _nfCurr, _nfCurr->_next);
		_nfCurr->_next->_front = newPartition;
		_nfCurr->_next = newPartition;
		// ���µ�ǰ������С
		_nfCurr->_size = reqSize;
	} else {
		--_freeNum;
	}
	// ��ǰ������״̬Ϊæµ
	_nfCurr->_status = BUSY;
	++_busyNum;
	// ���ص�ǰ�ڵ�
	return _nfCurr->_id;
}


/* ���շ���
 * id: Ҫ���շ����ķ�����
 *
*/
int PartitionTable::recycle(int id) {
	PartitionNode *curr = _head->_next;
	// ���� ������Ϊ id �ķ���
	while (curr != _tail && curr->_id != id) {
		curr = curr->_next;
	}
	// δ�ҵ����˳�
	if (curr == _tail) return -1;
	// ���½�һ���½ڵ�
	int newId = assignId();
	int newSize = curr->_size;
	int newStartAddr = curr->_startAddr;
	// �½ڵ��ǰ���ͺ��
	PartitionNode *front = curr->_front;
	PartitionNode *next = curr->_next;
	// ǰ���������У���ϲ�
	while (front->_status == FREE) {
		newSize += curr->_front->_size;
		newStartAddr = curr->_front->_startAddr;
		front = front->_front;
		delete front->_next;
		front->_next = nullptr;
		--_freeNum;
	}
	// ��̽ڵ���У���ϲ�
	while (next->_status == FREE) {
		newSize += curr->_next->_size;
		next = next->_next;
		if (next->_front) {
			delete next->_front;
		}
		--_freeNum;
	}
	// �����ɽڵ�
	delete curr;
	--_busyNum;
	// �����½ڵ�
	curr = new PartitionNode(
			newId, newSize, newStartAddr, FREE, front, next);
	front->_next = curr;
	next->_front = curr;
	++_freeNum;
	return curr->_id;
}

/* ��ʾ���̱�
 *
 *
*/
void PartitionTable::show() const {
	PartitionNode *curr = _head->_next;
	printf("\n���ܷ��� %d �������з��� %d ����æµ���� %d ����\n",
			_freeNum+_busyNum, _freeNum, _busyNum);
	printf("\n����ǰѭ������ָ��ָ����� %d, ", _nfCurr->_id);
	printf("��С������СΪ %d��\n", _minSize);
	printf("-------------------------------------------------------------------\n");
	printf("%8s%20s%20s%18s\n", "������", "������С(KB)", "����ʼַ(K)", "״̬\n");
	while (curr != _tail) {
		printf("%5d%20d%20d", curr->_id, curr->_size, curr->_startAddr);
		printf("%20s\n", (curr->_status == FREE ? "����" : "æµ"));
		curr = curr->_next;
	}
	printf("-------------------------------------------------------------------\n");
}


/*
 * ���Ժ���
 *
*/
int main() {
	// ����������,�ܵ�ַ��СΪ1024KB,��С�и��СΪ12
	PartitionTable pT(10240, 12);
	pT.show();
	//srand(unsigned(time(0)));
	vector<int> busyPartition;
	cout << "����������:" << endl;
	for (int i = 0; i < 1000; ++i) {
		if (rand() % 3 != 0) {
			int reqSize = rand() % 100 + 1;
			int partitionId = pT.partNextFit(reqSize);
			if (partitionId != -1) {
				printf("��÷��� id = %d, ��СΪ %d\n", partitionId, reqSize);
				busyPartition.push_back(partitionId);
			}
		} else if (!busyPartition.empty()){
			int i = rand() % busyPartition.size();
			pT.recycle(busyPartition[i]);
			printf("���շ��� id = %d\n", busyPartition[i]);
			busyPartition.erase(busyPartition.begin()+i);
		}
	}
	pT.show();
	while (true) {
		int partitionNo;
		int reqSize;
		int reqKind;
		cout << "��1 �״���Ӧ�㷨������Դ��\n"
				"��2 ѭ���״���Ӧ�㷨������Դ��\n"
				"��3 ������Դ��" << endl;
		cout << "�����룺" ;
		cin >> reqKind;
		if (reqKind == 1 || reqKind == 2) {
			cout << "������Դ��С��";
			cin >> reqSize;
			partitionNo = (reqKind == 1 ? pT.partFirstFit(reqSize) : pT.partNextFit(reqSize));

			if (partitionNo == -1) {
			cout << "����ʧ�ܣ�û����ô��С�ķ�����" << endl;
			} else {
				cout << "����ɹ���������Ϊ" << partitionNo << endl;
			}
		} else if (reqKind == 3) {
			cout << "���շ�����: ";
			cin >> partitionNo;
			if (pT.recycle(partitionNo) == -1) {
				cout << "����ʧ�ܣ��޸÷���" << endl;
			}
		}
		pT.show();
	}
	return 0;
}
