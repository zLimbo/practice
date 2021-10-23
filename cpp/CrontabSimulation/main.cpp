#include "SimulateModule.h"
#include "IOModule.h"

#include <iostream>

const string _InputFileName("input.txt");
const string _OutputFileName("output.txt");

int main() {

	cout << "\ncreate simulator ..." << endl;
	SimulateModule::SimulatorCls mySimulator;	// 模拟模块创造 Crontab 模拟器
	cout << "create succeed." << endl;

	cout << "\nFile " << _InputFileName << " content: " << endl;
	IOModule::Print(_InputFileName); // IO模块打印输入文件内容

	cout << "\nstart read from " << _InputFileName << " ..." << endl;
	IOModule::Read(_InputFileName, mySimulator);	// IO模块从指定文件读入数据
    cout << "read succeed." << endl;

	cout << "\nstart simulate ..." << endl;
	mySimulator.simulate();		// 模拟 Crontab 时间调度
    cout << "simulate succeed." << endl;

	cout << "\nwrite in " << _OutputFileName << " ..." << endl;
	IOModule::Write(_OutputFileName, mySimulator); // IO模块将模拟结果写入指定文件
    cout << "write succeed." << endl;

	cout << "\nFile " << _OutputFileName << " content: " << endl;
	IOModule::Print(_OutputFileName); // IO模块打印输出文件内容

	return 0;
}
