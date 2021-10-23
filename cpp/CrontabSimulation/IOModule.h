#ifndef __INPUT_H__
#define __INPUT_H__

#include "SimulateModule.h"
#include "ScheduleModule.h"

#include <fstream>
#include <sstream>
#include <string>

using namespace std;


namespace IOModule {


	// 指定文件读入数据
	void Read(const string& inputFileName_, SimulateModule::SimulatorCls& mySimulator_) {
		// 创建文件输入流
		ifstream fileInputStream(inputFileName_);
		// 计划数，开始时间，结束时间
		int scheduleNum;
		string startTimeStr, endTimeStr;
		// 读入数据
		fileInputStream >> scheduleNum >> startTimeStr >> endTimeStr;
		// 时间模块
		TimeModule::TimeCls startTime(startTimeStr), endTime(endTimeStr);
		// 获取数据
		mySimulator_.setScheduleNum(scheduleNum);	// 设置计划数
		mySimulator_.setStartTime(startTime);		// 设置开始时间
		mySimulator_.setEndTime(endTime);			// 设置结束时间
		// 排除换行符干扰
		fileInputStream.get();
		// 读入计划
//		cout << "DEBUG : scheduleNum = " << scheduleNum << endl;
		for (int i = 0; i < scheduleNum; ++i) {
//			cout << "DEBUG : i = " << i << endl;
			string scheduleStr;
			getline(fileInputStream, scheduleStr);
			// 调用处理模块的解析函数对计划字符串进行解析，得到计划类
			ScheduleModule::ScheduleCls schedule(scheduleStr);
			schedule.setId(i);
			mySimulator_.addSchedule(schedule);	// 添加计划
		}
		// 释放文件流
		fileInputStream.close();
	}


	// 将模拟结果写入指定文件
	void Write(const string& outputFileName_, SimulateModule::SimulatorCls& mySimulator_) {
		// 创建文件输出流
		ofstream fileOutputStream(outputFileName_);
		// 获取任务串集合
		vector<string> taskStrSet = mySimulator_.getTaskStrSet();
		// 输出到文件中
		for (const string& taskStr: taskStrSet) fileOutputStream << taskStr << endl;
		// 释放文件流
		fileOutputStream.close();
	}


	// 打印文件内容
	void Print(const string& fileName) {
		// 创建文件输入流
		ifstream fileInputStream(fileName);

		string line;
		// 读数据输出到标准输出流
		while (getline(fileInputStream, line)) cout << line << endl;
		// 释放文件流
		fileInputStream.close();
	}

}

#endif
