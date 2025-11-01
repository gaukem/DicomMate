
// DicomMateView.cpp : implementation of the CDicomMateView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DicomMate.h"
#endif

#include "DicomMateDoc.h"
#include "DicomMateView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDicomMateView

IMPLEMENT_DYNCREATE(CDicomMateView, CView)

BEGIN_MESSAGE_MAP(CDicomMateView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDicomMateView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CDicomMateView construction/destruction

CDicomMateView::CDicomMateView() noexcept
{
	// TODO: add construction code here

}

CDicomMateView::~CDicomMateView()
{
}

BOOL CDicomMateView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CDicomMateView drawing

void CDicomMateView::OnDraw(CDC* /*pDC*/)
{
	CDicomMateDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CDicomMateView printing


void CDicomMateView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDicomMateView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDicomMateView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDicomMateView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CDicomMateView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDicomMateView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDicomMateView diagnostics

#ifdef _DEBUG
void CDicomMateView::AssertValid() const
{
	CView::AssertValid();
}

void CDicomMateView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDicomMateDoc* CDicomMateView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDicomMateDoc)));
	return (CDicomMateDoc*)m_pDocument;
}
#endif //_DEBUG


// CDicomMateView message handlers
