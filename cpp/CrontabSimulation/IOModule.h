#ifndef __INPUT_H__
#define __INPUT_H__

#include "SimulateModule.h"
#include "ScheduleModule.h"

#include <fstream>
#include <sstream>
#include <string>

using namespace std;


namespace IOModule {


	// ָ���ļ���������
	void Read(const string& inputFileName_, SimulateModule::SimulatorCls& mySimulator_) {
		// �����ļ�������
		ifstream fileInputStream(inputFileName_);
		// �ƻ�������ʼʱ�䣬����ʱ��
		int scheduleNum;
		string startTimeStr, endTimeStr;
		// ��������
		fileInputStream >> scheduleNum >> startTimeStr >> endTimeStr;
		// ʱ��ģ��
		TimeModule::TimeCls startTime(startTimeStr), endTime(endTimeStr);
		// ��ȡ����
		mySimulator_.setScheduleNum(scheduleNum);	// ���üƻ���
		mySimulator_.setStartTime(startTime);		// ���ÿ�ʼʱ��
		mySimulator_.setEndTime(endTime);			// ���ý���ʱ��
		// �ų����з�����
		fileInputStream.get();
		// ����ƻ�
//		cout << "DEBUG : scheduleNum = " << scheduleNum << endl;
		for (int i = 0; i < scheduleNum; ++i) {
//			cout << "DEBUG : i = " << i << endl;
			string scheduleStr;
			getline(fileInputStream, scheduleStr);
			// ���ô���ģ��Ľ��������Լƻ��ַ������н������õ��ƻ���
			ScheduleModule::ScheduleCls schedule(scheduleStr);
			schedule.setId(i);
			mySimulator_.addSchedule(schedule);	// ��Ӽƻ�
		}
		// �ͷ��ļ���
		fileInputStream.close();
	}


	// ��ģ����д��ָ���ļ�
	void Write(const string& outputFileName_, SimulateModule::SimulatorCls& mySimulator_) {
		// �����ļ������
		ofstream fileOutputStream(outputFileName_);
		// ��ȡ���񴮼���
		vector<string> taskStrSet = mySimulator_.getTaskStrSet();
		// ������ļ���
		for (const string& taskStr: taskStrSet) fileOutputStream << taskStr << endl;
		// �ͷ��ļ���
		fileOutputStream.close();
	}


	// ��ӡ�ļ�����
	void Print(const string& fileName) {
		// �����ļ�������
		ifstream fileInputStream(fileName);

		string line;
		// �������������׼�����
		while (getline(fileInputStream, line)) cout << line << endl;
		// �ͷ��ļ���
		fileInputStream.close();
	}

}

#endif
