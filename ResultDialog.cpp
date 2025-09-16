
#include "pch.h"
#include "framework.h"
#include "Cambus.h"
#include "ResultDialog.h"
#include "afxdialogex.h"

// CResultDialog 实现

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
    DDX_Control(pDX, IDC_LIST_RESULT, m_listResult);  // 绑定列表控件
}

BEGIN_MESSAGE_MAP(CResultDialog, CDialogEx)
END_MESSAGE_MAP()

// 初始化对话框：设置列表控件列
// 初始化对话框及列表控件
BOOL CResultDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_listResult.SetExtendedStyle(
        LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP
    );

    CRect rect;
    m_listResult.GetClientRect(&rect);
    // 改为四列，平均分配宽度（可根据需要调整比例）
    m_listResult.InsertColumn(0, _T("推荐路线"), LVCFMT_LEFT, rect.Width() / 100000);
    m_listResult.InsertColumn(1, _T("出发时间"), LVCFMT_LEFT, rect.Width() / 3);
    m_listResult.InsertColumn(2, _T("到达时间"), LVCFMT_LEFT, rect.Width() / 3);
    m_listResult.InsertColumn(3, _T("备注"), LVCFMT_LEFT, rect.Width() / 3);

    return TRUE;
}

// 修改AddResult函数以支持四列数据
void CResultDialog::AddResult(CString recommendRoute, CString departureTime, CString arrivalTime, CString remark)
{
    int nIndex = m_listResult.GetItemCount();
    m_listResult.InsertItem(nIndex, recommendRoute);  // 推荐时间
    m_listResult.SetItemText(nIndex, 1, departureTime);  // 出发时间
    m_listResult.SetItemText(nIndex, 2, arrivalTime);    // 到达时间
    m_listResult.SetItemText(nIndex, 3, remark);         // 备注
}