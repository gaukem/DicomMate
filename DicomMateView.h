
// DicomMateView.h : interface of the CDicomMateView class
//

#pragma once

#include <memory>

class CDicomMateView : public CView
{
protected: // create from serialization only
	CDicomMateView() noexcept;
	DECLARE_DYNCREATE(CDicomMateView)

// Attributes
public:
	CDicomMateDoc* GetDocument() const;

private:
	// View transformation parameters
	double m_zoomFactor;
	CPoint m_panOffset;
	
	// DIB for rendering
	BITMAPINFO* m_pBitmapInfo;
	BYTE* m_pDibBits;
	
	// Mouse interaction
	BOOL m_bPanning;
	CPoint m_lastMousePos;
	BOOL m_bWindowLeveling;
	
	// Zoom and window/level constants
	static constexpr double ZOOM_FACTOR_TOOLBAR = 1.25;
	static constexpr double ZOOM_FACTOR_WHEEL = 1.1;
	static constexpr double ZOOM_MIN = 0.1;
	static constexpr double ZOOM_MAX = 20.0;
	static constexpr double WINDOW_LEVEL_SENSITIVITY = 2.0;

// Operations
public:
	void UpdateDIB();
	void ResetView();
	void ZoomIn();
	void ZoomOut();
	void ZoomToFit();
	void SetPanMode(BOOL bEnable) { m_bPanning = bEnable; }
	void SetWindowLevelMode(BOOL bEnable) { m_bWindowLeveling = bEnable; }

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
	virtual ~CDicomMateView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DicomMateView.cpp
inline CDicomMateDoc* CDicomMateView::GetDocument() const
   { return reinterpret_cast<CDicomMateDoc*>(m_pDocument); }
#endif

