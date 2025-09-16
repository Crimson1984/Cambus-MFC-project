
#include "pch.h"
#include "framework.h"
#include "Cambus.h"
#include "ResultDialog.h"
#include "afxdialogex.h"

// CResultDialog ʵ��

IMPLEMENT_DYNAMIC(CResultDialog, CDialogEx)

CResultDialog::CResultDialog(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_RESULT_DIALOG, pParent)
{
}

CResultDialog::~CResultDialog()
{
}

void CResultDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_RESULT, m_listResult);  // ���б�ؼ�
}

BEGIN_MESSAGE_MAP(CResultDialog, CDialogEx)
END_MESSAGE_MAP()

// ��ʼ���Ի��������б�ؼ���
// ��ʼ���Ի����б�ؼ�
BOOL CResultDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_listResult.SetExtendedStyle(
        LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP
    );

    CRect rect;
    m_listResult.GetClientRect(&rect);
    // ��Ϊ���У�ƽ�������ȣ��ɸ�����Ҫ����������
    m_listResult.InsertColumn(0, _T("�Ƽ�·��"), LVCFMT_LEFT, rect.Width() / 100000);
    m_listResult.InsertColumn(1, _T("����ʱ��"), LVCFMT_LEFT, rect.Width() / 3);
    m_listResult.InsertColumn(2, _T("����ʱ��"), LVCFMT_LEFT, rect.Width() / 3);
    m_listResult.InsertColumn(3, _T("��ע"), LVCFMT_LEFT, rect.Width() / 3);

    return TRUE;
}

// �޸�AddResult������֧����������
void CResultDialog::AddResult(CString recommendRoute, CString departureTime, CString arrivalTime, CString remark)
{
    int nIndex = m_listResult.GetItemCount();
    m_listResult.InsertItem(nIndex, recommendRoute);  // �Ƽ�ʱ��
    m_listResult.SetItemText(nIndex, 1, departureTime);  // ����ʱ��
    m_listResult.SetItemText(nIndex, 2, arrivalTime);    // ����ʱ��
    m_listResult.SetItemText(nIndex, 3, remark);         // ��ע
}