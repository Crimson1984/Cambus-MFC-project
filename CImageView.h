#pragma once
#include <afxwin.h>
//为了使CImageView支持图片,引入<atlimage.h>图片处理库
#include <atlimage.h>	//图片处理库


// CImageView 视图


//图上标记结构体:为图标定义的结构体,方便图标的定位与绘制
struct Marker
{
	CPoint pt;		//图标坐标
	CString name;	//图标名
	CRect rect;		//图标区域(可点击区域)
	int choice;		//图标的被选择状态
	//0 for 未使用,1 for 被选为起点,2 for 被选为终点 

};

class CImageView : public CView
{
	DECLARE_DYNCREATE(CImageView)

protected:
	CImageView();           // 动态创建所使用的受保护的构造函数,初始化数据
	virtual ~CImageView();

public:
	virtual void OnDraw(CDC* pDC);      // 手动重写以绘制该视图,重写OnDraw()实现自定义绘图要求;

	BOOL LoadImageFromFile(const CString& path_map, const CString& path_marker, 
		const CString& path_marker2S, const CString& path_marker2E);	//从文件加载图片

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:

	CImage m_image;		//图片类型:用来储存地图图片
	CImage m_marker;	//图片类型:用来保存图标(默认地点所用图标)
	CImage m_marker2S;	//图片类型:用来保存图标2S(起点图标)
	CImage m_marker2E;	//图片类型:用来保存图标2E(终点图标)
	CPoint m_lastPoint;	//点类型:用来记录鼠标位置
	bool m_dragging;	//鼠标状态标记:是否为拖动状态
	CPoint m_offset;	//图片偏移量:用于计算拖动距离
	double m_scale;		//缩放系数:用于计算放大倍数
	int m_choice_count;	//用于判断起点终点选择状态
	//0 for 起点终点都未选择, 1 for 只选择了起点, 2 for 只选择了终点,3 for 都选择

	//需要标记地点的坐标数组
	static Marker m_markers[];	//在.cpp里定义
	static int m_markerCount;	//数组长度:在.cpp里计算

	CImage* GetMarkerImage(int Typecount);							//函数:根据Marker.chioce获取所用图标的地址,便于绘制时选择图标
	void DrawRoute(CDC* pDC, CPoint* route, COLORREF color);		//绘制路线函数,在OnDraw中调用以绘制特定路线
	int IfSpotInRoute(CPoint* route, Marker* mark);					//判断函数:判断地点是否在路线中以选择路线
	bool IsRestrictedTime();										//判断函数:是否是限行时段


	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);	//声明滚轮事件:滚动滚轮缩放地图
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);				//声明鼠标左键按下事件:根据m_dragging判断是地点选择还是拖动地图
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);				//声明鼠标移动事件:拖动地图
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);				//声明鼠标左键松开事件:完成拖动地图
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);								//自己处理背景,不自动重绘,减少闪烁

	
	DECLARE_MESSAGE_MAP()
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};

//以下皆与路径绘制相关
//用于绘制路线的坐标数组
extern CPoint route1[];
extern CPoint route2[];
extern CPoint route3[];
extern CPoint route4[];
extern CPoint route5[];
extern CPoint route6[];
extern CPoint route7[];
extern CPoint route8[];
//路径坐标数组数组:用于储存各路线的指针
extern CPoint* allRoutes[];