
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
	
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND_RANGE(ID_TOOLS_FREE, ID_TOOLS_ZOOM,OnDrawTools)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TOOLS_FREE, ID_TOOLS_ZOOM, OnUpdateDrawTools)
	ON_COMMAND_RANGE(ID_COLORS_BLACK, ID_COLORS_WHITE,OnColors)
	ON_UPDATE_COMMAND_UI_RANGE(ID_COLORS_BLACK, ID_COLORS_PALLETTE, OnUpdateColors)
	ON_COMMAND(ID_COLORS_PALLETTE,OnColorsPallette)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_FILE_NEW, &CPaintAppMFCView::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CPaintAppMFCView::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CPaintAppMFCView::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CPaintAppMFCView::OnFileSaveAs)
	ON_COMMAND(ID_EDIT_COPY, &CPaintAppMFCView::OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CPaintAppMFCView::OnEditPaste)
	ON_COMMAND(ID_EDIT_CUT, &CPaintAppMFCView::OnEditCut)
	ON_COMMAND(ID_EDIT_UNDO, &CPaintAppMFCView::OnEditUndo)
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

void CPaintAppMFCView::OnDraw(CDC *pDC)
{
	CPaintAppMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
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
void CPaintAppMFCView::OnInitialUpdate(){
	CView::OnInitialUpdate();
	GetClientRect(&m_rect);
	CClientDC dc(this);	
	FileName = "";
	
	m_MDC.CreateCompatibleDC(&dc);
	m_bmpMDC.CreateCompatibleBitmap(&dc, m_rect.Width() + 500, m_rect.Height() + 500);
	m_MDC.SelectObject(&m_bmpMDC);

	m_SDC.CreateCompatibleDC(&dc);
	m_bmpSDC.CreateCompatibleBitmap(&dc, m_rect.Width() + 500, m_rect.Height() + 500);
	m_SDC.SelectObject(&m_bmpSDC);

	m_MDC.FillSolidRect(0, 0, m_rect.Width() + 500, m_rect.Height() + 500, RGB(255, 255, 255));
	m_SDC.FillSolidRect(0, 0, m_rect.Width() + 500, m_rect.Height() + 500, RGB(255, 255, 255));


	for (int i = 0; i < 100; i = i + 1){
		m_lastDC[i].CreateCompatibleDC(&dc);
		m_lastBitmap[i].CreateCompatibleBitmap(&dc, m_rect.Width(), m_rect.Height());
		m_lastDC[i].SelectObject(&m_lastBitmap[i]);
		m_lastDC[i].FillSolidRect(0, 0, m_rect.Width(), m_rect.Height(), RGB(255, 255, 255));
	}

	hBitmap = ::CreateCompatibleBitmap(m_MDC, 1400, 600);
}
void CPaintAppMFCView::OnDrawTools(UINT nID){
	dType = nID;
	pWidth = 1;
}
void CPaintAppMFCView::OnUpdateDrawTools(CCmdUI* pCmdUI){
	pCmdUI->SetRadio(pCmdUI->m_nID == dType);
}
void CPaintAppMFCView::OnColors(UINT nID){
	chooseColor = nID;
	static COLORREF crColors[8] = {
		RGB(0, 0, 0),
		RGB(0, 0, 255),
		RGB(0, 255, 0),
		RGB(0, 255, 255),
		RGB(255, 0, 0),
		RGB(255, 0, 255),
		RGB(255, 255, 0),
		RGB(255, 255, 255)
	};
	m_Color=crColors[chooseColor - ID_COLORS_BLACK];
}
void CPaintAppMFCView::OnUpdateColors(CCmdUI* pCmdUI){
	pCmdUI->SetRadio(pCmdUI->m_nID == chooseColor);
}
void CPaintAppMFCView::OnColorsPallette(){
	chooseColor = ID_COLORS_PALLETTE;
	CColorDialog dlg(RGB(255, 0, 0), CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_Color = dlg.GetColor();
	}
}
void CPaintAppMFCView::OnLButtonDown(UINT nFlags, CPoint point){
	m_lastDC[index++].BitBlt(0, 0, m_rect.Width(), m_rect.Height(), &m_MDC, 0, 0, SRCCOPY);
	pointFrom= pointStart = point;
	if (dType == ID_TOOLS_FILL){
		CBrush br(m_Color);
		m_MDC.SelectObject(br);
		m_MDC.ExtFloodFill(point.x, point.y, m_MDC.GetPixel(point),FLOODFILLSURFACE);	
	}
	if (dType == ID_TOOLS_FREE){
		m_MDC.MoveTo(pointStart);
		m_MDC.LineTo(point);
		pointStart = point;
	}
	if (dType == ID_TOOLS_AIRBRUSH){
		for (int i = 0; i < 50; i++){
			int dx = rand() % (41) - 20;
			int dy = rand() % (41) - 20;
			m_MDC.SetPixel(point.x + dx, point.y + dy, m_Color);
		}
	}
	if (dType == ID_TOOLS_ZOOM){
		CRect rect;
		rect.SetRect(point.x - 70, point.y - 50, point.x + 70, point.y + 50);
		m_MDC.StretchBlt(point.x - 70, point.y - 50, 140, 100, &m_MDC, point.x - 35, point.y - 25, 70, 50, SRCCOPY);
	}
	Invalidate(false);
}
void CPaintAppMFCView::OnLButtonUp(UINT nFlags, CPoint point){
	CPen Pen(PS_SOLID,pWidth, m_Color);
	m_MDC.SelectObject(Pen);
	m_MDC.SelectStockObject(NULL_BRUSH);
	if (dType == ID_TOOLS_LINE){
		m_MDC.MoveTo(pointFrom);
		m_MDC.LineTo(point);
	}
	if (dType == ID_TOOLS_CURVE){
		CRect rect;
		rect.SetRect(pointFrom, point);
		m_MDC.Arc(rect, pointFrom, point);
	}
	if (dType == ID_TOOLS_CIRCLE){
		m_MDC.Ellipse(pointFrom.x, pointFrom.y, point.x, point.y);
	}
	if (dType == ID_TOOLS_RECTANGLE){
		m_MDC.Rectangle(pointFrom.x,pointFrom.y,point.x,point.y);
	}
	if (dType == ID_TOOLS_TEXT){
		CRect editRect;
		editRect.SetRect(pointFrom, point);
		CEdit *m_edit = new CEdit();
		m_edit->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP |
			ES_AUTOHSCROLL | WS_BORDER, editRect, this, IDC_EXTRA_EDIT);
		m_edit->SetFocus();
		delete(m_edit);
	}
	if (dType == ID_TOOLS_SELECT){
		m_selectRect.SetRect(pointFrom, point);
	}
	m_SDC.BitBlt(0, 0, m_rect.Width() + 500, m_rect.Height() + 500, &m_MDC, 0, 0, SRCCOPY);
	Invalidate(false);
}
void CPaintAppMFCView::OnMouseMove(UINT nFlags, CPoint point){
	if (dType == ID_TOOLS_ZOOM){
		m_SDC.BitBlt(0, 0, m_rect.Width() + 500, m_rect.Height() + 500, &m_MDC, 0, 0, SRCCOPY);
		CPen Pen(PS_SOLID, 1, RGB(0,0,0));
		m_SDC.SelectObject(Pen);
		m_SDC.SelectStockObject(NULL_BRUSH);
		m_SDC.Rectangle(point.x - 70, point.y - 50, point.x + 70, point.y + 50);
	}
	if (nFlags&&MK_LBUTTON){
		CPen Pen(PS_SOLID, pWidth, m_Color);
		m_SDC.SelectObject(Pen);
		m_SDC.SelectStockObject(NULL_BRUSH);
		m_MDC.SelectObject(Pen);
		m_MDC.SelectStockObject(NULL_BRUSH);
		m_SDC.BitBlt(0, 0, m_rect.Width() + 500, m_rect.Height() + 500, &m_MDC, 0, 0, SRCCOPY);
		if (dType == ID_TOOLS_FREE){
			m_MDC.MoveTo(pointStart);
			m_MDC.LineTo(point);
			pointStart = point;
		}
		if (dType == ID_TOOLS_BRUSH){
			CPen pBrush(PS_SOLID, pWidth + 9, m_Color);
			m_MDC.SelectObject(pBrush);
			m_MDC.MoveTo(pointStart);
			m_MDC.LineTo(point);
			pointStart = point;
		}
		if (dType == ID_TOOLS_AIRBRUSH){
			for (int i = 0; i < 50; i++){
				int dx = rand() % (41) - 20;
				int dy = rand() % (41) - 20;
				m_MDC.SetPixel(point.x + dx, point.y + dy, m_Color);
			}
		}
		if (dType == ID_TOOLS_LINE){
			m_SDC.MoveTo(pointFrom);
			m_SDC.LineTo(point);
		}
		if (dType == ID_TOOLS_CURVE){
			CRect rect;
			rect.SetRect(pointFrom, point);
			m_SDC.Arc(rect, pointFrom, point);
		}
		if (dType == ID_TOOLS_CIRCLE){
			m_SDC.Ellipse(pointFrom.x, pointFrom.y, point.x, point.y);
		}
		if (dType == ID_TOOLS_RECTANGLE){
			m_SDC.Rectangle(pointFrom.x, pointFrom.y, point.x, point.y);
		}
		if (dType == ID_TOOLS_TEXT){
			m_SDC.Rectangle(pointFrom.x, pointFrom.y, point.x, point.y);
		}
		if (dType == ID_TOOLS_SELECT){
			m_SDC.Rectangle(pointFrom.x, pointFrom.y, point.x, point.y);
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
	TCHAR strFilter[] = { TEXT("Picture Files (*.bmp)|*.bmp||") };
	CFileDialog dlg(TRUE, TEXT("*.bmp"), NULL, 0, strFilter);
	if (dlg.DoModal() == IDOK){
		FileName = dlg.GetPathName();
		HBITMAP hBmp = (HBITMAP)::LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		CBitmap bmp;
		bmp.Attach(hBmp);
		BITMAP bm;
		bmp.GetBitmap(&bm);
		CClientDC dc(this);
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		CBitmap *Oldbmp = memDC.SelectObject(&bmp);
		CRect rect;
		GetClientRect(&rect);
		m_MDC.StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
		m_SDC.BitBlt(0, 0, m_rect.Width() + 500, m_rect.Height() + 500, &m_MDC, 0, 0, SRCCOPY);
		memDC.SelectObject(Oldbmp);
		bmp.DeleteObject();
		memDC.DeleteDC();
		Invalidate(false);
	}
}


void CPaintAppMFCView::OnFileSave()
{
	if (FileName != ""){
		CRect rect;
		GetClientRect(&rect);
		CDC * pDC = GetDC();
		CDC MemDC;
		MemDC.CreateCompatibleDC(pDC);
		CBitmap bmp;
		bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
		MemDC.SelectObject(&bmp);
		MemDC.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);
		CImage image;
		image.Attach((HBITMAP)bmp.Detach());
		image.Save(FileName);
	}
	else OnFileSaveAs();
}




void CPaintAppMFCView::OnFileSaveAs()
{
	TCHAR strFilter[] = { TEXT("Picture Files (*.bmp)|*.bmp||") };
	CFileDialog dlg(FALSE, TEXT("*.bmp"), NULL, 0, strFilter);
	if (dlg.DoModal() == IDOK){
		FileName = dlg.GetPathName();
		if (FileName == "")return;
		OnFileSave();
	}
}


void CPaintAppMFCView::OnEditCopy()
{
	if (OpenClipboard()){
		EmptyClipboard();
		CClientDC dc(this);
		CDC memDC;
		CBitmap bmp;
		memDC.CreateCompatibleDC(&dc);
		bmp.CreateCompatibleBitmap(&dc, m_selectRect.Width(), m_selectRect.Height());
		CBitmap *pOldbmp = memDC.SelectObject(&bmp);
		memDC.BitBlt(0, 0, m_selectRect.Width(),
			m_selectRect.Height(), &dc, m_selectRect.left, m_selectRect.top, SRCCOPY);
		SetClipboardData(CF_BITMAP, bmp.m_hObject);
		memDC.SelectObject(pOldbmp);
		bmp.DeleteObject();
		memDC.DeleteDC();
	}
}


void CPaintAppMFCView::OnEditPaste()
{
	if (OpenClipboard()){
		HBITMAP hBmp = (HBITMAP)GetClipboardData(CF_BITMAP);
		CBitmap bmp;
		bmp.Attach(hBmp);
		BITMAP bi;
		bmp.GetBitmap(&bi);

		CClientDC dc(this);
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		CBitmap *pOldbmp = memDC.SelectObject(&bmp);
		m_MDC.BitBlt(0, 0, bi.bmWidth, bi.bmHeight, &memDC, 0, 0, SRCCOPY);
		m_SDC.BitBlt(0, 0, bi.bmWidth, bi.bmHeight, &memDC, 0, 0, SRCCOPY);
		Invalidate(false);

		memDC.SelectObject(pOldbmp);
		bmp.DeleteObject();
		memDC.DeleteDC();

	}
}


void CPaintAppMFCView::OnEditCut()
{
	if (OpenClipboard()){
		EmptyClipboard();
		CClientDC dc(this);
		CDC memDC;
		CBitmap bmp;
		memDC.CreateCompatibleDC(&dc);
		bmp.CreateCompatibleBitmap(&dc, m_selectRect.Width(), m_selectRect.Height());
		CBitmap *pOldbmp = memDC.SelectObject(&bmp);
		memDC.BitBlt(0, 0, m_selectRect.Width(),
			m_selectRect.Height(), &dc, m_selectRect.left, m_selectRect.top, SRCCOPY);

		m_MDC.FillSolidRect(m_selectRect, RGB(255, 255, 255));
		m_SDC.FillSolidRect(m_selectRect, RGB(255, 255, 255));

		SetClipboardData(CF_BITMAP, bmp.m_hObject);
		memDC.SelectObject(pOldbmp);
		bmp.DeleteObject();
		memDC.DeleteDC();
		Invalidate(false);
	}
}


void CPaintAppMFCView::OnEditUndo()
{
	m_MDC.BitBlt(0, 0, m_rect.Width(), m_rect.Height(), &m_lastDC[--index], 0, 0, SRCCOPY);
	m_SDC.BitBlt(0, 0, m_rect.Width(), m_rect.Height(), &m_lastDC[index], 0, 0, SRCCOPY);
	Invalidate(false);
}
