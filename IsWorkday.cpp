
#include"pch.h"
#include<iostream>
#include"Searching.h"
using namespace std;

int IsSpecial(Time* t) {//1���طż٣�2���ص��ݹ�����3���ز��Ƿżٻ���ݣ�4���ز���2025��
	if (t->yr == 2025) {//��2025Ϊ����ÿ��һά�����޸�Ϊ����ķ����ڼ���
		//Ԫ��
		if (t->month == 1 && t->day == 1) return 1;
		//���ڣ�1��28�ŵ�2��4�ŷż١�1��26�ź�2��8�ŵ����ϰ�
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
		//������4��4�ŵ�6��
		else if (t->month == 4) {
			if (t->day == 4 || t->day == 5 || t->day == 6) return 1;
			else if (t->day == 27) return 2;

		}
		//�Ͷ���5��1�ŵ�5�ŷż٣�4��27�ŵ����ϰ�
		else if (t->month == 5) {
			for (int d = 1; d <= 5; d++) {
				if (d == t->day) return 1;
			}
			if (t->day == 31) return 1;
		}
		//���磬5��31�ŵ�6��2��
		else if (t->month == 6) {
			if (t->day == 1 || t->day == 2) return 1;
		}
		//����+���죬10��1�ŵ�8�ţ�9��28�ź�10��11�ŵ���
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


bool IsWorkday(Time* t) {//�����շ���true��˫�ݷ���false
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
		week = (h + 6) % 7;//����ĳ��ĳ��ĳ�������ڼ�
		if (week == 0 || week == 6) return false;
		else return true;
	}
}