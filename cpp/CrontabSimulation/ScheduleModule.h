#ifndef __HANDLE_H__
#define __HANDLE_H__

#include "TimeModule.h"

#include <sstream>
#include <algorithm>
#include <iostream>
using namespace std;

namespace ScheduleModule {

	class ScheduleCls {

		public:
			string _command;
			vector<int> _minuteSet;
			vector<int> _hourSet;
			vector<int> _daySet;
			vector<int> _monthSet;
			vector<int> _weekdaySet;
			int _id;

		public:
			void setCommand(const string& command_) { _command = command_; }
			string getCommand() const { return _command; }
			void setId(int id_) { _id = id_; }
			int getId() const { return _id; }


			ScheduleCls(const string& scheduleStr) {
				string part[6];
				istringstream stringInputStream(scheduleStr);

				for (int i = 0; i < 6; ++i) stringInputStream >> part[i];
 //               cout << "DEBUG part: "; for (int i = 0; i < 6; ++i) cout << part[i] << " "; cout << endl;
				resolve(_minuteSet, part[0]);
				resolve(_hourSet, part[1]);
				resolve(_daySet, part[2]);
				resolve(_monthSet, part[3]);
				resolve(_weekdaySet, part[4]);

				setCommand(part[5]);	// ÉèÖÃÃüÁî
			}


			void resolve(vector<int>& vec_, string& str_) {
				if (str_ == "*") {	// * Çé¿ö
					vec_.push_back(-1);
				} else {
					for (char &vec_: str_) if (vec_ == ',') vec_ = ' ';
					istringstream stringInputStream(str_);
					string tmpStr;
					while (stringInputStream >> tmpStr) {
						int pos;
						if ((pos = tmpStr.find('-')) != -1) {
							tmpStr[pos] = ' ';
							istringstream stringInputStream2(tmpStr);
							string begStr, endStr;
							stringInputStream2 >> begStr >> endStr;
							int begNum = TimeModule::GetNum(begStr);
							int endNum = TimeModule::GetNum(endStr);
							while (begNum <= endNum) vec_.push_back(begNum++);
						} else {
							vec_.push_back(TimeModule::GetNum(tmpStr));
						}
					}
				}
				sort(vec_.begin(), vec_.end());
			}

			void print() {
                cout << "_command: " << _command << endl;
                cout << "_minuteSet: "; for (int i : _minuteSet) cout << i << " "; cout << endl;
                cout << "_hourSet: "; for (int i : _hourSet) cout << i << " "; cout << endl;
                cout << "_daySet: "; for (int i : _daySet) cout << i << " "; cout << endl;
                cout << "_monthSet: "; for (int i : _monthSet) cout << i << " "; cout << endl;
                cout << "_weekdaySet: "; for (int i : _weekdaySet) cout << i << " "; cout << endl;
			}
	};

}	// namespace HandleModule {

#endif
