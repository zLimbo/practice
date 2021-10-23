#ifndef __SIMULATEMOUDLE_H__
#define __SIMULATEMOUDLE_H__

#include "TimeModule.h"
#include "ScheduleModule.h"

#include <iostream>
using namespace std;

namespace SimulateModule {

    class TaskCls {
    public:
            TimeModule::TimeCls _time;
            string _command;
            int _id;
            TaskCls(TimeModule::TimeCls time_, string command_, int id_):
                _time(time_), _command(command_), _id(id_)
            { }

            bool operator<(const TaskCls &rhs_) {
                return (_time < rhs_._time) || (_time == rhs_._time && _id < rhs_._id);
            }
    };


	class SimulatorCls {

		private:

			int _scheduleNum;

			TimeModule::TimeCls _startTime;

			TimeModule::TimeCls _endTime;

			vector<ScheduleModule::ScheduleCls> _scheduleSet; 	// 计划集合

			vector<TaskCls> _taskSet;

		public:

			void setScheduleNum(int scheduleNum_) { _scheduleNum = scheduleNum_; }

			int getScheduleCmdNum() const { return _scheduleNum; }

			void setStartTime(TimeModule::TimeCls& startTime_) { _startTime = startTime_; }

			TimeModule::TimeCls getStartTime() const { return _startTime; }

			void setEndTime(TimeModule::TimeCls& endTime_) { _endTime = endTime_; }

			TimeModule::TimeCls getEndTime() const { return _endTime; }

			// 添加计划
			void addSchedule(ScheduleModule::ScheduleCls& schedule_) { _scheduleSet.push_back(schedule_); }


            void simulate() {
			    _taskSet.clear();
				for (ScheduleModule::ScheduleCls& schedule: _scheduleSet)
                    runSchedule(schedule);
                sort(_taskSet.begin(), _taskSet.end());
			}


            void runSchedule(ScheduleModule::ScheduleCls& schedule_) {
                int month = _startTime._month;
                int day = _startTime._day;
                int hour = _startTime._hour;
                int minute = _startTime._minute;
                for (int year = _startTime._year; year <= _endTime._year; ++year, month = 1) {
                    for ( ; month <= 12; ++month, day = 1) {
                        if (match(month, schedule_._monthSet)) {
                            int dayUp = TimeModule::GetMonthDays(month, year);
                            for ( ; day <= dayUp; ++day, hour = 0) {
                                if (match(day, schedule_._daySet) &&
                                        match(TimeModule::GetWeek(year, month, day), schedule_._weekdaySet)) {
                                    for ( ; hour <= 23; ++hour, minute = 0) {
                                        if (match(hour, schedule_._hourSet)) {
                                            for ( ; minute <= 59; ++minute) {
                                                TimeModule::TimeCls time(year, month, day, hour, minute);
                                                if (time < _endTime && match(minute, schedule_._minuteSet)) {
                                                    _taskSet.push_back(TaskCls(time, schedule_._command, schedule_._id));
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            bool match(int val_, vector<int> &vec_) {
                if (vec_.front() != -1) {
                    if (val_ < vec_.front() || val_ > vec_.back()) return false;
                    for (int val: vec_) if (val == val_) return true;
                    return false;
                }
                return true;
            }

			vector<string> getTaskStrSet() {
			    vector<string> taskStrSet;
                for (TaskCls &task: _taskSet) {
                    string taskStr;
                    taskStr = task._time.str() + " " + task._command;
                    taskStrSet.push_back(taskStr);
                }
				return taskStrSet;
			}

			void print() {
			    cout << "scheduleNum = " << _scheduleSet.size() << "\n" << endl;
                for (ScheduleModule::ScheduleCls& schedule: _scheduleSet) {
                    schedule.print();
                    cout << endl;
                }
			}
	};

}

#endif
