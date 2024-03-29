﻿
// AddToolBarNewDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "AddToolBarNew.h"
#include "AddToolBarNewDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAddToolBarNewDlg 对话框



CAddToolBarNewDlg::CAddToolBarNewDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADDTOOLBARNEW_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAddToolBarNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_ICON, m_wndBtnIcon);
}

BEGIN_MESSAGE_MAP(CAddToolBarNewDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CAddToolBarNewDlg 消息处理程序

BOOL CAddToolBarNewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	UINT buttonID[] = { ID_SELECT_BORDER,ID_SELECT_INNER };
	if (!m_wndToolBar.Create(WS_VISIBLE | WS_CHILD | CCS_BOTTOM | TBSTYLE_TOOLTIPS
		| CCS_ADJUSTABLE, CRect(0, 0, 0, 0), this, IDB_TOOLBAR))
	{
		TRACE0("failed to create toolbar\n");
		return -1;
	}

	const int iButtonNum = 2;
	TBBUTTON *pIndex, *pRTF_Button = new TBBUTTON[iButtonNum];
	pIndex = pRTF_Button;
	for (long lIndex = 0; lIndex < iButtonNum; lIndex++)
	{
		memset(pIndex, NULL, sizeof(TBBUTTON));
		pIndex->iBitmap = lIndex;
		pIndex->idCommand = buttonID[lIndex];
		pIndex->fsStyle = TBSTYLE_CHECKGROUP;
		pIndex->fsState = TBSTATE_ENABLED;
		pIndex++;
	}

	CSize size(16, 16);
	m_wndToolBar.SetButtonSize(size);
	m_wndToolBar.AddButtons(iButtonNum, pRTF_Button);
	delete[] pRTF_Button;

	m_wndToolBar.SetBitmapSize(size);
	m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT);

	//下面的方式加载的位图是黑的。
	//不知道是不是CToolBarCtrl::AddBitmap的bug，如果加载的是24位位图的ID，直接显示不出来，但是，
	//如果用CBitmap加载24位位图,然后再用CToolBarCtrl::AddBitmap加载位图的指针，则可以成功。
	//m_wndToolBar.AddBitmap(2, IDB_TOOLBAR);
	m_bmpToolBar.LoadBitmap(IDB_TOOLBAR);
	m_wndToolBar.AddBitmap(iButtonNum, &m_bmpToolBar);

	//方法一(只能加载32×32位ICON，不推荐)
	//m_hBtnIcon = AfxGetApp()->LoadIcon(IDI_ICON_BTN);

	//方法二（可以根据按钮尺寸定制ICON大小）
	CRect rect;
	m_wndBtnIcon.GetWindowRect(rect);
	m_hBtnIcon = (HICON)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_ICON_BTN), IMAGE_ICON, rect.Width(), rect.Height(), LR_DEFAULTCOLOR);
	m_wndBtnIcon.SetIcon(m_hBtnIcon);

	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CAddToolBarNewDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAddToolBarNewDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CAddToolBarNewDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAddToolBarNewDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}
