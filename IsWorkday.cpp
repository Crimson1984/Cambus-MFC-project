
#include"pch.h"
#include<iostream>
#include"Searching.h"
using namespace std;

int IsSpecial(Time* t) {//1返回放假，2返回调休工作，3返回不是放假或调休，4返回不是2025年
	if (t->yr == 2025) {//以2025为例，每年一维护，修改为当年的法定节假日
		//元旦
		if (t->month == 1 && t->day == 1) return 1;
		//春节，1月28号到2月4号放假。1月26号和2月8号调休上班
		else if (t->month == 1) {
			for (int d = 28; d <= 31; d++) {
				if (t->day == d) return 1;
			}
			if (t->day == 26) return 2;
		}
		else if (t->month == 2) {
			for (int d = 1; d <= 4; d++) {
				if (d == t->day) return 1;
			}
			if (t->day == 8) return 2;
		}
		//清明，4月4号到6号
		else if (t->month == 4) {
			if (t->day == 4 || t->day == 5 || t->day == 6) return 1;
			else if (t->day == 27) return 2;

		}
		//劳动，5月1号到5号放假，4月27号调休上班
		else if (t->month == 5) {
			for (int d = 1; d <= 5; d++) {
				if (d == t->day) return 1;
			}
			if (t->day == 31) return 1;
		}
		//端午，5月31号到6月2号
		else if (t->month == 6) {
			if (t->day == 1 || t->day == 2) return 1;
		}
		//中秋+国庆，10月1号到8号，9月28号和10月11号调休
		else if (t->month == 9 && t->day == 28) return 2;
		else if (t->month == 10) {
			for (int d = 1; d <= 8; d++) {
				if (d == t->day) return 1;
			}
			if (t->day == 11) return 2;
		}
		else return 3;
	}
	else return 4;
}


bool IsWorkday(Time* t) {//工作日返回true，双休返回false
	if (IsSpecial(t) == 3) {
		int week;
		Time temp(*t);
		if (t->month == 1) {
			temp.month = 13;
			temp.yr--;
		}
		else if (t->month == 2) {
			temp.month = 14;
			temp.yr--;
		}
		int q = temp.day;
		int m = temp.month;
		int k = temp.yr % 100;
		int j = temp.yr / 100;
		int h = (q + (13 * (m + 1)) / 5 + k + k / 4 + j / 4 + j * 5) % 7;
		week = (h + 6) % 7;//计算某年某月某日是星期几
		if (week == 0 || week == 6) return false;
		else return true;
	}
}