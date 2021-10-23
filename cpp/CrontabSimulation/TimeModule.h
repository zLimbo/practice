#ifndef __TIME_H__
#define __TIME_H__

#include <unordered_map>
using namespace std;


namespace TimeModule {		// ʱ��ģ��


	// �·�����
	static const int MONTH_DAYS[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


	// ����·ݵĵ�����
	inline int GetMonthDays(int month_, int year_) {
		if (month_ == 2) return (year_ % 4 == 0 && year_ % 100 != 0 || year_ % 400 == 0) ? 29 : 28;
		else return MONTH_DAYS[month_];
	}


	// ��ø����������ڼ�
	inline int GetWeek(int year_, int mouth_, int day_) {
		if (mouth_ <= 2) {
			mouth_ += 12;
			--year_;
		}
		int c = year_ / 100;
		year_ = year_ % 100;
		int weakday = year_ + year_/4 + c/4 - 2*c + 26*(mouth_+1)/10 + day_ - 1;
		return weakday % 7;
	}


	// �·ݼ�д��Ӧ������
	static unordered_map<string, int> MW2N = { {"jan", 1}, {"feb", 2}, {"mar", 3}, {"apr", 4}, {"may", 5},
		{"jun", 6}, {"jul", 7}, {"aug", 8}, {"sep", 9}, {"oct", 10}, {"nov", 11}, {"dec", 12},
		{"sun", 0}, {"mon", 1}, {"tue", 2}, {"wed", 3}, {"thu", 4}, {"fri", 5}, {"sat", 6} };


	// �����ַ����õ���Ӧ����ֵ
	inline int GetNum(string &s) {
		if (s[0] < '0' || s[0] > '9') {
			for (char &vec_: s) if (vec_ <= 'Z') vec_ += 32;
			return MW2N[s];
		} else {
			int num = 0;
			for (char vec_: s) {
				num = num * 10 + vec_ - '0';
			}
			return num;
		}
	}


	// ʱ����
	class TimeCls {

		public:

			int _year;	// ��
			int _month;	// ��
			int _day;	// ��
			int _hour;	// ʱ
			int _minute;	// ��

			//  Ĭ�Ϲ��캯��
			TimeCls() = default;

			// �����ַ�������
			TimeCls(string &s_) {
				s_.insert(s_.begin()+10, ' ');
				s_.insert(s_.begin()+8, ' ');
				s_.insert(s_.begin()+6, ' ');
				s_.insert(s_.begin()+4, ' ');
				char *cs = const_cast<char*>(s_.c_str());
				sscanf(cs, "%d%d%d%d%d", &_year, &_month, &_day, &_hour, &_minute);
			}


			// ���ݸ���ֵ����
			TimeCls(int year_, int month_, int day_, int hour_, int minute_):
				_year(year_), _month(month_), _day(day_), _hour(hour_), _minute(minute_)
			{ }


			// �ַ�����
			inline string str() {
				char cs[12];
				sprintf(cs, "%d%02d%02d%02d%02d", _year, _month, _day, _hour, _minute);
				return cs;
			}

	};	// class TimeCls {


	// ���رȽ������
	inline bool operator<(const TimeCls &lhs_, const TimeCls &rhs_) {
		return lhs_._year < rhs_._year ||
				(lhs_._year == rhs_._year && (lhs_._month < rhs_._month ||
				(lhs_._month == rhs_._month && (lhs_._day < rhs_._day ||
				(lhs_._day == rhs_._day && (lhs_._hour < rhs_._hour ||
				(lhs_._hour == lhs_._hour && (lhs_._minute < rhs_._minute) ) ) ) ) ) ) );
	}


	// ���ص��������
	inline bool operator==(const TimeCls &lhs_, const TimeCls &rhs_) {
		return lhs_._year == rhs_._year && lhs_._month == rhs_._month && lhs_._day == rhs_._day
				&& lhs_._hour == rhs_._hour && lhs_._minute == rhs_._minute;
	}

}	// namespace Timemonthodule {		// ʱ��ģ��

#endif
