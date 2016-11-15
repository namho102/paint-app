
// PaintAppMFCView.cpp : implementation of the CPaintAppMFCView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "PaintAppMFC.h"
#endif

#include "PaintAppMFCDoc.h"
#include "PaintAppMFCView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPaintAppMFCView

IMPLEMENT_DYNCREATE(CPaintAppMFCView, CView)

BEGIN_MESSAGE_MAP(CPaintAppMFCView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	//ON_COMMAND(ID_TOOLS_FREE, &CPaintAppMFCView::OnToolsFree)
	ON_COMMAND_RANGE(ID_TOOLS_FREE, ID_BUTTON_MAX, OnDrawTools)

	ON_COMMAND_RANGE(ID_COLORS_BLACK, ID_COLORS_MAGENTA, OnColors)

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_FILE_NEW, &CPaintAppMFCView::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CPaintAppMFCView::OnFileOpen)
END_MESSAGE_MAP()

// CPaintAppMFCView construction/destruction

CPaintAppMFCView::CPaintAppMFCView()
{
	// TODO: add construction code here

}

CPaintAppMFCView::~CPaintAppMFCView()
{
}

BOOL CPaintAppMFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CPaintAppMFCView drawing

void CPaintAppMFCView::OnDraw(CDC* pDC)
{
	CPaintAppMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	// The BitBlt function performs a bit-block transfer of the color data corresponding to a rectangle of pixels
	// from the specified source device context into a destination device context.

	pDC->BitBlt(0, 0, m_rect.Width() + 500, m_rect.Height() + 500, &m_SDC, 0, 0, SRCCOPY);
	// TODO: add draw code for native data here
}


// CPaintAppMFCView printing

BOOL CPaintAppMFCView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPaintAppMFCView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPaintAppMFCView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CPaintAppMFCView diagnostics

#ifdef _DEBUG
void CPaintAppMFCView::AssertValid() const
{
	CView::AssertValid();
}

void CPaintAppMFCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPaintAppMFCDoc* CPaintAppMFCView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPaintAppMFCDoc)));
	return (CPaintAppMFCDoc*)m_pDocument;
}
#endif //_DEBUG

void CPaintAppMFCView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	GetClientRect(&m_rect);
	CClientDC dc(this);
	FileName = "";

	//The CreateCompatibleDC function creates a memory device context(DC) compatible with the specified device.

	m_MDC.CreateCompatibleDC(&dc);
	m_bmpMDC.CreateCompatibleBitmap(&dc, m_rect.Width() + 500, m_rect.Height() + 500);
	m_MDC.SelectObject(&m_bmpMDC);

	m_SDC.CreateCompatibleDC(&dc);
	m_bmpSDC.CreateCompatibleBitmap(&dc, m_rect.Width() + 500, m_rect.Height() + 500);
	m_SDC.SelectObject(&m_bmpSDC);

	m_MDC.FillSolidRect(0, 0, m_rect.Width() + 500, m_rect.Height() + 500, RGB(255, 255, 255));
	m_SDC.FillSolidRect(0, 0, m_rect.Width() + 500, m_rect.Height() + 500, RGB(255, 255, 255));


	for (int i = 0; i < 100; i = i + 1) {
		m_lastDC[i].CreateCompatibleDC(&dc);
		m_lastBitmap[i].CreateCompatibleBitmap(&dc, m_rect.Width(), m_rect.Height());
		m_lastDC[i].SelectObject(&m_lastBitmap[i]);
		m_lastDC[i].FillSolidRect(0, 0, m_rect.Width(), m_rect.Height(), RGB(255, 255, 255));
	}

	hBitmap = ::CreateCompatibleBitmap(m_MDC, 1400, 600);
}

// CPaintAppMFCView message handlers
//void CPaintAppMFCView::OnToolsFree()
//{
//	pWidth = 3;
//	// TODO: Add your command handler code here
//}

void CPaintAppMFCView::OnDrawTools(UINT nID) {
	dType = nID;
	pWidth = 1;
}

void CPaintAppMFCView::OnColors(UINT nID) {
	chooseColor = nID;
	static COLORREF crColors[8] = {
		RGB(0, 0, 0),
		RGB(255, 255, 255),
		RGB(255, 255, 0),
		RGB(255, 0, 0),
		RGB(0, 255, 0),
		RGB(0, 0, 255),
		RGB(0, 255, 255),
		RGB(255, 0, 255)
		
	};
	m_Color = crColors[chooseColor - ID_COLORS_BLACK];
}

void CPaintAppMFCView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//MessageBox(TEXT("wtf"), 0, MB_OK);
	m_lastDC[index++].BitBlt(0, 0, m_rect.Width(), m_rect.Height(), &m_MDC, 0, 0, SRCCOPY);
	pointFrom = pointStart = point;
	
	switch (dType)
	{
	case ID_TOOLS_FREE: {
		m_MDC.MoveTo(pointStart);
		m_MDC.LineTo(point);
		pointStart = point;
		break;
	}
	case ID_TOOLS_SPRAY: {
		for (int i = 0; i < 50; i++) {	
			int dx = rand() % (31) - 10;
			int dy = rand() % (31) - 10;
			m_MDC.SetPixel(point.x + dx, point.y + dy, m_Color);
		}
		break;
	}
	default:
		break;
	}
	m_MDC.MoveTo(pointStart);
	m_MDC.LineTo(point);
	pointStart = point;
	
	Invalidate(false);
}


void CPaintAppMFCView::OnLButtonUp(UINT nFlags, CPoint point)
{
	
	CPen Pen(PS_SOLID, pWidth, m_Color);
	m_MDC.SelectObject(Pen);
	m_MDC.SelectStockObject(NULL_BRUSH);
	
	m_SDC.BitBlt(0, 0, m_rect.Width() + 500, m_rect.Height() + 500, &m_MDC, 0, 0, SRCCOPY);
	Invalidate(false);
}


void CPaintAppMFCView::OnMouseMove(UINT nFlags, CPoint point)
{	
	if (dType == ID_TOOLS_ERASE) {
		//ShowCursor(false);
		m_SDC.BitBlt(0, 0, m_rect.Width() + 500, m_rect.Height() + 500, &m_MDC, 0, 0, SRCCOPY);
		CPen Pen(PS_SOLID, 1, RGB(0, 0, 0));
		m_SDC.SelectObject(Pen);
		m_SDC.SelectStockObject(NULL_BRUSH);
		m_SDC.Rectangle(point.x - 5, point.y - 5, point.x + 5, point.y + 5);
	}
	
	if (nFlags && MK_LBUTTON) {
		//if (dType == ID_TOOLS_ERASE) {
		//	CPen Pen(PS_SOLID, pWidth, m_Color);
		//	m_SDC.SelectObject(Pen);
		//	m_SDC.SelectStockObject(NULL_BRUSH);
		//	m_MDC.SelectObject(Pen);
		//	m_MDC.SelectStockObject(NULL_BRUSH);
		//	m_SDC.BitBlt(0, 0, m_rect.Width() + 500, m_rect.Height() + 500, &m_MDC, 0, 0, SRCCOPY);
		//
		//	m_MDC.FillSolidRect(point.x - 5, point.y - 5, 10, 10, RGB(255, 255, 152));
		//}
		//else {
		//	
		//}
		
		CPen Pen(PS_SOLID, pWidth, m_Color);
		m_SDC.SelectObject(Pen);
		m_SDC.SelectStockObject(NULL_BRUSH);
		m_MDC.SelectObject(Pen);
		m_MDC.SelectStockObject(NULL_BRUSH);
		m_SDC.BitBlt(0, 0, m_rect.Width() + 500, m_rect.Height() + 500, &m_MDC, 0, 0, SRCCOPY);
		switch (dType)
		{
		case ID_TOOLS_FREE: {
			m_MDC.MoveTo(pointStart);
			m_MDC.LineTo(point);
			pointStart = point;
			break;
		}
		case ID_TOOLS_ERASE: {
			//CPen pBrush(PS_SOLID, pWidth + 9, m_Color);
			//m_MDC.SelectObject(pBrush);
			//m_MDC.MoveTo(pointStart);
			//m_MDC.LineTo(point);
			//pointStart = point;
			m_MDC.FillSolidRect(point.x - 5, point.y - 5, 10, 10, m_Color);
			break;
		}
		case ID_TOOLS_SPRAY: {
			for (int i = 0; i < 100; i++) {
				//int dx = rand() % (41) - 20;
				//int dy = rand() % (41) - 20;
				int dx = rand() % (31) - 10;
				int dy = rand() % (31) - 10;
				m_MDC.SetPixel(point.x + dx, point.y + dy, m_Color);
			}
			break;
		}
		default:
			break;
		}
		
		
	
	}
	Invalidate(false);

}


void CPaintAppMFCView::OnFileNew()
{
	m_MDC.FillSolidRect(0, 0, m_rect.Width() + 500, m_rect.Height() + 500, RGB(255, 255, 255));
	m_SDC.FillSolidRect(0, 0, m_rect.Width() + 500, m_rect.Height() + 500, RGB(255, 255, 255));
	FileName = "";
	Invalidate(true);
}


void CPaintAppMFCView::OnFileOpen()
{
	// TODO: Add your command handler code here
}
