#pragma once

#include "afxwin.h"
#include "ResultDialog.h"
#include "Resource.h"

class CCambusView : public CView
{
	DECLARE_DYNCREATE(CCambusView)

protected:
	CCambusView() noexcept;
	virtual ~CCambusView() override;

public:
	DECLARE_MESSAGE_MAP()
	virtual void OnDraw(CDC* pDC) override;
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs) override;

protected:
	// 控件成员（修正重复声明，明确区分日期和时间控件）
	CDateTimeCtrl m_datePicker;		// 日期选择控件
	CDateTimeCtrl m_timePicker;		// 时间选择控件
	CComboBox m_cmbStart;			// 出发地下拉框
	CComboBox m_cmbEnd;				// 目的地下拉框
	CButton m_btnQuery;				// 查询按钮

	// 结果对话框
	CResultDialog m_resultDialog;

	// 成员函数声明
	BOOL GetSelectedDateTime(COleDateTime& dateTime);  // 合并日期时间获取
	void InitControls();			// 初始化控件
	void InitStations();			// 初始化站点数据
	void QueryData(CString strStart, CString strEnd, bool isWeekend);

	//添加说明文字
	CStringArray m_infoTexts;

	// 添加:官方说明链接
	CRect m_linkRect1;										//记录超链接文本的显示区域
	CString m_linkText1 = _T("总务处接驳车通知");			//提示
	CString m_linkURL1 = _T("https://zwc.seu.edu.cn/2024/0914/c4297a502881/page.htm");	// 点击后打开的链接

	// 链接2
	CRect m_linkRect2;
	CString m_linkText2 = _T("无线谷接驳车时刻表(最新)");
	CString m_linkURL2 = _T("http://10.208.177.204:8081/cambus.jpg");


	// 消息处理函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedQuery();
	afx_msg void OnCbnSelchangeStart();		//修改:增加出发地下拉框内容改变响应函数
	afx_msg void OnCbnSelchangeEnd();		//修改:增加目的地下拉框内容改变响应函数
	afx_msg void OnDateTimeChanged(NMHDR* pNMHDR, LRESULT* pResult);	//修改:增加时间日期下拉框内容改变响应函数

	//添加
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point); // 鼠标左键点击
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);   // 鼠标移动
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);	//修改:Doc改变响应函数
};