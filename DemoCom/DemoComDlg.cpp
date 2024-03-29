﻿
// DemoComDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "DemoCom.h"
#include "DemoComDlg.h"
#include "afxdialogex.h"
#include "CExcelOperation.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDemoComDlg 对话框



CDemoComDlg::CDemoComDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DEMOCOM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDemoComDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_WRAPTEXT, m_wndEdtWrapText);
}

BEGIN_MESSAGE_MAP(CDemoComDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_TEST, &CDemoComDlg::OnBnClickedBtnTest)
END_MESSAGE_MAP()


// CDemoComDlg 消息处理程序

BOOL CDemoComDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDemoComDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDemoComDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDemoComDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDemoComDlg::OnBnClickedBtnTest()
{
	// TODO: 在此添加控件通知处理程序代码
	//ExcelOperation* excel = ExcelOperation::getInstance();
	//excel->init();
	//excel->openExcelFile("C:\\test.xlsx");
	//excel->OpenSheet("NewSheet");
	//excel->setCellFormat("A1", "@");
	//excel->setCellValue("A1", "1234567890123456789");
	//excel->setCellFormat("A2", "0.00");
	//excel->setCellValue("A2", "3342.5684");
	//excel->setView(true);
	//excel->saveExcel();
	//excel->saveExcelAs("C:\\saveTest.xlsx");
	//excel->Close();


	CExcelOperation* excel = CExcelOperation::getInstance();
	excel->init();
	excel->openExcelFile("C:\\test.xlsx");
	excel->OpenSheet("NewSheet");
	CString sContent = excel->ReadCell("A1");
	excel->GetCellFont("A1", m_sFontName, m_dPointSize);
	double dCellWidth;
	excel->GetCellWidth("A1", dCellWidth);
	bool bbbb = excel->IsWrapCell("A1");
	CString ss=	CExcelOperation::GetAutoWrapText(sContent, dCellWidth, m_dPointSize, m_sFontName);

	SetEditTExt(sContent, m_dPointSize, m_sFontName,dCellWidth);
	CString s2 = excel->ReadCell("B1");
	excel->setView(true);

	excel->destroyInstance();
}

void CDemoComDlg::SetEditTExt(LPCTSTR pszText, int iPointSize, LPCTSTR szFontName, double dWidth)
{
	CRect rectEdit;
	m_wndEdtWrapText.GetWindowRect(&rectEdit);
	ScreenToClient(rectEdit);
	rectEdit.right = rectEdit.left + dWidth;
	m_wndEdtWrapText.MoveWindow(rectEdit);

	CString sEdtContent = pszText;
	sEdtContent.Replace(_T("\n"), _T("\r\n"));

	CFont font;
	DWORD dw;
	int iLow;
	if (font.CreatePointFont(iPointSize * 10, szFontName, NULL))
	{
		m_wndEdtWrapText.SetFont(&font);
		dw = m_wndEdtWrapText.GetMargins();
		iLow = LOWORD(dw);
		m_wndEdtWrapText.SetMargins(iLow, iLow);
		m_wndEdtWrapText.SetWindowText(sEdtContent);
	}
}
