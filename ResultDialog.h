#pragma once

#include "afxwin.h"

// CResultDialog 对话框
class CResultDialog : public CDialogEx
{
    DECLARE_DYNAMIC(CResultDialog)

public:
    CResultDialog(CWnd* pParent = nullptr);   // 标准构造函数
    virtual ~CResultDialog();

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_RESULT_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();                       // 初始化对话框

    DECLARE_MESSAGE_MAP()

public:
    CListCtrl m_listResult;  // 列表控件成员变量，用于显示结果
    // 改为四参数函数，对应四列数据
    void AddResult(CString recommendRoute, CString departureTime, CString arrivalTime, CString remark);
    // 向列表添加结果的方法
}; 
