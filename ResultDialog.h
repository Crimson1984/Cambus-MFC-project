#pragma once

#include "afxwin.h"

// CResultDialog �Ի���
class CResultDialog : public CDialogEx
{
    DECLARE_DYNAMIC(CResultDialog)

public:
    CResultDialog(CWnd* pParent = nullptr);   // ��׼���캯��
    virtual ~CResultDialog();

    // �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_RESULT_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    virtual BOOL OnInitDialog();                       // ��ʼ���Ի���

    DECLARE_MESSAGE_MAP()

public:
    CListCtrl m_listResult;  // �б�ؼ���Ա������������ʾ���
    // ��Ϊ�Ĳ�����������Ӧ��������
    void AddResult(CString recommendRoute, CString departureTime, CString arrivalTime, CString remark);
    // ���б���ӽ���ķ���
}; 
