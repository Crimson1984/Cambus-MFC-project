#pragma once
// Searching.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include <iostream>
#include <string>


class Spot//����վ����
{
public:
    std::string name;//վ������std::string

    char line[8] = {};
    int numline[8] = {};


    Spot(std::string nm = "0", char l1 = 0, int nl1 = 0, char l2 = 0, int nl2 = 0, char l3 = 0, int nl3 = 0,
        char l4 = 0, int nl4 = 0, char l5 = 0, int nl5 = 0, char l6 = 0, int nl6 = 0,
        char l7 = 0, int nl7 = 0, char l8 = 0, int nl8 = 0
    )//��ʼ��վ����
    {
        name = nm;  line[0] = l1; numline[0] = nl1; line[1] = l2; numline[1] = nl2;
        line[2] = l3; numline[2] = nl3; line[3] = l4; numline[3] = nl4;
        line[4] = l5; numline[4] = nl5; line[5] = l6; numline[5] = nl6;
        line[6] = l7; numline[6] = nl7; line[7] = l8; numline[7] = nl8;
    }
    friend class Line;
};

class Time//����ʱ����
{
public:
    int yr;
    int month;
    int day;
    int clock;
    int min;
    int week;//��n
    int mins() { return (clock * 60 + min); }
    Time(int year = 0, int mth = 0, int d = 0, int clk = 0, int mint = 0, int wk = 0)//��ʼ��ʱ����
    {
        yr = year; month = mth; day = d; clock = clk; min = mint; week = wk;
    }
};

class sTime//����΢ʱ����
{
public:
    int clock;
    int min;
    int mins() { return (clock * 60 + min); }
    sTime(int clk = 0, int m = 0) {
        clock = clk; min = m;
    }
};

class Line //������·��
{
protected:
    int num;//��·ͣվ��Ŀ
    Spot* storage;//�洢վ��
public:
    Line(int number = 0, const Spot* SpotIn = nullptr);
    virtual ~Line();
    Line(const Line& other);
};

class LineTimeTable :public Line
{
public:
    int num;//������Ŀ
    sTime* TimeTable;//���η���ʱ���

    LineTimeTable(const Line& l, int number, const std::string* tmtb);
    //�޸�3������Line�Ĺ��캯��������Ϊ���ڶ���
    ~LineTimeTable();


};

class Route
{
public:
    Spot* st;//��ʼվ
    Spot* fi;//�յ�վ
    Spot* tran = nullptr;//����վ(����Ϊ��)
    sTime t1;//�ϳ�ʱ��
    sTime t2;//�³�ʱ��

    // �洢·����Ϣ
    char directLine;        // ֱ��·�ߣ���'A'/'B'��
    char firstLine;         // ���˵�һ��·��
    char secondLine;        // ���˵ڶ���·��

    sTime t_t1 = sTime(-1, -1);  // ��ʼ��Ϊ��Чֵ
    sTime t_t2 = sTime(-1, -1);

    std::string info;//��ʾ��Ϣ

    // �洢��ͬ��·��ѡ�е���·����
    char sameline[5] = {};  // �洢������·
    int tempNum = 0;        // ѡ�е���·����
    int tempNum1 = 0;       // ���˵�һ��·������
    int tempNum2 = 0;       // ���˵ڶ���·������


    sTime cal_Spottime(sTime time, char line, int linenum, bool isAnti = false);
    LineTimeTable* SearchTable(char line, bool iswkd);
    Route* Search(Time t, const std::string st, const std::string fi);//������������������Routeָ��
    bool simpleSearch(sTime t, const Spot* stt, const Spot* fin, bool, sTime*, sTime*);
};

// ȫ��վ����������������������Searching.cpp�У�
extern Spot Library_N;
extern Spot LiterBlock_N;
extern Spot WestGate;
extern Spot Orange;
extern Spot AdminBuld;
extern Spot EleCen;
extern Spot Plum;
extern Spot Gym_W;
extern Spot J8;
extern Spot NorthGate;
extern Spot NEGate;
extern Spot PlumTurn;
extern Spot EGateTurn;
extern Spot SERoad;
extern Spot Peach_N_Din;
extern Spot LanTai;
extern Spot NorGateTurn;
extern Spot JZBlock;
extern Spot LiterColl;
extern Spot SuYuan;
extern Spot WiFi;
extern Spot AntiSuYuan;
extern Spot JiaoE;

// ��·վ����������
extern Spot Aa[11];
extern Spot Bb[12];
extern Spot Cc[3];
extern Spot Dd[2];
extern Spot Ee[4];
extern Spot Ff[3];
extern Spot Gg[2];
extern Spot Hh[4];
extern Spot Ii[2];
extern Spot Jj[4];
extern Spot Kk[5];
extern Spot Ll[3];
extern Spot Mm[3];
extern Spot Nn[5];
extern Spot All[23];

// ȫ����·��������������������Searching.cpp�У�
extern Line A;
extern Line B;
extern Line C;
extern Line D;
extern Line E;
extern Line F;
extern Line G;
extern Line H;
extern Line I;
extern Line J;
extern Line K;
extern Line L;
extern Line M;
extern Line N;

// ʱ����������
// ��������·��ʱ������󣨲��䵽Searching.h�У�
extern LineTimeTable ATimeTable_work;
extern LineTimeTable ATimeTable_end;
extern LineTimeTable BTimeTable;
extern LineTimeTable CTimeTable_work;
extern LineTimeTable DTimeTable_work;
extern LineTimeTable DTimeTable_end;
extern LineTimeTable ETimeTable_work;
extern LineTimeTable FTimeTable_work;
extern LineTimeTable GTimeTable_work;
extern LineTimeTable GTimeTable_end;
extern LineTimeTable HTimeTable_work;
extern LineTimeTable ITimeTable_work;
extern LineTimeTable ITimeTable_end;
extern LineTimeTable JTimeTable_work;
extern LineTimeTable KTimeTable_work;
extern LineTimeTable LTimeTable_work;
extern LineTimeTable LTimeTable_end;
extern LineTimeTable MTimeTable_work;
extern LineTimeTable MTimeTable_end;
extern LineTimeTable NTimeTable_work;
extern LineTimeTable NTimeTable_end;


int IsSpecial(Time* t);
int IsValid(Time* t);
bool IsWorkday(Time* t);

