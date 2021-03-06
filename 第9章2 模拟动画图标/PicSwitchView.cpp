
// PicSwitchView.cpp : CPicSwitchView 类的实现
//

#include "stdafx.h"
#include "PicSwitch.h"

#include "PicSwitchDoc.h"
#include "PicSwitchView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPicSwitchView

IMPLEMENT_DYNCREATE(CPicSwitchView, CView)

BEGIN_MESSAGE_MAP(CPicSwitchView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPicSwitchView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CPicSwitchView 构造/析构

CPicSwitchView::CPicSwitchView()
{
	// TODO: 在此处添加构造代码

}

CPicSwitchView::~CPicSwitchView()
{
}

BOOL CPicSwitchView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CPicSwitchView 绘制

void CPicSwitchView::OnDraw(CDC* /*pDC*/)
{
	CPicSwitchDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CPicSwitchView 打印


void CPicSwitchView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CPicSwitchView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CPicSwitchView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CPicSwitchView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CPicSwitchView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPicSwitchView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CPicSwitchView 诊断

#ifdef _DEBUG
void CPicSwitchView::AssertValid() const
{
	CView::AssertValid();
}

void CPicSwitchView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPicSwitchDoc* CPicSwitchView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPicSwitchDoc)));
	return (CPicSwitchDoc*)m_pDocument;
}
#endif //_DEBUG


// CPicSwitchView 消息处理程序
