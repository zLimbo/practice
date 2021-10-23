#ifndef __HUFFMAN_ENCODER_COMPRESS_H__
#define __HUFFMAN_ENCODER_COMPRESS_H__

#include <string>
using std::string;


constexpr int _CODE_NUM = 256;			// ���� 
constexpr int _ZIP_NAME_LEN = 16;		// ѹ��ʶ������� 
constexpr int _FILE_NAME_LEN = 256;		//  �ļ������� 
constexpr int _FILE_SIZE_LEN = 8; 		// �ļ���Сֵ���� 
constexpr int _CODE_FREQUENCY_LEN = 8;	//  ��Ƶֵ���� 
constexpr int _ZLZIP_HEAD_LEN = _ZIP_NAME_LEN + _FILE_NAME_LEN + _FILE_SIZE_LEN 	// ͷ�ļ���Ϣ���� 
		+ _CODE_FREQUENCY_LEN * _CODE_NUM;	
static const char _ZIP_NAME[_ZIP_NAME_LEN] = "zLimbo zLzip";	// ѹ�� ʶ��� 
		
		
// ������ 
struct Code {
	unsigned char oldCode;				// ���� 
	unsigned long long frequency;		// Ƶ�� 
	unsigned long long newCode;			// ���� 
	string newCodeStr;					// �����ַ�����ʾ debug 
	int length;							// ���볤�� 
	
	Code(): oldCode(0), frequency(0), newCode(0), length(0) { }
};


// Huffman �ڵ� 
struct HuffmanTreeNode {
	
	unsigned long long weight;			// Ȩ�� 
	Code* codePtr; 						// ָ���루Ҷ�ӽڵ����ָ�� 
	HuffmanTreeNode* left;				// ���֧ 
	HuffmanTreeNode* right;				// �ҷ�֧ 

	
	HuffmanTreeNode(unsigned long long w = 0, Code* cp = nullptr, 
			HuffmanTreeNode* l = nullptr, HuffmanTreeNode* r = nullptr):
		weight(w), codePtr(cp), left(l), right(r)
	{ }
};


// �Զ���Ƚ������������ȶ��� 
class CmparetorOfHuffmanTreeNode {
	public:
		 bool operator() (HuffmanTreeNode*& lhs, HuffmanTreeNode*& rhs) const {
	        return lhs->weight > rhs->weight;
	    }
};


class HuffmanEncoderCompress {
	
	private:
		
		Code _codes[_CODE_NUM];		// �� 
		
		bool _isCompress; 				// �Ƿ�ѹ�������ڶ��ѹ�� �� 
		
		string _inputFileName;			// �����ļ��� 
		string _outputFileName;			// ����ļ��� 
		
		unsigned long long _inputFileSize;		// �����ļ���С 
		unsigned long long _outputFileSize;		// ����ļ���С 
		
		HuffmanTreeNode* _huffmanTreeRoot;		// Huffman�����ڵ� 
	
	
	private:
		// �ͷŽڵ� 
		freeNode(HuffmanTreeNode* np);
	
	public:
		// ���� 
		HuffmanEncoderCompress(const string& inputFileName, bool isCompress = false);
		
		// ���� 
		~HuffmanEncoderCompress();
		
		// �޷��������� 
		HuffmanEncoderCompress(const HuffmanEncoderCompress& hec) = delete;
		
		// ���� 
		void run();
		
		// ͳ��Ƶ�� 
		void statisticalFrequency();
		
		// ���� Huffman �� 
		HuffmanTreeNode* buildHuffmanTree();
		
		// ��� Huffman ���� 
		void getNewCodes(HuffmanTreeNode* np, unsigned long long newCode, string newCodeStr, int length);
		
		// ѹ���ļ� 
		void compress();
		
		// �����ڵ㣬�����ڽ��� 
		bool findNode(HuffmanTreeNode*& np, unsigned char inputByte, int& pos);
		
		// ��ѹ�� 
		void uncompress();
		
		// ��ӡ������������Ϣ 
		void printHuffmanEncodeInfo();
		
		// ��ӡѹ�����ѹ����Ϣ 
		void printInfo(const char* type);
		
		// ��ȡ����ļ��� 
		string getOutputFileName() const { return _outputFileName; }
		
		// �ж������ļ��Ƿ���� 
		bool compare2File(const string& fileName1, const string& fileName2);
};


#endif
