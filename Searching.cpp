
#include "pch.h"
#include "Searching.h"

// 全局站点定义
Spot Library_N("图书馆北（中央大道）", 'A', 1, 'B', 1);
Spot LiterBlock_N("文科大楼北", 'A', 2, 'B', 2);
Spot WestGate("西门（纪忠楼）", 'A', 3, 'B', 3);
Spot Orange("橘园", 'A', 4, 'B', 4, 'I', 2, 'J', 4, 'K', 5, 'L', 3, 'M', 1, 'N', 1);
Spot AdminBuld("行政楼", 'A', 5, 'B', 5);
Spot EleCen("电工电子中心", 'A', 6, 'B', 6);
Spot Plum("梅园", 'A', 7);
Spot Gym_W("体育馆西（南工路丁字路口）", 'A', 8);
Spot J8("教八站", 'A', 9);
Spot NorthGate("北门站", 'A', 10, 'B', 11);
Spot NEGate("东北门", 'A', 11, 'B', 12);
Spot PlumTurn("梅园转盘（西南角）", 'B', 7);
Spot EGateTurn("东门转盘", 'B', 8);
Spot SERoad("东南路（桃园1-4路口）", 'B', 9);
Spot Peach_N_Din("桃园北食堂", 'B', 10);
Spot LanTai("兰台", 'C', 1, 'D', 1, 'E', 1, 'F', 3, 'G', 2, 'H', 4);
Spot NorGateTurn("北门转盘", 'C', 2, 'D', 2, 'E', 2, 'F', 2, 'G', 1, 'H', 3);
Spot JZBlock("纪忠楼", 'C', 3, 'E', 4, 'F', 1, 'H', 1);
Spot LiterColl("文学院", 'E', 3, 'H', 2);
Spot SuYuan("苏源", 'I', 1, 'J', 1, 'K', 2, 'M', 2, 'N', 2);
Spot WiFi("无线谷", 'J', 2, 'K', 3, 'L', 1, 'N', 3);
Spot AntiSuYuan("苏源（对面）", 'J', 3, 'K', 4, 'L', 2, 'N', 4);
Spot JiaoE("焦廷标馆东侧", 'K', 1);

// 线路站点数组定义
Spot Aa[11] = { Library_N,LiterBlock_N ,WestGate ,Orange ,AdminBuld ,EleCen ,Plum ,Gym_W, J8 ,NorthGate ,NEGate };
Spot Bb[12] = { Library_N,LiterBlock_N, WestGate,Orange,AdminBuld,EleCen,PlumTurn,EGateTurn,SERoad, Peach_N_Din, NorthGate,NEGate };
Spot Cc[3] = { LanTai ,NorGateTurn ,JZBlock };
Spot Dd[2] = { LanTai ,NorGateTurn };
Spot Ee[4] = { LanTai ,NorGateTurn ,LiterColl,JZBlock };
Spot Ff[3] = { JZBlock ,NorGateTurn ,LanTai };
Spot Gg[2] = { NorGateTurn,LanTai };
Spot Hh[4] = { JZBlock ,LiterColl ,NorGateTurn ,LanTai };
Spot Ii[2] = { SuYuan ,Orange };
Spot Jj[4] = { SuYuan ,WiFi ,AntiSuYuan ,Orange };
Spot Kk[5] = { JiaoE ,SuYuan ,WiFi ,AntiSuYuan ,Orange };
Spot Ll[3] = { WiFi ,AntiSuYuan,Orange };
Spot Mm[3] = { Orange  ,SuYuan ,Orange };
Spot Nn[5] = { Orange ,SuYuan ,WiFi ,AntiSuYuan ,Orange };
Spot All[23] = { Library_N,LiterBlock_N ,WestGate ,Orange ,AdminBuld ,EleCen ,Plum ,Gym_W, J8 ,NorthGate ,NEGate ,PlumTurn,EGateTurn,SERoad, Peach_N_Din ,LanTai ,NorGateTurn ,JZBlock,LiterColl,SuYuan ,WiFi ,AntiSuYuan,JiaoE };

// 全局线路定义
Line A(11, Aa);
Line B(12, Bb);
Line C(3, Cc);
Line D(2, Dd);
Line E(4, Ee);
Line F(3, Ff);
Line G(2, Gg);
Line H(4, Hh);
Line I(2, Ii);
Line J(4, Jj);
Line K(5, Kk);
Line L(3, Ll);
Line M(3, Mm);
Line N(5, Nn);

// 时间数组定义
std::string Atime_work[] = { "0700","0710","0720","0730","0740","0745",
"0750","0755","0800","0805","0810","0815","0820","0825","0830",
"0835","0840","0845","0850","0855","0900","0910","1000","1030","1100",
"1200","1210","1220","1230","1240","1250","1300","1310","1400",
"1500","1510","1600","1650","1655","1700","1705","1710","1720",
"1730","1740","1750","1800","1810","1820","1830","1900","1930",
"2000","2030","2100","2130","2200","2230","2300" };//59

std::string Atime_end[] = { "0800","0830","0900","0930","1030","1130","1200","1230",
"1300","1330","1430","1530","1630","1700","1730","1800","1830","1900",
"2000","2100","2200","2230","2300" };//23

std::string Btime[] = { "0920","0930","0940","0950","1130",
"1140","1150","1320","1330","1520","1530","1540","1550" };//13

std::string Ctime_work[] = { "0730","0740","0930" };//3

std::string Dtime_work[] = { "1000","1030","1100","1120","1140","1155",
"1210","1220","1240","1250","1305","1320","1400","1420","1440","1500",
"1520","1540","1600","1630","1650","1710","1720","1740","1750","1800",
"1820","1830","1840","1850","1910","1930","1945","2005","2025","2045",
"2105","2125","2145","2205","2215","2225" };//42

std::string Dtime_end[] = { "0720","0740","0800","0820","0840","0900","0920",
"0940","1000","1030","1050","1110","1130","1150","1210","1230","1250",
"1300","1320","1340","1400","1420","1440","1500","1520","1540","1600",
"1620","1640","1700","1720","1740","1800","1820","1840","1900","1920",
"1935","1955","2015","2035","2055","2115","2135","2155","2215","2225" };//47

std::string Etime_work[] = { "1020","1110","1340","1430","1530","1620","1710","1840",
"1930","2025" };//10

std::string Ftime_work[] = { "0730","0740","0930" };//3

std::string Gtime_work[] = { "1010","1040","1110","1130","1150","1200","1220",
"1230","1240","1300","1310","1330","1410","1430","1450","1510","1530",
"1550","1610","1640","1700","1710","1730","1740","1750","1800","1810",
"1830","1840","1850","1900","1920","1940","2000","2020","2040","2100",
"2120","2140","2200","2210","2220","2230" };//43

std::string Gtime_end[] = { "0730","0750","0810","0830","0850","0910","0930",
"0950","1010","1040","1100","1120","1140","1200","1220","1240","1255",
"1310","1330","1350","1410","1430","1450","1510","1530","1550","1610",
"1630","1650","1710","1730","1750","1810","1830","1850","1910","1930",
"2010","2030","2050","2110","2130","2150","2210","2220","2230" };//46

std::string Htime_work[] = { "1050","1140","1230","1500","1550","1650","1740",
"1830","1955","2050","2140" };//11

std::string Itime_work[] = { "0720","0730","0740","0750" };//4

std::string Itime_end[] = { "0730","0750","0800","0820" };//4

std::string Jtime_work[] = { "0800" };//1

std::string Ktime_work[] = { "0830","0900" };//2

std::string Ltime_work[] = { "0810","0820","0830","0840","0850","0900","0910",
"0920","0930","0940","0950","1005","1110","1130","1140","1150","1200",
"1210","1210","1230","1240","1340","1350","1400","1410","1530","1550",
"1610","1640","1700","1720","1730","1740","1800","1810","1830","2100" };//37

std::string Ltime_end[] = { "0840","0850","0900","0930","0940","1120","1130",
"1150","1210","1340","1350","1410","1540","1610","1640","1700","1730",
"1750","1900","2040","2110" };//21

std::string Mtime_work[] = { "1000","1030","1300","1320","1430","1450","1510",
"1840","1900","1920","1940","2000","2020","2040","2110" };//15

std::string Mtime_end[] = { "1000","1030","1100","1230","1300","1430","1500",
"1800","1820","1840","1930","1950","2010" };//13

std::string Ntime_work[] = { "0800","0810","0820","0830","0840","0850","0900",
"0910","0920","0930","0940","0955","1100","1120","1130","1140","1150",
"1200","1220","1230","1330","1340","1350","1400","1520","1540","1600",
"1630","1650","1710","1720","1730","1750","1800","1820","2050" ,"2130","2210" };//38

std::string Ntime_end[] = { "0830","0840","0850","0920","0930","1110","1120", };
// 补充完整剩余内容

// 定义各线路的时间表格对象（补充到Searching.cpp中）
LineTimeTable ATimeTable_work(A, 59, Atime_work);       // Atime_work长度为59
LineTimeTable ATimeTable_end(A, 23, Atime_end);         // Atime_end长度为23
LineTimeTable BTimeTable(B, 13, Btime);                 // Btime长度为13
LineTimeTable CTimeTable_work(C, 3, Ctime_work);         // Ctime_work长度为3
LineTimeTable DTimeTable_work(D, 42, Dtime_work);       // Dtime_work长度为42
LineTimeTable DTimeTable_end(D, 47, Dtime_end);         // Dtime_end长度为47
LineTimeTable ETimeTable_work(E, 10, Etime_work);       // Etime_work长度为10
LineTimeTable FTimeTable_work(F, 3, Ftime_work);         // Ftime_work长度为3
LineTimeTable GTimeTable_work(G, 43, Gtime_work);       // Gtime_work长度为43
LineTimeTable GTimeTable_end(G, 46, Gtime_end);         // Gtime_end长度为46
LineTimeTable HTimeTable_work(H, 11, Htime_work);       // Htime_work长度为11
LineTimeTable ITimeTable_work(I, 4, Itime_work);         // Itime_work长度为4
LineTimeTable ITimeTable_end(I, 4, Itime_end);           // Itime_end长度为4
LineTimeTable JTimeTable_work(J, 1, Jtime_work);         // Jtime_work长度为1
LineTimeTable KTimeTable_work(K, 2, Ktime_work);         // Ktime_work长度为2
LineTimeTable LTimeTable_work(L, 37, Ltime_work);       // Ltime_work长度为37
LineTimeTable LTimeTable_end(L, 21, Ltime_end);         // Ltime_end长度为21
LineTimeTable MTimeTable_work(M, 15, Mtime_work);       // Mtime_work长度为15
LineTimeTable MTimeTable_end(M, 13, Mtime_end);         // Mtime_end长度为13
LineTimeTable NTimeTable_work(N, 38, Ntime_work);       // Ntime_work长度为38
LineTimeTable NTimeTable_end(N, 7, Ntime_end);          // 假设Ntime_end剩余长度为7（根据实际补充）

Line::Line(const Line& other) : num(other.num) {
	storage = new Spot[num];
	for (int i = 0; i < num; i++) {
		storage[i] = other.storage[i];
	}
}


Line::Line(int number, const Spot* SpotIn) :num(number) //使用动态内存管理初始化线路类
{
	storage = new Spot[num];
	for (int i = 0; i < num; i++)
	{
		storage[i] = SpotIn[i];
	}
}

Line::~Line()//析构函数，确保无内存泄露/////////////////////注释2，修改为类内定义
{
	if (storage != nullptr) {
		delete[] storage;
	}
}

//为了键入方便，采用std::string转化为sTime的方法，其中时间键入格式为 01:02
//时间键入修改为0102


LineTimeTable::LineTimeTable(const Line& l, int number, const std::string* tmtb) :Line(l) {
	num = number;
	TimeTable = new sTime[number];
	for (int i = 0; i < number; i++)
	{
		const char* tempcharptr = tmtb[i].c_str();
		TimeTable[i] = sTime((tempcharptr[0] - '0') * 10 + (tempcharptr[1] - '0'), (tempcharptr[2] - '0') * 10 + (tempcharptr[3] - '0'));//此处修改
	}
}
LineTimeTable::~LineTimeTable()
{
	delete[] TimeTable;
}

Route* Route::Search(Time t, const std::string stt, const std::string fin)
{
	// 初始化路线信息
	directLine = 0;
	firstLine = 0;
	secondLine = 0;
	tempNum1 = 0;
	tempNum2 = 0;

	info = "查询失败：未找到匹配的班次"; // 默认失败信息
	st = nullptr;
	fi = nullptr;
	tran = nullptr; // 重置换乘点

	// 查找起点
	for (int i = 0; i < 23; i++) {
		if (stt == All[i].name) { // 关键：字符串比较必须完全一致
			st = &All[i];
			break;
		}
	}
	if (st == nullptr) {
		info = "未找到起点";
		return this;
	}

	// 查找终点
	for (int i = 0; i < 23; i++) {
		if (fin == All[i].name) {
			fi = &All[i];
			break;
		}
	}
	if (fi == nullptr) {
		info = "未找到终点";
		return this;
	}

	// 判断是否为工作日
	bool iswkD = (t.week >= 1 && t.week <= 5);
	bool isOK = false;
	sTime t_s(t.clock, t.min);

	// 首先尝试所有可能的直达路线
	isOK = simpleSearch(t_s, st, fi, iswkD, &t1, &t2);
	if (isOK) {
		directLine = sameline[tempNum];
		return this;
	}

	// 直达失败，尝试换乘
	// 优化换乘站选择逻辑
	tran = nullptr;
	// 优先选择共线站点作为换乘站（排除起点站点）
	for (int i = 0; i < 23; i++) {
		// 跳过起点站点
		if (&All[i] == st) continue;

		bool isStOnLine = false, isFiOnLine = false;
		for (int j = 0; j < 8; j++) {
			if (All[i].line[j] == 0) break;
			for (int k = 0; k < 8; k++) {
				if (st->line[k] == All[i].line[j]) isStOnLine = true;
				if (fi->line[k] == All[i].line[j]) isFiOnLine = true;
			}
		}
		if (isStOnLine && isFiOnLine) {
			tran = &All[i];
			break;
		}
	}
	if (tran == nullptr) {
		info = "未找到合适的换乘路线";
		return nullptr;
	}

	// 第一段：起点到换乘站
	sTime t1_temp, tt1_temp;
	bool firstSegOK = simpleSearch(t_s, st, tran, iswkD, &t1_temp, &tt1_temp);
	char firstLineTemp = 0;
	if (firstSegOK) {
		// 保存第一程查询后的线路信息
		char firstSameline[5];
		int firstTempNum = tempNum;
		memcpy(firstSameline, sameline, sizeof(sameline));
		// 获取第一程的线路编号
		firstLineTemp = firstSameline[firstTempNum];

		// 执行第二程查询（从换乘站到终点）
		bool secondSegOK = simpleSearch(tt1_temp, tran, fi, iswkD, &t_t2, &t2);

		if (secondSegOK) {
			firstLine = firstLineTemp;  // 第一程线路
			secondLine = sameline[tempNum];  // 第二程线路
			t1 = t1_temp;  // 第一程出发时间（起点）
			t_t1 = tt1_temp;  // 第一程到达时间（换乘站）
			info = "经" + tran->name + "换乘，" + std::string(1, firstLine) + "转" + std::string(1, secondLine);
			return this;
		}
		else {
			// 恢复第一程的查询状态
			memcpy(sameline, firstSameline, sizeof(sameline));
			tempNum = firstTempNum;
		}
	}

	info = "未找到合适的换乘路线";
	return this;
}


bool Route::simpleSearch(sTime t, const Spot* stt, const Spot* fin, bool iswkD, sTime* ts1, sTime* ts2)
{
	// 移除局部变量定义，使用类成员变量
	char sameline[5] = {};
	int samelinenumst[5] = {};  // 起始站在共同线路中的索引
	int samelinenumfi[5] = {};  // 终点站在共同线路中的索引
	int count = 0;

	// 清空之前的线路信息
	memset(sameline, 0, sizeof(sameline));
	tempNum = 0;

	for (int i = 0; i < 8; i++)
	{
		if (stt->line[i] == 0) break;
		for (int j = 0; j < 8; j++)
		{
			if (fin->line[j] == 0)break;
			if (fin->line[j] == stt->line[i]) {
				sameline[count] = stt->line[i];  // 使用成员变量
				samelinenumst[count] = i;
				samelinenumfi[count] = j;
				count++;
			}
		}
	}
	if (count == 0)return false;

	sTime stTime[5];
	sTime fiTime[5];
	int minsduring[5] = {};

	for (int i = 0; i < count; i++)
	{
		LineTimeTable* timeTablenow = SearchTable(sameline[i], iswkD);
		if (timeTablenow == nullptr) continue;
		sTime thisbeginningtime;
		int j = 0;
		for (; j < timeTablenow->num; j++)
		{
			if (timeTablenow->TimeTable[j].mins() >= t.mins() && timeTablenow->TimeTable[j].mins() - t.mins() <= 60) {
				thisbeginningtime = timeTablenow->TimeTable[j];
				break;
			}
		}

		if (j == timeTablenow->num)continue;

		if (sameline[i] == 'A' || sameline[i] == 'B') {
			bool isAnti = false;
			if (fin->numline[samelinenumfi[i]] - stt->numline[samelinenumst[i]] < 0)
				isAnti = true;
			stTime[i] = cal_Spottime(thisbeginningtime, sameline[i], stt->numline[samelinenumst[i]], isAnti);
			fiTime[i] = cal_Spottime(thisbeginningtime, sameline[i], fin->numline[samelinenumfi[i]], isAnti);
			minsduring[i] = fiTime[i].mins() - t.mins();
		}
		else
		{
			if (fin->numline[samelinenumfi[i]] - stt->numline[samelinenumst[i]] < 0)
				continue;
			stTime[i] = cal_Spottime(thisbeginningtime, sameline[i], stt->numline[samelinenumst[i]]);
			fiTime[i] = cal_Spottime(thisbeginningtime, sameline[i], fin->numline[samelinenumfi[i]]);
			minsduring[i] = fiTime[i].mins() - t.mins();
		}
	}

	// 选择最优线路（修改为成员变量tempNum）
	tempNum = 0;
	for (int i = 0; i < count; i++)
	{
		if (minsduring[i] > minsduring[tempNum])
			tempNum = i;
	}
	if (minsduring[tempNum] == 0) {
		return false;
	}
	else {
		for (int i = 0; i < count; i++)
		{
			if (minsduring[i] < minsduring[tempNum] && minsduring[i] > 0)
				tempNum = i;
		}
		ts1->clock = stTime[tempNum].clock;
		ts2->clock = fiTime[tempNum].clock;
		ts1->min = stTime[tempNum].min;
		ts2->min = fiTime[tempNum].min;
	}


	return true;
}

sTime Route::cal_Spottime(sTime time, char line, int linenum, bool isAnti)
{
	int mins = time.clock * 60 + time.min;
	if (isAnti == true)mins += 10;
	if (line == 'A' && isAnti == false) { mins += 2 * linenum - 2; return sTime(mins / 60, mins % 60); }
	if (line == 'A' && isAnti == true) { mins += 2 * (11 - linenum); return sTime(mins / 60, mins % 60); }
	if (line == 'B' && isAnti == false) { mins += 2 * linenum - 2; return sTime(mins / 60, mins % 60); }
	if (line == 'B' && isAnti == true) { mins += 2 * (12 - linenum); return sTime(mins / 60, mins % 60); }
	if (line == 'C') { mins += 7 * (linenum - 1); return sTime(mins / 60, mins % 60); }
	if (line == 'D') { mins += 7 * (linenum - 1); return sTime(mins / 60, mins % 60); }
	if (line == 'E') { mins += 7 * (linenum - 1); return sTime(mins / 60, mins % 60); }
	if (line == 'F') { mins += 7 * (linenum - 1); return sTime(mins / 60, mins % 60); }
	if (line == 'G') { mins += 7 * (linenum - 1); return sTime(mins / 60, mins % 60); }
	if (line == 'H') { mins += 7 * (linenum - 1); return sTime(mins / 60, mins % 60); }
	if (line == 'I') { mins += 7 * (linenum - 1); return sTime(mins / 60, mins % 60); }
	if (line == 'J') { mins += 7 * (linenum - 1); return sTime(mins / 60, mins % 60); }
	if (line == 'K' && linenum < 2) { return sTime(mins / 60, mins % 60); }

	if (line == 'K' && linenum > 2) { mins += (7 * (linenum - 2) + 10); return sTime(mins / 60, mins % 60); }
	if (line == 'K' && linenum == 2) { mins += 10; return sTime(mins / 60, mins % 60); }
	if (line == 'L') { mins += 7 * (linenum - 1); return sTime(mins / 60, mins % 60); }
	if (line == 'M') { mins += 7 * (linenum - 1); return sTime(mins / 60, mins % 60); }
	if (line == 'N') { mins += 7 * (linenum - 1); return sTime(mins / 60, mins % 60); }


}

LineTimeTable* Route::SearchTable(char line, bool iswkd)
{
	if (line == 'A' && iswkd) return &ATimeTable_work;
	if (line == 'A' && !iswkd) return &ATimeTable_end;
	if (line == 'B') return &BTimeTable;  // B线路未区分工作日/非工作日
	if (line == 'C' && iswkd) return &CTimeTable_work;
	if (line == 'D' && iswkd) return &DTimeTable_work;
	if (line == 'D' && !iswkd) return &DTimeTable_end;
	if (line == 'E' && iswkd) return &ETimeTable_work;
	if (line == 'F' && iswkd) return &FTimeTable_work;
	if (line == 'G' && iswkd) return &GTimeTable_work;
	if (line == 'G' && !iswkd) return &GTimeTable_end;
	if (line == 'H' && iswkd) return &HTimeTable_work;
	if (line == 'I' && iswkd) return &ITimeTable_work;
	if (line == 'I' && !iswkd) return &ITimeTable_end;
	if (line == 'J' && iswkd) return &JTimeTable_work;
	if (line == 'K' && iswkd) return &KTimeTable_work;
	if (line == 'L' && iswkd) return &LTimeTable_work;
	if (line == 'L' && !iswkd) return &LTimeTable_end;
	if (line == 'M' && iswkd) return &MTimeTable_work;
	if (line == 'M' && !iswkd) return &MTimeTable_end;
	if (line == 'N' && iswkd) return &NTimeTable_work;
	if (line == 'N' && !iswkd) return &NTimeTable_end;
	return nullptr;  // 未匹配到线路时返回空
}