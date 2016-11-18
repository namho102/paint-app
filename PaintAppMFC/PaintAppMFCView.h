
// PaintAppMFCView.h : interface of the CPaintAppMFCView class
//

#pragma once


class CPaintAppMFCView : public CView
{
protected: // create from serialization only
	CPaintAppMFCView();
	DECLARE_DYNCREATE(CPaintAppMFCView)

// Attributes
public:
	CPaintAppMFCDoc* GetDocument() const;
	CPoint pointStart;
	CPoint pointFrom;
	
	int pWidth;
	COLORREF m_Color;
	UINT chooseColor;
	UINT dType;

	CRect m_rect;
	CDC m_MDC;
	CBitmap m_bmpMDC;
	CDC m_SDC;
	CBitmap m_bmpSDC;

	CDC  m_lastDC[100];
	CBitmap m_lastBitmap[100];
	int index = 0;
	CRect m_selectRect;

	CString FileName;
	HBITMAP hBitmap;

	CPoint CursorLocation;
// Operations
public:
	void OnInitialUpdate();
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
// Implementation
public:
	virtual ~CPaintAppMFCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDrawTools(UINT nID);
	afx_msg void OnUpdateDrawTools(CCmdUI* pCmdUI);
	afx_msg void OnColors(UINT nID);
	afx_msg void OnUpdateColors(CCmdUI* pCmdUI);
	afx_msg void OnColorsPallette();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditCut();
	afx_msg void OnEditUndo();
};

#ifndef _DEBUG  // debug version in PaintAppMFCView.cpp
inline CPaintAppMFCDoc* CPaintAppMFCView::GetDocument() const
   { return reinterpret_cast<CPaintAppMFCDoc*>(m_pDocument); }
#endif

