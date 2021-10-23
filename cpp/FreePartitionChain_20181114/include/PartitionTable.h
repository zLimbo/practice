#ifndef PARTITIONTABLE_H
#define PARTITIONTABLE_H

/*
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
	void createNewPartitionNode(PartitionNode *curr, int reqSize);
	int partFirstFit(int reqSize);	// �״���Ӧ�㷨
	int partNextFit(int reqSize);	// ѭ���״���Ӧ�㷨
	int partBestFit(int reqSize);		// �����Ӧ�㷨
	int partWorstFit(int reqSize);	// ���Ӧ�㷨
	int recycle(int id);			// �����㷨
	void show() const;			// ��ʾ
};
*/
#endif // PARTITIONTABLE_H
