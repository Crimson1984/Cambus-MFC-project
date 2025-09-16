
// MainFrm.cpp: CMainFrame 类的实现
//

#include "pch.h"
#include "framework.h"
#include "Cambus.h"

#include "MainFrm.h"

//新增包含两个类
#include "CambusView.h"
#include "CImageView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame() noexcept
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));


	//设置窗口左标题
	SetTitle(_T("CamBus"));
	
	//设置窗口位置和大小
	MoveWindow(0, 0, 1920, 1080);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类

	//return CFrameWnd::OnCreateClient(lpcs, pContext);

	//当前窗口拆分为1行2列
	m_splitter.CreateStatic(this, 1, 2);

	//设置拆分过的窗口
	m_splitter.CreateView(0, 0, RUNTIME_CLASS(CCambusView),CSize(500,1080), pContext);			//一行一列设置为CCambusView,大小为500*1080
	m_splitter.CreateView(0, 1, RUNTIME_CLASS(CImageView), CSize(1420, 1080), pContext);		//一行二列设置为CImageView,大小为1420*1080


	//获取文件相对路径
	TCHAR szPath[MAX_PATH];									//设置一个字符数组
	GetModuleFileName(NULL, szPath, MAX_PATH);				//获取exe的路径
	CString strDir = szPath;								//将exe路劲存到strDir里
	int pos = strDir.ReverseFind('\\');						//找到最后的"\\"即包含exe的文件夹路径
	strDir = strDir.Left(pos);								//将strDir变成文件夹路径
	CString strMap = strDir + _T("\\map.png");				//合成map.png路径
	CString strMarker = strDir + _T("\\marker.png");		//合成marker.png路径
	CString strMarker2S = strDir + _T("\\marker2S.png");	//合成marker2S.png路径
	CString strMarker2E = strDir + _T("\\marker2E.png");	//合成marker2E.png路径

	CImageView* pImageView = (CImageView*)m_splitter.GetPane(0, 1);				//获取右窗口的指针
	pImageView->LoadImageFromFile(strMap,strMarker,strMarker2S, strMarker2E);	//设置图片路径


	//设置为自己拆分
	return true;
}
