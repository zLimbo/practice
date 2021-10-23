#include "SimulateModule.h"
#include "IOModule.h"

#include <iostream>

const string _InputFileName("input.txt");
const string _OutputFileName("output.txt");

int main() {

	cout << "\ncreate simulator ..." << endl;
	SimulateModule::SimulatorCls mySimulator;	// ģ��ģ�鴴�� Crontab ģ����
	cout << "create succeed." << endl;

	cout << "\nFile " << _InputFileName << " content: " << endl;
	IOModule::Print(_InputFileName); // IOģ���ӡ�����ļ�����

	cout << "\nstart read from " << _InputFileName << " ..." << endl;
	IOModule::Read(_InputFileName, mySimulator);	// IOģ���ָ���ļ���������
    cout << "read succeed." << endl;

	cout << "\nstart simulate ..." << endl;
	mySimulator.simulate();		// ģ�� Crontab ʱ�����
    cout << "simulate succeed." << endl;

	cout << "\nwrite in " << _OutputFileName << " ..." << endl;
	IOModule::Write(_OutputFileName, mySimulator); // IOģ�齫ģ����д��ָ���ļ�
    cout << "write succeed." << endl;

	cout << "\nFile " << _OutputFileName << " content: " << endl;
	IOModule::Print(_OutputFileName); // IOģ���ӡ����ļ�����

	return 0;
}
