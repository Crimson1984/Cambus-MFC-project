// CImageView.cpp: 实现文件
//

#include "pch.h"
#include "Cambus.h"
#include "CImageView.h"
#include "CambusDoc.h"		//添加CambusDoc头文件以实现两类数据交换
#include <mmsystem.h>	//多媒体API,实现音频播放
#pragma comment(lib, "winmm.lib") // 链接 winmm 库,与音频播放相关


// CImageView

IMPLEMENT_DYNCREATE(CImageView, CView)



//初始化函数:鼠标状态,图片偏移量,缩放系数,图标选择状态置零
CImageView::CImageView()
{
	m_dragging = false;
	m_offset = CPoint(0, 0);
	m_scale = 1.0;
	m_choice_count = 0;
}

CImageView::~CImageView()
{
}

//声明鼠标事件
//声明背景擦除事件:减少闪烁
BEGIN_MESSAGE_MAP(CImageView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CImageView 绘图
//重写OnDraw函数,使用双缓冲优化:先在内存里画好内容,再一次性复制到窗口，避免屏幕闪烁。
void CImageView::OnDraw(CDC* pDC)
{
	// TODO:  在此添加绘制代码

	//获取窗口矩形大小
	CRect rect;
	GetClientRect(&rect);

	// 创建内存 DC
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	// 创建兼容位图:作为画布
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	// 选入内存 DC,保存旧指针
	CBitmap* pOldBmp = memDC.SelectObject(&bmp);//给memDC绑定一个位图

	// 填充纯白背景
	memDC.FillSolidRect(&rect, RGB(255, 255, 255));

	// 绘制图片
	if (!m_image.IsNull())	//确保图片存在
	{
		//根据缩放系数计算图片大小
		int newWidth = int(m_image.GetWidth() * m_scale);
		int newHeight = int(m_image.GetHeight() * m_scale);

		//绘制:考虑偏移量
		m_image.Draw(memDC.m_hDC, m_offset.x, m_offset.y,newWidth, newHeight);
	}


	CImage* PImage;		//图标指针,便于选择图标


	//RouteNum用于判断两个图标是否在同一条线上
	int RouteNum1 = -1;
	int RouteNum2 = -2;

	//direction用于判断出发地与目的地的先后顺序
	int direction = 0;
	int direction1 = 0;
	int direction2 = 0;

	//遍历地点,画出图标
	for (int i = 0; i < m_markerCount; i++)
	{

		//计算地点坐标
		int x = m_offset.x + int(m_markers[i].pt.x * m_scale);	//考虑缩放大小
		int y = m_offset.y + int(m_markers[i].pt.y * m_scale);


		//绘制路线
		
		//判断方向(若出发地与目的地都选择且出发地与目的地在同一线路(双向线路)上)
		if ((m_choice_count == 3) && (RouteNum1 == RouteNum2) && (RouteNum1 == 1 || RouteNum1 == 3 || RouteNum1 == 5))	//都是返回路段
		{
			//出发方向
			if (direction1 > direction2) { DrawRoute(&memDC, allRoutes[RouteNum1 - 1], RGB(51, 255, 51)); }//以绿色绘制
			//返回方向
			else
			{
				if(direction1 < direction2){ DrawRoute(&memDC, allRoutes[RouteNum1], RGB(51, 255, 51)); }//以绿色绘制
			}
		}
		else
		{
			if (m_markers[i].choice != 0)	//出发地与目的地不在同一条路线上的情况
			{
				for (int ii = 0; ii < 7; ii++)	//循环判断地点在哪条线路上
				{
					if (direction = IfSpotInRoute(allRoutes[ii], &m_markers[i]))//direction获得地点排名
					{
						switch (m_markers[i].choice)
						{
						case 1://出发点
							if (IsRestrictedTime() && (ii == 0 || ii == 1))break;	//限行时段不画正常路线
							if (!IsRestrictedTime() && (ii == 2 || ii == 3))break;	//正常时段不画限行路线
							direction1 = direction;
							RouteNum1 = ii;
							DrawRoute(&memDC, allRoutes[ii], RGB(0, 128, 255));//以蓝色绘制
							break;
						case 2://目的地
							if (IsRestrictedTime() && (ii == 0 || ii == 1))break;	//限行时段不画正常路线
							if (!IsRestrictedTime() && (ii == 2 || ii == 3))break;	//正常时段不画限行路线
							direction2 = direction;
							RouteNum2 = ii;
							DrawRoute(&memDC, allRoutes[ii], RGB(255, 51, 51));//以红色绘制
						}

					}
				}
			}
		}

		PImage = GetMarkerImage(m_markers[i].choice);	//选择使用的图标
		
		if (!PImage->IsNull())
		{
			//获取图标大小
			int w = PImage->GetWidth();
			int h = PImage->GetHeight();

			//画出图标
			PImage->Draw(memDC.m_hDC, x - w / 2, y - h, w, h);

			//计算图标可点击区域
			m_markers[i].rect = CRect(x - w / 2, y - h, x + w / 2, y);
		}
		
		//绘制名称
		memDC.TextOutW(x + 8, y - 8, m_markers[i].name);
	}

	// 将内存 DC 一次性绘制到屏幕
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	// 恢复原位图指针
	memDC.SelectObject(pOldBmp);
}


// CImageView 诊断

#ifdef _DEBUG
void CImageView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CImageView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CImageView 消息处理程序

//加载图片文件并判断是否加载成功
BOOL CImageView::LoadImageFromFile(const CString& path_map, const CString& path_marker, const CString& path_marker2S, const CString& path_marker2E)
{
	m_marker.Load(path_marker);
	m_marker2S.Load(path_marker2S);
	m_marker2E.Load(path_marker2E);
	return SUCCEEDED(m_image.Load(path_map));//加载图片后返回加载状态
}

//鼠标左键按下消息的响应函数
void CImageView::OnLButtonDown(UINT nFlags, CPoint point)
{
	/*CString msg;*/
	CCambusDoc* pDoc = (CCambusDoc*)GetDocument();	//指针转换CDocument → CCambusDoc,获取Doc内的起点终点数据
	for (int i = 0; i < m_markerCount; i++)		//遍历每一个标记
	{
		if (m_markers[i].rect.PtInRect(point))	//判断鼠标点击时是否在标记范围内
		{
			//四种情况:起终未选,起点未选,终点未选,起终皆选
			switch (m_choice_count) {
				case 0:		//起终未选
					m_markers[i].choice = 1;	//设为起点
					m_choice_count = 1;
					pDoc->startStation = m_markers[i].name;		//将起点选择写入Doc数据
					PlaySound(_T("click_r.wav"), NULL, SND_FILENAME | SND_ASYNC);
					/*msg.Format(_T("选择起点:%s"), m_markers[i].name);*/
					break;
				case 1:		//终点未选
					//点击起点
					if (m_markers[i].choice == 1) 
					{
						m_markers[i].choice = 0;
						m_choice_count = 0;		//取消选择起点
						pDoc->startStation = "";		//将Doc数据起点清空
						PlaySound(_T("click_w.wav"), NULL, SND_FILENAME | SND_ASYNC);
						/*msg.Format(_T("取消起点:%s"), m_markers[i].name);*/
						break;
					}
					//点击闲置点
					else {
						m_markers[i].choice = 2;	//设为终点
						m_choice_count = 3;
						pDoc->endStation = m_markers[i].name;	////将终点选择写入Doc数据
						PlaySound(_T("click_r.wav"), NULL, SND_FILENAME | SND_ASYNC);
						//msg.Format(_T("选择终点:%s"), m_markers[i].name);
						break;
					}
				case 2://起点未选
					//点击终点
					if (m_markers[i].choice == 2) 
					{
						m_markers[i].choice = 0;
						m_choice_count = 0;			//取消选择终点
						pDoc->endStation = "";		//将Doc数据终点清空
						PlaySound(_T("click_w.wav"), NULL, SND_FILENAME | SND_ASYNC);
						//msg.Format(_T("取消终点:%s"), m_markers[i].name);
						break;
					}
					//点击闲置点
					else 
					{
						m_markers[i].choice = 1;
						m_choice_count = 3;	//选为起点
						pDoc->startStation = m_markers[i].name;		//将起点选择写入Doc数据
						PlaySound(_T("click_r.wav"), NULL, SND_FILENAME | SND_ASYNC);
						//msg.Format(_T("选择起点:%s"), m_markers[i].name);
						break;
					}
				case 3://起终皆选
					//点击闲置点
					if (m_markers[i].choice == 0) 
					{
						//msg.Format(_T("已完成选择,不再响应"));
						break; 					
					}	//已完成选择,不再响应
					else
					{
						//点击起点
						if (m_markers[i].choice == 1)	//取消起点选择
						{
							m_markers[i].choice = 0;
							m_choice_count = 2;
							pDoc->startStation = "";		//将Doc数据起点清空
							PlaySound(_T("click_w.wav"), NULL, SND_FILENAME | SND_ASYNC);
							//msg.Format(_T("取消起点:%s"), m_markers[i].name);
							break;
						}
						//点击终点
						else   //取消终点选择
						{
							m_markers[i].choice = 0;
							m_choice_count = 1;
							pDoc->endStation = "";		//将Doc数据起点清空
							PlaySound(_T("click_w.wav"), NULL, SND_FILENAME | SND_ASYNC);
							//msg.Format(_T("取消终点:%s"), m_markers[i].name);
							break;
						}
					}
			}
				

			
			//msg.Format(_T("点击标记:%s"), m_markers[i].name);
			//AfxMessageBox(msg);
			
			Invalidate();	//触发重绘

			pDoc->UpdateAllViews(this);		//通知所有视图进行更新(this 表示点击图片的视图发起刷新)

			return;	//结束事件,不进行拖动
		}
	}

	//拖动标记
	m_dragging = true;	//拖动BOOL为真
	m_lastPoint = point;	//几下鼠标当前位置,便于计算拖动距离与方位
	SetCapture();	//设置拖动状态,鼠标超出窗口依然为拖动状态
}

//鼠标移动消息响应函数
void CImageView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_dragging)		//若处于拖动状态
	{
		CPoint delta = point - m_lastPoint;		//得到拖动量
		m_offset += delta;		//得到新的位移
		m_lastPoint = point;	//得到鼠标当前坐标,为下一次拖动计算作准备

		CRect rect;
		GetClientRect(&rect);

		//限制边界
		if (m_offset.x > 0)m_offset.x = 0; 
		if (m_offset.y > 0)m_offset.y = 0;

		int maxX = rect.Width() - m_image.GetWidth() * m_scale;
		int maxY = rect.Height() - m_image.GetHeight() * m_scale;

		if (m_offset.x < maxX) m_offset.x = maxX;
		if (m_offset.y < maxY) m_offset.y = maxY;

		Invalidate();	//触发重绘
	}
}

//鼠标左边抬起消息响应函数
void CImageView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_dragging)			//若处于拖动状态
	{
		m_dragging = false;	//取消拖动标志
		ReleaseCapture();	//解锁拖动状态
	}
}

//自己处理背景,不自动刷新,减少闪烁
BOOL CImageView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE; // 告诉系统自己处理背景
}

//鼠标滚轮消息响应函数滚轮缩放
BOOL CImageView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	ScreenToClient(&pt);	//将屏幕坐标转换成窗口坐标

	double oldScale = m_scale;	//记录原来的缩放比例

	if (zDelta > 0) { m_scale *= 1.1; }	//放大10%
	else { m_scale *= 0.9; }			//缩小10%

	//限制缩放倍率
	if (m_scale < 0.1)m_scale = 0.1;	
	if (m_scale > 10.0)m_scale = 10.0;

	//以鼠标为中心缩放
	double newScale = m_scale;
	m_offset.x = pt.x - (pt.x - m_offset.x) * (newScale / oldScale);
	m_offset.y = pt.y - (pt.y - m_offset.y) * (newScale / oldScale);

	Invalidate();	//触发重绘

	return true;
}


//图标数组:初始化地点坐标(相对与地图图片)与图标名称
Marker CImageView::m_markers[] =
{
	{ CPoint(966, 922),  _T("图书馆北（中央大道）") },
	{ CPoint(794, 752),  _T("文科大楼北") },
	{ CPoint(584, 1260), _T("西门（纪忠楼）") },
	{ CPoint(610, 1490), _T("橘园") },
	{ CPoint(866, 1566), _T("行政楼") },
	{ CPoint(1160, 1547), _T("电工电子中心") },
	{ CPoint(1475, 1301), _T("梅园") },
	{ CPoint(1487, 1160), _T("体育馆西（南工路丁字路口）") },
	{ CPoint(1457, 958), _T("教八站") },
	{ CPoint(1403, 762), _T("北门站") },
	{ CPoint(1854, 484), _T("东北门") },
	{ CPoint(1440, 1507), _T("梅园转盘（西南角）") },
	{ CPoint(1744, 1111), _T("东门转盘") },
	{ CPoint(1742, 864), _T("东南路（桃园1-4路口）") },
	{ CPoint(1505, 778), _T("桃园北食堂") },
	{ CPoint(824, 13), _T("兰台") },
	{ CPoint(1377, 687), _T("北门转盘") },
	{ CPoint(713, 1181), _T("纪忠楼") },
	{ CPoint(1031, 915), _T("文学院") },
	{ CPoint(550, 2064), _T("苏源") },
	{ CPoint(1294, 2756), _T("无线谷") },
	{ CPoint(591, 2048), _T("苏源（对面）") },
	{ CPoint(1597, 994), _T("焦廷标馆东侧") }
};

//计算图标数量
int CImageView::m_markerCount = _countof(CImageView::m_markers);

//用于绘制路线的CPoint数组
//说明:每个数组第一个数据的x坐标成员用于存储数组长度+1;
//正常运行时段-出发
CPoint route1[] =	//30
{
	CPoint(30,0),	//用于坐标存储个数
	CPoint(966, 922),   // 图书馆北（中央大道）
	CPoint(951, 749),
	CPoint(794, 752),   // 文科大楼北
	CPoint(611, 772),
	CPoint(587, 795),
	CPoint(569, 935),
	CPoint(584, 1260),  // 西门（纪忠楼）
	CPoint(610, 1490),  // 橘园
	CPoint(633, 1543),
	CPoint(679, 1573),
	CPoint(866, 1566),  // 行政楼
	CPoint(1160, 1547), // 电工电子中心
	CPoint(1422, 1514),
	CPoint(1430, 1526),
	CPoint(1448, 1528),
	CPoint(1457, 1517),
	CPoint(1456, 1500),
	CPoint(1442, 1490),
	CPoint(1475, 1301), // 梅园
	CPoint(1487, 1160), // 体育馆西（南工路丁字路口）
	CPoint(1457, 958),  // 教八站
	CPoint(1403, 762),  // 北门站
	CPoint(1383, 714),
	CPoint(1395, 693),
	CPoint(1393, 687),
	CPoint(1542, 645),
	CPoint(1733, 624),
	CPoint(1874, 619),
	CPoint(1870, 510),
	CPoint(1854, 484)   // 东北门（终点）
};

//正常运行时段-返回
CPoint route2[] =	//33
{
	CPoint(33,0),
	CPoint(1854, 484),   // 东北门（终点）
	CPoint(1832, 472),
	CPoint(1745, 471),
	CPoint(1751, 556),
	CPoint(1736, 608),
	CPoint(1733, 624),
	CPoint(1542, 645),
	CPoint(1396, 684),
	CPoint(1382, 671),
	CPoint(1372, 670),
	CPoint(1361, 678),
	CPoint(1359, 694),
	CPoint(1366, 705),
	CPoint(1383, 714),
	CPoint(1403, 762),   // 北门站
	CPoint(1457, 958),   // 教八站
	CPoint(1487, 1160),  // 体育馆西（南工路丁字路口）
	CPoint(1475, 1301),  // 梅园
	CPoint(1443,1489),
	CPoint(1427,1497),
	CPoint(1422, 1514),
	CPoint(1160, 1547),  // 电工电子中心
	CPoint(866, 1566),   // 行政楼
	CPoint(679, 1573),
	CPoint(633, 1543),
	CPoint(610, 1490),   // 橘园
	CPoint(584, 1260),   // 西门（纪忠楼）
	CPoint(569, 935),
	CPoint(587, 795),
	CPoint(611, 772),
	CPoint(794, 752),    // 文科大楼北
	CPoint(951, 749),
	CPoint(966, 922)     // 图书馆北（中央大道）
};




//限行时段-出发
CPoint route3[] =	//30
{
	CPoint(30,0),
	CPoint(966, 922),   // 图书馆北（中央大道）
	CPoint(951, 749),
	CPoint(794, 752),   // 文科大楼北
	CPoint(611, 772),
	CPoint(587, 795),
	CPoint(569, 935),
	CPoint(584, 1260),  // 西门（纪忠楼）
	CPoint(610, 1490),  // 橘园
	CPoint(633, 1543),
	CPoint(679, 1573),
	CPoint(866, 1566),  // 行政楼
	CPoint(1160, 1547), // 电工电子中心
	CPoint(1440, 1507),	//梅园转盘（西南角）
	CPoint(1447,1469),
	CPoint(1749,1456),
	CPoint(1744, 1111),	//东门转盘
	CPoint(1742, 864),	//东南路（桃园1-4路口）
	CPoint(1738,749),
	CPoint(1586,760),
	CPoint(1505, 778),	//桃园北食堂
	CPoint(1410,806),
	CPoint(1403, 762),  // 北门站
	CPoint(1383, 714),
	CPoint(1395, 693),
	CPoint(1393, 687),
	CPoint(1542, 645),
	CPoint(1733, 624),
	CPoint(1874, 619),
	CPoint(1870, 510),
	CPoint(1854, 484)   // 东北门（终点）
};


//限行时段-返回
CPoint route4[] =	//36
{
	CPoint(36,0),
	CPoint(1854, 484),   // 东北门（终点）
	CPoint(1832,472),
	CPoint(1745,471),
	CPoint(1751,556),
	CPoint(1736,608),
	CPoint(1733,624),
	CPoint(1542,645),
	CPoint(1396,684),
	CPoint(1382,671),
	CPoint(1372,670),
	CPoint(1361,678),
	CPoint(1359,694),
	CPoint(1366,705),
	CPoint(1383,714),
	CPoint(1403,762),   // 北门站
	CPoint(1410,806),
	CPoint(1505,778),   // 桃园北食堂
	CPoint(1586,760),
	CPoint(1738,749),
	CPoint(1742,864),   // 东南路（桃园1-4路口）
	CPoint(1744,1111),  // 东门转盘
	CPoint(1749,1456),
	CPoint(1447,1469),
	CPoint(1440,1507),  // 梅园转盘（西南角）
	CPoint(1160,1547),  // 电工电子中心
	CPoint(866,1566),   // 行政楼
	CPoint(679,1573),
	CPoint(633,1543),
	CPoint(610,1490),   // 橘园
	CPoint(584,1260),   // 西门（纪忠楼）
	CPoint(569,935),
	CPoint(587,795),
	CPoint(611,772),
	CPoint(794,752),    // 文科大楼北
	CPoint(951,749),
	CPoint(966,922)     // 图书馆北（中央大道）

};

//橘园->无线谷
CPoint route5[] =	//11
{
	CPoint(11,0),
	CPoint(610, 1490),  // 橘园
	CPoint(591,1380),
	CPoint(579,1191),
	CPoint(356,1201),
	CPoint(394,1449),
	CPoint(550,2064),// 苏源
	CPoint(607,2183),
	CPoint(672,2322),
	CPoint(938,2820),
	CPoint(1315,2812),
	CPoint(1294, 2756),// 无线谷
};

//无线谷->橘园
CPoint route6[] =	//11
{
	CPoint(11,0),
	CPoint(1294, 2756),// 无线谷
	CPoint(1312,2790),
	CPoint(951,2796),
	CPoint(688,2321),
	CPoint(623,2174),
	CPoint(591, 2048),// 苏源（对面)
	CPoint(428,1520),
	CPoint(356,1201),
	CPoint(579,1191),
	CPoint(591,1380),
	CPoint(610, 1490)  // 橘园
};

//兰台->纪忠楼
CPoint route7[] =	//23
{
	CPoint(23,0),
	CPoint(824, 13),// 兰台
	CPoint(809,112),
	CPoint(819,209),
	CPoint(865,324),
	CPoint(966,502),
	CPoint(1321,475),
	CPoint(1377, 687), //北门转盘
	CPoint(1223,731),
	CPoint(1020,742),
	CPoint(1031, 915), //文学院
	CPoint(1033,938),
	CPoint(1144,981),
	CPoint(1202,1046),
	CPoint(1235,1144),
	CPoint(1220,1235),
	CPoint(1168,1318),
	CPoint(1139,1340),
	CPoint(1052,1379),
	CPoint(966,1377),
	CPoint(870,1332),
	CPoint(830,1285),
	CPoint(787,1177),
	CPoint(713, 1181) //纪忠楼
};

//路径坐标数组数组
CPoint* allRoutes[] =
{
	route1,
	route2,
	route3,
	route4,
	route5,
	route6,
	route7
};



//此函数用于判断标记点该用何种图标
CImage* CImageView::GetMarkerImage(int Typecount)
{
	//根据Typecount判断应用图标类型
	switch (Typecount)
	{
	case 0:return &m_marker;	//普通图标
	case 1:return &m_marker2S;	//起点图标
	case 2:return &m_marker2E;	//终点图标
	}
}


//绘制路线函数,在 OnDraw中调用
void CImageView::DrawRoute(CDC* pDC, CPoint* route, COLORREF color)
{
	int count = route[0].x;
	if (route == nullptr || count < 2)return;	//不绘制空地址和单点

	CPen pen(PS_SOLID, 5, color);	//创建CPen对象并设置画笔的粗细颜色;
	CPen* pOldPen = pDC->SelectObject(&pen);	//缓存旧的CPen对象

	//计算第一个点的坐标,考虑偏移和缩放
	int x0 = m_offset.x + int(route[1].x * m_scale);
	int y0 = m_offset.y + int(route[1].y * m_scale);
	pDC->MoveTo(x0, y0);
	

	//计算后续点的坐标并连接
	for (int i = 2; i <= count; i++)
	{
		int x = m_offset.x + int(route[i].x * m_scale);
		int y = m_offset.y + int(route[i].y * m_scale);
		pDC->LineTo(x, y);
	}

	pDC->SelectObject(pOldPen);	//换回原来的 CPen对象
}

//判断函数:地点是否在路线中
int CImageView::IfSpotInRoute(CPoint* route, Marker* mark)
{
	int count = route[0].x;
	for (int i = 1; i <= count; i++)
	{
		if (route[i].x == mark->pt.x && route[i].y == mark->pt.y)
			return i;//返回地点排名
	}
	return 0;
}

//新增函数:CambusView下拉框改变时的交互函数,接受Doc类数据
void CImageView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	CCambusDoc* pDoc = (CCambusDoc * )GetDocument();	//获得Doc指针
	if (!pDoc) return;		//没获取到Doc指针则退出
	m_choice_count = 0;
	// 根据文档里保存的站点名字更新图标选择状态
	for (int i = 0; i < m_markerCount; i++)
	{
		if (m_markers[i].name == pDoc->startStation)
		{
			m_markers[i].choice = 1;
			m_choice_count += 1;
		}
			
		else 
		{
			if (m_markers[i].name == pDoc->endStation)
			{
				m_markers[i].choice = 2;
				m_choice_count += 2;
			}
			
			else
				m_markers[i].choice = 0;
		}
		
	}
	Invalidate(); // 触发重绘
}

//判断是否是限行时间
bool CImageView::IsRestrictedTime()
{
	CCambusDoc* pDoc = (CCambusDoc*)GetDocument();	//获得Doc指针
	
	CTime now(		//Search类中的时间类,用Doc里的时间进行初始化
		pDoc->m_selectedDateTime.GetYear(),
		pDoc->m_selectedDateTime.GetMonth(),
		pDoc->m_selectedDateTime.GetDay(),
		pDoc->m_selectedDateTime.GetHour(),
		pDoc->m_selectedDateTime.GetMinute(),
		pDoc->m_selectedDateTime.GetSecond()
	);
	int hour = now.GetHour();	//获取小时
	int minute = now.GetMinute();	//获取分钟

	// 转换成总分钟数，便于比较
	int totalMinutes = hour * 60 + minute;

	// 定义限行区间（起止分钟）
	struct TimeRange
	{
		int start;
		int end;
	};

	TimeRange ranges[] = {
		{ 9 * 60 + 20, 10 * 60 },   // 09:20 - 10:00
		{ 11 * 60 + 20, 12 * 60 },  // 11:20 - 12:00
		{ 13 * 60 + 20, 14 * 60 },  // 13:20 - 14:00
		{ 15 * 60 + 20, 16 * 60 }   // 15:20 - 16:00
	};

	//判断是否在限行时段
	for (int i = 0; i < 4; i++)
	{
		if (totalMinutes >= ranges[i].start && totalMinutes < ranges[i].end)
		{
			return true;  // 在限行时段
		}
	}
	return false; // 不在限行时段
}