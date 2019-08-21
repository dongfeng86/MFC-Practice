
// TextView.cpp : CTextView ���ʵ��
//

#include "stdafx.h"
#include "Text.h"

#include "TextDoc.h"
#include "TextView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTextView

IMPLEMENT_DYNCREATE(CTextView, CView)

BEGIN_MESSAGE_MAP(CTextView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTextView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CTextView ����/����

CTextView::CTextView()
: m_strLine(_T(""))
, m_ptOrigin(0)
, m_iWidth(0)
{
	// TODO: �ڴ˴����ӹ������
}

CTextView::~CTextView()
{
}

BOOL CTextView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CTextView ����

void CTextView::OnDraw(CDC* pDC)
{
	CTextDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: �ڴ˴�Ϊ�����������ӻ��ƴ���

	CString str(_T("΢������"));
	pDC->TextOut(50,50,str);
	CSize sz=pDC->GetTextExtent(str);

	str.LoadString(IDS_WEIXIN);
	pDC->TextOut(50,100,str);

	//���´�����һ��·����
	pDC->BeginPath();
	pDC->Rectangle(50,50,50+sz.cx,50+sz.cy);
	pDC->EndPath();
	pDC->SelectClipPath(RGN_AND);

	for(int i=0;i<300;i+=10)
	{
		pDC->MoveTo(0,i);
		pDC->LineTo(300,i);
		pDC->MoveTo(i,0);
		pDC->LineTo(i,300);
	}
}


// CTextView ��ӡ

void CTextView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CTextView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CTextView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ���Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CTextView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ���Ӵ�ӡ����е���������
}

void CTextView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTextView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CTextView ���

#ifdef _DEBUG
void CTextView::AssertValid() const
{
	CView::AssertValid();
}

void CTextView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTextDoc* CTextView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTextDoc)));
	return (CTextDoc*)m_pDocument;
}
#endif //_DEBUG


// CTextView ��Ϣ��������

int CTextView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ�������ר�õĴ�������

	//�����豸������
	CClientDC dc(this);
	//�����ı���Ϣ�ṹ�����
	TEXTMETRIC tm;
	//����豸���������ı���Ϣ
	dc.GetTextMetrics(&tm);
	CreateSolidCaret(tm.tmAveCharWidth/8,tm.tmHeight);
	ShowCaret();
	
	//��ʾһ��λͼ��ʾ��
	//m_bitmap.LoadBitmap(IDB_BITMAP1);
	//CreateCaret(&m_bitmap);
	//ShowCaret();

	SetTimer(1,100,NULL);

	return 0;
}

//��Ӧ�ú�����Ҫ������view��ͼ����ʾ�ַ�
void CTextView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	CClientDC dc(this);

	CFont font;
	font.CreatePointFont(300,_T("�����п�"),NULL);
	CFont *pOldFont = dc.SelectObject(&font);

	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	if(0x0d==nChar)
	{
		m_strLine.Empty();
		m_ptOrigin.y+=tm.tmHeight;
	}
	else if(0x08==nChar)
	{
		COLORREF clr=dc.SetTextColor(dc.GetBkColor());
		dc.TextOut(m_ptOrigin.x,m_ptOrigin.y,m_strLine);
		m_strLine=m_strLine.Left(m_strLine.GetLength()-1);
		dc.SetTextColor(clr);
	}
	else
	{
		m_strLine+= (TCHAR)nChar;
	}

	dc.TextOut(m_ptOrigin.x,m_ptOrigin.y,m_strLine);

	CSize sz=dc.GetTextExtent(m_strLine);
	CPoint pt;
	pt.y=m_ptOrigin.y;
	pt.x=m_ptOrigin.x+sz.cx;
	SetCaretPos(pt);

	CView::OnChar(nChar, nRepCnt, nFlags);
}

void CTextView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	SetCaretPos(point);
	m_strLine.Empty();
	m_ptOrigin=point;
	CView::OnLButtonDown(nFlags, point);
}

void CTextView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ

	m_iWidth+=5;
	CClientDC dc(this);

	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	CRect rect;
	rect.left=0;
	rect.top=200;
	rect.right=m_iWidth;
	rect.bottom=rect.top+tm.tmHeight;

	dc.SetTextColor(RGB(255,0,0));
	CString str;
	str.LoadString(IDS_WEIXIN);
	dc.DrawText(str,rect,DT_RIGHT);

	CSize sz=dc.GetTextExtent(str);
	if(m_iWidth>sz.cx)
	{
		m_iWidth=0;
		//dc.SetTextColor(RGB(0,255,0));
		COLORREF clr=dc.SetTextColor(dc.GetBkColor());
		dc.TextOut(0,200,str);
	}

	CView::OnTimer(nIDEvent);
}