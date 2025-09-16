#include"pch.h"
#include "framework.h"
#include "Cambus.h"
#include "CambusView.h"
#include "vector"
#include "Searching.h"
#include "CambusDoc.h"  //修改:添加Doc头文件以实现CambusView与CImageView的数据交换

IMPLEMENT_DYNCREATE(CCambusView, CView)

BEGIN_MESSAGE_MAP(CCambusView, CView)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_BUTTON_QUERY, &CCambusView::OnBnClickedQuery)
    //修改:响应出发地目的地下拉框响应事件
    ON_CBN_SELCHANGE(IDC_COMBO_START, &CCambusView::OnCbnSelchangeStart)
    ON_CBN_SELCHANGE(IDC_COMBO_END, &CCambusView::OnCbnSelchangeEnd)
    //修改:响应时间日期下拉框响应事件
    ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER, &CCambusView::OnDateTimeChanged)
    ON_NOTIFY(DTN_DATETIMECHANGE, IDC_TIMEPICKER, &CCambusView::OnDateTimeChanged)
    //添加:鼠标点击链接响应
    ON_WM_LBUTTONDOWN()   // 鼠标左键点击
    ON_WM_MOUSEMOVE()     // 鼠标移动
END_MESSAGE_MAP()

CCambusView::CCambusView() noexcept
    : m_datePicker(), m_timePicker(), m_cmbStart(), m_cmbEnd(), m_btnQuery(), m_resultDialog()
{
    m_infoTexts.Add(_T("限行时段:"));
    m_infoTexts.Add(_T("09:20 - 10:00  11:20 - 12:00"));
    m_infoTexts.Add(_T("13:20 - 14:00  15:20 - 16:00"));
    m_infoTexts.Add(_T(" "));
    m_infoTexts.Add(_T("说明: 此软件数据仅供参考"));
    m_infoTexts.Add(_T("请以接驳车实际运行情况为准"));
    m_infoTexts.Add(_T("点击链接获取更多信息。"));
}

CCambusView::~CCambusView()
{
}

BOOL CCambusView::PreCreateWindow(CREATESTRUCT& cs)
{
    return CView::PreCreateWindow(cs);
}

void CCambusView::OnDraw(CDC* pDC)
{
    CDocument* pDoc = GetDocument();
    CRect rectClient;
    GetClientRect(&rectClient);

    // 绘制背景
    pDC->FillSolidRect(rectClient, RGB(240, 240, 240));

    // 绘制标题
    CFont titleFont;
    titleFont.CreatePointFont(160, _T("微软雅黑"));
    CFont* pOldTitleFont = pDC->SelectObject(&titleFont);
    pDC->SetTextColor(RGB(0, 51, 102));
    pDC->SetBkMode(TRANSPARENT);
    pDC->DrawText(_T("校园接驳车查询系统"),
        CRect(0, 20, rectClient.Width(), 100),
        DT_CENTER | DT_VCENTER);
    pDC->SelectObject(pOldTitleFont);

    // 设置字体用于说明文字
    CFont infoFont;
    infoFont.CreatePointFont(120, _T("微软雅黑")); // 12pt
    CFont* pOldInfoFont = pDC->SelectObject(&infoFont);
    pDC->SetTextColor(RGB(0, 0, 0));
    pDC->SetBkMode(TRANSPARENT);
    
    const int spacing = 5;  // 行间距

    // 先计算说明文字总高度
    int infoHeight = 0;
    CArray<CSize, CSize> infoSizes;
    for (int i = 0; i < m_infoTexts.GetCount(); i++)
    {
        CSize sz = pDC->GetTextExtent(m_infoTexts[i]);
        infoSizes.Add(sz);
        infoHeight += sz.cy + spacing;
    }

    // 链接总高度
    CSize szLink1 = pDC->GetTextExtent(m_linkText1);
    CSize szLink2 = pDC->GetTextExtent(m_linkText2);
    int linkHeight = szLink1.cy + spacing + szLink2.cy;

    // 从底部开始布局
    int y = rectClient.Height() - 20 - linkHeight - infoHeight;

    for (int i = 0; i < m_infoTexts.GetCount(); i++)
    {
        CSize sz = pDC->GetTextExtent(m_infoTexts[i]);
        CRect rect((rectClient.Width() - sz.cx) / 2, y, (rectClient.Width() + sz.cx) / 2, y + sz.cy);
        pDC->DrawText(m_infoTexts[i], &rect, DT_LEFT | DT_SINGLELINE);
        y += sz.cy + spacing;
    }
    pDC->SelectObject(pOldInfoFont);


    // 绘制底部超链接
    CFont linkFont;
    linkFont.CreatePointFont(120, _T("微软雅黑"));  // 12pt
    CFont* pOldLinkFont = pDC->SelectObject(&linkFont);
    pDC->SetTextColor(RGB(0, 0, 255));   // 蓝色
    pDC->SetBkMode(TRANSPARENT);

    // 链接1位置
    CSize size1 = pDC->GetTextExtent(m_linkText1);
    m_linkRect1 = CRect(
        (rectClient.Width() - size1.cx) / 2,           // 居中
        rectClient.Height() - size1.cy * 2 - spacing - 20, // 上
        (rectClient.Width() + size1.cx) / 2,           // 右
        rectClient.Height() - size1.cy - spacing - 20  // 下
    );
    pDC->DrawText(m_linkText1, &m_linkRect1, DT_LEFT | DT_SINGLELINE);

    // 链接2位置（在链接1下面）
    CSize size2 = pDC->GetTextExtent(m_linkText2);
    m_linkRect2 = CRect(
        (rectClient.Width() - size2.cx) / 2,           // 居中
        rectClient.Height() - size2.cy - 20,           // 上
        (rectClient.Width() + size2.cx) / 2,           // 右
        rectClient.Height() - 20                        // 下
    );
    pDC->DrawText(m_linkText2, &m_linkRect2, DT_LEFT | DT_SINGLELINE);

    pDC->SelectObject(pOldLinkFont);

}

int CCambusView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    CRect rectClient;
    GetClientRect(&rectClient);

    // 创建日期选择控件（仅日期）
    if (!m_datePicker.Create(WS_CHILD | WS_VISIBLE | DTS_SHORTDATEFORMAT,
        CRect(50, 80, rectClient.Width() - 50, 110),
        this, IDC_DATEPICKER))
    {
        TRACE0("未能创建日期选择器控件\n");
        return -1;
    }

    // 日期选择提示文本
    CStatic* pDateHint = new CStatic();
    pDateHint->Create(_T("请选择日期:"),
        WS_CHILD | WS_VISIBLE | SS_LEFT,
        CRect(50, 120, 120, 140),
        this, IDC_STATIC_DATE_SELECT);

    // 创建时间选择控件（仅时间）
    if (!m_timePicker.Create(WS_CHILD | WS_VISIBLE | DTS_TIMEFORMAT,
        CRect(50, 150, rectClient.Width() - 50, 180),
        this, IDC_TIMEPICKER))
    {
        TRACE0("未能创建时间选择器控件\n");
        return -1;
    }

    // 时间选择提示文本
    CStatic* pTimeHint = new CStatic();
    pTimeHint->Create(_T("请选择时间:"),
        WS_CHILD | WS_VISIBLE | SS_LEFT,
        CRect(50, 190, 120, 210),
        this, IDC_STATIC_TIME_SELECT);

    // 出发地下拉框
    if (!m_cmbStart.Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL,
        CRect(50, 220, rectClient.Width() - 50, 250),
        this, IDC_COMBO_START))
    {
        TRACE0("未能创建出发地下拉框\n");
        return -1;
    }

    // 目的地下拉框
    if (!m_cmbEnd.Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL,
        CRect(50, 270, rectClient.Width() - 50, 300),
        this, IDC_COMBO_END))
    {
        TRACE0("未能创建目的地下拉框\n");
        return -1;
    }

    // 查询按钮
    if (!m_btnQuery.Create(_T("查询班次"),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        CRect(50, 320, rectClient.Width() - 50, 350),
        this, IDC_BUTTON_QUERY))
    {
        TRACE0("未能创建查询按钮\n");
        return -1;
    }

    // 初始化控件样式和数据
    InitControls();
    InitStations();

    return 0;
}

void CCambusView::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);

    // 调整控件位置（随窗口大小变化）
    if (::IsWindow(m_datePicker.GetSafeHwnd()))
    {
        m_datePicker.MoveWindow(50, 80, cx - 100, 30);    // 日期控件位置
        m_timePicker.MoveWindow(50, 150, cx - 100, 30);   // 时间控件位置
        m_cmbStart.MoveWindow(50, 220, cx - 100, 30);
        m_cmbEnd.MoveWindow(50, 270, cx - 100, 30);
        m_btnQuery.MoveWindow(50, 320, cx - 100, 30);
    }
}

void CCambusView::InitControls()
{
    // 设置统一字体
    CFont ctrlFont;
    if (ctrlFont.CreatePointFont(100, _T("微软雅黑")))
    {
        m_datePicker.SetFont(&ctrlFont);
        m_timePicker.SetFont(&ctrlFont);
        m_cmbStart.SetFont(&ctrlFont);
        m_cmbEnd.SetFont(&ctrlFont);
        m_btnQuery.SetFont(&ctrlFont);
    }

    // 设置日期范围（2025年1月1日至2025年12月31日）
    COleDateTime minDate(2025, 1, 1, 0, 0, 0);
    COleDateTime maxDate(2025, 12, 31, 23, 59, 59);
    m_datePicker.SetRange(&minDate, &maxDate);  // 限制日期选择范围

    // 设置时间控件格式为"小时:分钟"（不显示秒）
    m_timePicker.SetFormat(_T("HH:mm"));  // 添加此行设置时间格式

    // 设置默认日期和时间为当前
    COleDateTime now = COleDateTime::GetCurrentTime();
    m_datePicker.SetTime(now);  // 默认当前日期
    m_timePicker.SetTime(now);  // 默认当前时间
}
// 确保正确合并日期和时间控件的值
BOOL CCambusView::GetSelectedDateTime(COleDateTime& dateTime)
{
    COleDateTime date, time;
    if (!m_datePicker.GetTime(date) || !m_timePicker.GetTime(time)) {
        return FALSE;
    }
    // 合并日期和时间
    dateTime.SetDateTime(
        date.GetYear(), date.GetMonth(), date.GetDay(),
        time.GetHour(), time.GetMinute(), 0
    );
    
    //修改:添加向Doc类写入数据代码
    CCambusDoc* pDoc = (CCambusDoc * )GetDocument();
    if (pDoc)
    {
        pDoc->m_selectedDateTime = dateTime;
        pDoc->UpdateAllViews(this);  // 通知其他视图更新
    }

    return dateTime.GetStatus() == COleDateTime::valid;
}

void CCambusView::InitStations()
{
    // 站点名称需与Searching.cpp中Spot的name完全一致（含中英文标点）
    CString stations[] = {
        _T("图书馆北（中央大道）"),  // 对应Library_N的name
        _T("文科大楼北"),            // 对应LiterBlock_N的name
        _T("西门（纪忠楼）"),        // 对应WestGate的name
        _T("橘园"),                  // 对应Orange的name
        _T("行政楼"),                // 对应AdminBuld的name
        _T("电工电子中心"),          // 对应EleCen的name
        _T("梅园"),                  // 对应Plum的name
        _T("体育馆西（南工路丁字路口）"), // 对应Gym_W的name
        _T("教八站"),                // 对应J8的name
        _T("北门站"),                // 对应NorthGate的name
        _T("兰台"),                  // 对应LanTai的name
        _T("北门转盘"),              // 对应NorGateTurn的name
        _T("焦廷标馆东侧"),          // 对应JiaoE的name
        _T("东门转盘"),              // 对应EGateTurn的name
        _T("梅园转盘（西南角）"),    // 对应PlumTurn的name
        _T("苏源"),                  // 对应SuYuan的name
        _T("苏源（对面）"),          // 对应AntiSuYuan的name
        _T("无线谷"),                // 对应WiFi的name
        _T("东南路（桃园1-4路口）"), // 对应SERoad的name
        _T("桃园北食堂"),            // 对应Peach_N_Din的name
        _T("纪忠楼"),                // 对应JZBlock的name
        _T("东北门"),         // 对应NEGate的name
        _T("文学院")   //修改:添加文学院
    };

    // 向下拉框添加站点（保持与上面数组一致）
    const int stationCount = _countof(stations);
    for (int i = 0; i < stationCount; ++i)
    {
        m_cmbStart.AddString(stations[i]);
        m_cmbEnd.AddString(stations[i]);
    }

    // 默认选择初始站点
    m_cmbStart.SetCurSel(0);
    m_cmbEnd.SetCurSel(1);
}

void CCambusView::OnBnClickedQuery()
{
    // 确保结果对话框已创建
    if (!m_resultDialog.m_hWnd)
    {
        if (!m_resultDialog.Create(IDD_RESULT_DIALOG, this))
        {
            AfxMessageBox(_T("无法创建结果对话框！"), MB_ICONERROR);
            return;
        }
        m_resultDialog.SetWindowText(_T("查询结果"));
    }

    // 清除历史结果
    // 清除历史结果（CListCtrl 使用 DeleteAllItems 清除所有项）
    m_resultDialog.m_listResult.DeleteAllItems();

    // 获取并验证用户选择的日期时间   
    COleDateTime selectedDateTime;
    if (!GetSelectedDateTime(selectedDateTime))
        return;

    // 转换为Time结构体（适配外部接口）
    Time queryDate;
    queryDate.yr = selectedDateTime.GetYear();
    queryDate.month = selectedDateTime.GetMonth();
    queryDate.day = selectedDateTime.GetDay();

    const int specialType = IsSpecial(&queryDate);
    if (specialType == 1)
    {
        // 假期提示适配四列
        m_resultDialog.AddResult(_T(""), _T(""), _T(""), _T("假期时段，接驳车暂不运行"));
        m_resultDialog.ShowWindow(SW_SHOW);
        return;
    }

    // 判断是否为周末
    bool isWeekend = false;
    if (specialType == 2)
    {
        // 调休工作日（非周末）
        isWeekend = false;
    }
    else if (specialType == 3)
    {
        // 普通日期：非工作日即为周末
        isWeekend = !IsWorkday(&queryDate);
    }
    else
    {
        AfxMessageBox(_T("日期必须在2025年！"), MB_ICONWARNING);
        return;
    }

    // 获取并验证站点选择
    CString strStart, strEnd;
    m_cmbStart.GetWindowText(strStart);
    m_cmbEnd.GetWindowText(strEnd);

    if (strStart.IsEmpty() || strEnd.IsEmpty())
    {
        AfxMessageBox(_T("请选择出发地和目的地！"), MB_ICONWARNING);
        return;
    }

    if (strStart == strEnd)
    {
        AfxMessageBox(_T("出发地和目的地不能相同！"), MB_ICONWARNING);
        return;
    }

    // 执行查询并显示结果
    QueryData(strStart, strEnd, isWeekend);
    m_resultDialog.ShowWindow(SW_SHOW);
}

int GetWeekday(COleDateTime date) {
    int dayOfWeek = date.GetDayOfWeek(); // 1=周日，2=周一，...，7=周六
    if (dayOfWeek == 1) return 7; // 周日对应7
    else return dayOfWeek - 1; // 周一到周六对应1-6
}

void CCambusView::QueryData(CString strStart, CString strEnd, bool isWeekend)
{
    // 转换为UTF-8编码的std::string（匹配Searching中站点名编码）
 /*   std::string startStation = CT2A(strStart.GetBuffer(), CP_UTF8);
    std::string endStation = CT2A(strEnd.GetBuffer(), CP_UTF8);*/

    //修改:地点输入方式
    /*std::string startStation = CT2A(strStart);
    std::string endStation = CT2A(strEnd);*/

    // 转换CString为std::string（适配Searching.cpp的接口）
    std::string start = CW2A(strStart.GetBuffer());
    std::string end = CW2A(strEnd.GetBuffer());

    strStart.ReleaseBuffer();
    strEnd.ReleaseBuffer();

    // 获取查询时间（已在OnBnClickedQuery中验证）
    COleDateTime selectedDateTime;
    if (!GetSelectedDateTime(selectedDateTime)) return;

    // 构造Time对象（适配Searching接口）
    Time queryTime(
        selectedDateTime.GetYear(),
        selectedDateTime.GetMonth(),
        selectedDateTime.GetDay(),
        selectedDateTime.GetHour(),
        selectedDateTime.GetMinute(),
        GetWeekday(selectedDateTime) // 补全GetWeekday函数实现
    );

    // 调用Search方法查询所有可能路线（含换乘）
    Route route;
    Route* result = route.Search(queryTime, start, end);

    // 显示结果（区分直达和换乘）
    if (result == nullptr)
    {
        m_resultDialog.AddResult(_T(""), _T(""), _T(""), _T("未找到匹配路线"));
        return;
    }

    // 处理直达路线
    if (result->tran == nullptr) // 无换乘
    {
        CString line;
        line.Format(_T("%c线"), result->directLine);
        CString t1;
        t1.Format(_T("%02d:%02d"), result->t1.clock, result->t1.min);
        CString t2;
        t2.Format(_T("%02d:%02d"), result->t2.clock, result->t2.min);
        m_resultDialog.AddResult(line, t1, t2, _T("直达"));
    }
    else // 有换乘
    {
        // 第一程：起点 -> 换乘站
        CString line1;
        line1.Format(_T("%c线"), result->firstLine);
        // 第一程时间（起点→换乘站）
        CString T1;
        T1.Format(_T("%02d:%02d"), result->t1.clock, result->t1.min);  // 正确获取第一程出发时间
        CString tTranArrive;
        tTranArrive.Format(_T("%02d:%02d"), result->t_t1.clock, result->t_t1.min);  // 正确获取换乘站到达时间

        if (result->t1.clock == 0) {
            m_resultDialog.AddResult(_T(""), _T(""), _T(""), _T("未找到匹配路线"));
            return;
        }
        CString info1;
        CString strTranName = CA2W(result->tran->name.c_str());
        info1.Format(_T("换乘站：%s"), strTranName);
        m_resultDialog.AddResult(line1, T1, tTranArrive, info1);

        // 第二程：换乘站 -> 终点
        CString line2;
        line2.Format(_T("%c线"), result->secondLine);
        CString tTranDepart;
        tTranDepart.Format(_T("%02d:%02d"), result->t_t2.clock, result->t_t2.min);  // 修复后可正确获取换乘站出发时间
        CString T2;
        T2.Format(_T("%02d:%02d"), result->t2.clock, result->t2.min);  // 正确获取终点到达时间
        m_resultDialog.AddResult(line2, tTranDepart, T2, _T("换乘后直达终点"));
    }
}

//修改:添加视图更新函数
void CCambusView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    //获取关联的文档对象
    CCambusDoc* pDoc = (CCambusDoc*) GetDocument();  //转换指针类型
    if (!pDoc) return;

    // 更新下拉框选中项
    int startIndex = m_cmbStart.FindStringExact(0, pDoc->startStation); //在出发地下拉框里从头寻找与pDoc->startStation对应的结果,并返回索引
    //如果找到,将下拉框数据设为索引值
    if (startIndex != CB_ERR)
        m_cmbStart.SetCurSel(startIndex);   

    int endIndex = m_cmbEnd.FindStringExact(0, pDoc->endStation);   //同上注释
    if (endIndex != CB_ERR)
        m_cmbEnd.SetCurSel(endIndex);
}


//修改:添加出发地下拉框地点改变响应函数
void CCambusView::OnCbnSelchangeStart()
{
    CString strStart;
    m_cmbStart.GetWindowText(strStart);

    // 更新文档或同步图片上的起点
    CCambusDoc* pDoc = (CCambusDoc*)GetDocument();
    if (pDoc)
    {
        pDoc->startStation = strStart;
        // 触发视图更新
        pDoc->UpdateAllViews(NULL);
    }
}

//修改:添加目的地地下拉框地点改变响应函数
void CCambusView::OnCbnSelchangeEnd()
{
    CString strEnd;
    m_cmbEnd.GetWindowText(strEnd);

    // 更新文档或同步图片上的终点
    CCambusDoc* pDoc = (CCambusDoc*)GetDocument();
    if (pDoc)
    {
        pDoc->endStation = strEnd;
        pDoc->UpdateAllViews(NULL);
    }
}

void CCambusView::OnDateTimeChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
    COleDateTime selectedTime;
    if (GetSelectedDateTime(selectedTime))  // 获取控件当前值
    {
        CCambusDoc* pDoc = (CCambusDoc*)GetDocument();
        if (pDoc)
        {
            pDoc->m_selectedDateTime = selectedTime;  // 写入 Doc
            pDoc->UpdateAllViews(NULL);               // 通知 CImageView 更新
        }
    }

    *pResult = 0;
}

//添加:鼠标点击函数
void CCambusView::OnLButtonDown(UINT nFlags, CPoint point)
{
    // 判断点击是否在链接矩形内
    if (m_linkRect1.PtInRect(point))
    {
        ::ShellExecute(NULL, _T("open"), m_linkURL1, NULL, NULL, SW_SHOWNORMAL);
    }
    else if (m_linkRect2.PtInRect(point))
    {
        ::ShellExecute(NULL, _T("open"), m_linkURL2, NULL, NULL, SW_SHOWNORMAL);
    }
    CView::OnLButtonDown(nFlags, point); // 调用基类处理其他事件
}

void CCambusView::OnMouseMove(UINT nFlags, CPoint point)
{
    if(m_linkRect1.PtInRect(point) || m_linkRect2.PtInRect(point))
        ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
    else
        ::SetCursor(::LoadCursor(NULL, IDC_ARROW));

    CView::OnMouseMove(nFlags, point);// 调用基类处理其他事件
}