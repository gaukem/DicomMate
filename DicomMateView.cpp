
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
#include "DicomImage.h"

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
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CDicomMateView construction/destruction

CDicomMateView::CDicomMateView() noexcept
	: m_zoomFactor(1.0)
	, m_panOffset(0, 0)
	, m_pBitmapInfo(nullptr)
	, m_pDibBits(nullptr)
	, m_bPanning(FALSE)
	, m_bWindowLeveling(FALSE)
	, m_lastMousePos(0, 0)
{
	// TODO: add construction code here

}

CDicomMateView::~CDicomMateView()
{
	if (m_pBitmapInfo)
	{
		delete[] m_pBitmapInfo;
		m_pBitmapInfo = nullptr;
	}
	if (m_pDibBits)
	{
		delete[] m_pDibBits;
		m_pDibBits = nullptr;
	}
}

BOOL CDicomMateView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CDicomMateView drawing

void CDicomMateView::OnDraw(CDC* pDC)
{
	CDicomMateDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Check if we have a valid DICOM image
	CDicomImage* pDicomImage = pDoc->GetDicomImage();
	if (!pDicomImage || !pDicomImage->IsValid())
	{
		// Display message when no image is loaded
		CRect rect;
		GetClientRect(&rect);
		pDC->DrawText(_T("No DICOM image loaded. Open a DICOM file to begin."), 
			&rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		return;
	}

	// Update DIB if necessary
	if (!m_pDibBits)
	{
		UpdateDIB();
	}

	if (!m_pDibBits || !m_pBitmapInfo)
		return;

	// Get client area
	CRect clientRect;
	GetClientRect(&clientRect);

	// Calculate display size with zoom
	int imageWidth = pDicomImage->GetWidth();
	int imageHeight = pDicomImage->GetHeight();
	int displayWidth = static_cast<int>(imageWidth * m_zoomFactor);
	int displayHeight = static_cast<int>(imageHeight * m_zoomFactor);

	// Center the image with pan offset
	int xPos = (clientRect.Width() - displayWidth) / 2 + m_panOffset.x;
	int yPos = (clientRect.Height() - displayHeight) / 2 + m_panOffset.y;

	// Set stretch mode for better quality
	pDC->SetStretchBltMode(HALFTONE);
	pDC->SetBrushOrg(0, 0);

	// Draw the image
	::StretchDIBits(pDC->m_hDC,
		xPos, yPos, displayWidth, displayHeight,
		0, 0, imageWidth, imageHeight,
		m_pDibBits,
		m_pBitmapInfo,
		DIB_RGB_COLORS,
		SRCCOPY);
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

void CDicomMateView::UpdateDIB()
{
	CDicomMateDoc* pDoc = GetDocument();
	if (!pDoc)
		return;

	CDicomImage* pDicomImage = pDoc->GetDicomImage();
	if (!pDicomImage || !pDicomImage->IsValid())
		return;

	// Clean up old DIB
	if (m_pBitmapInfo)
	{
		delete[] m_pBitmapInfo;
		m_pBitmapInfo = nullptr;
	}
	if (m_pDibBits)
	{
		delete[] m_pDibBits;
		m_pDibBits = nullptr;
	}

	int width = pDicomImage->GetWidth();
	int height = pDicomImage->GetHeight();

	// Get pixel data from DICOM image (8-bit grayscale)
	const void* pixelData = pDicomImage->GetPixelData();
	if (!pixelData)
		return;

	// Create BITMAPINFO for 8-bit grayscale
	int bmiSize = sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
	m_pBitmapInfo = reinterpret_cast<BITMAPINFO*>(new BYTE[bmiSize]);
	ZeroMemory(m_pBitmapInfo, bmiSize);

	m_pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBitmapInfo->bmiHeader.biWidth = width;
	m_pBitmapInfo->bmiHeader.biHeight = -height; // Top-down DIB
	m_pBitmapInfo->bmiHeader.biPlanes = 1;
	m_pBitmapInfo->bmiHeader.biBitCount = 8;
	m_pBitmapInfo->bmiHeader.biCompression = BI_RGB;

	// Create grayscale palette
	for (int i = 0; i < 256; i++)
	{
		m_pBitmapInfo->bmiColors[i].rgbRed = static_cast<BYTE>(i);
		m_pBitmapInfo->bmiColors[i].rgbGreen = static_cast<BYTE>(i);
		m_pBitmapInfo->bmiColors[i].rgbBlue = static_cast<BYTE>(i);
		m_pBitmapInfo->bmiColors[i].rgbReserved = 0;
	}

	// Allocate and copy pixel data
	int dibSize = width * height;
	m_pDibBits = new BYTE[dibSize];
	memcpy(m_pDibBits, pixelData, dibSize);
}

void CDicomMateView::ResetView()
{
	m_zoomFactor = 1.0;
	m_panOffset = CPoint(0, 0);
	Invalidate();
}

void CDicomMateView::ZoomIn()
{
	m_zoomFactor *= 1.25;
	Invalidate();
}

void CDicomMateView::ZoomOut()
{
	m_zoomFactor /= 1.25;
	if (m_zoomFactor < 0.1)
		m_zoomFactor = 0.1;
	Invalidate();
}

void CDicomMateView::ZoomToFit()
{
	CDicomMateDoc* pDoc = GetDocument();
	if (!pDoc)
		return;

	CDicomImage* pDicomImage = pDoc->GetDicomImage();
	if (!pDicomImage || !pDicomImage->IsValid())
		return;

	CRect clientRect;
	GetClientRect(&clientRect);

	int imageWidth = pDicomImage->GetWidth();
	int imageHeight = pDicomImage->GetHeight();

	double zoomX = static_cast<double>(clientRect.Width()) / imageWidth;
	double zoomY = static_cast<double>(clientRect.Height()) / imageHeight;

	m_zoomFactor = min(zoomX, zoomY) * 0.95; // 95% to leave some margin
	m_panOffset = CPoint(0, 0);
	Invalidate();
}

void CDicomMateView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_lastMousePos = point;
	SetCapture();
	
	CView::OnLButtonDown(nFlags, point);
}

void CDicomMateView::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	
	CView::OnLButtonUp(nFlags, point);
}

void CDicomMateView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags & MK_LBUTTON && GetCapture() == this)
	{
		if (m_bPanning)
		{
			// Pan the image
			CPoint delta = point - m_lastMousePos;
			m_panOffset.x += delta.x;
			m_panOffset.y += delta.y;
			m_lastMousePos = point;
			Invalidate();
		}
		else if (m_bWindowLeveling)
		{
			// Adjust window/level
			CDicomMateDoc* pDoc = GetDocument();
			if (pDoc)
			{
				CDicomImage* pDicomImage = pDoc->GetDicomImage();
				if (pDicomImage && pDicomImage->IsValid())
				{
					CPoint delta = point - m_lastMousePos;
					
					double windowCenter = pDicomImage->GetWindowCenter();
					double windowWidth = pDicomImage->GetWindowWidth();
					
					// Adjust window center (horizontal movement)
					windowCenter += delta.x * 2.0;
					
					// Adjust window width (vertical movement)
					windowWidth += delta.y * 2.0;
					if (windowWidth < 1.0)
						windowWidth = 1.0;
					
					pDicomImage->SetWindowCenter(windowCenter);
					pDicomImage->SetWindowWidth(windowWidth);
					
					m_lastMousePos = point;
					
					// Update the DIB with new window/level
					UpdateDIB();
					Invalidate();
				}
			}
		}
	}
	
	CView::OnMouseMove(nFlags, point);
}

BOOL CDicomMateView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// Zoom with mouse wheel
	if (zDelta > 0)
	{
		m_zoomFactor *= 1.1;
	}
	else
	{
		m_zoomFactor /= 1.1;
		if (m_zoomFactor < 0.1)
			m_zoomFactor = 0.1;
	}
	
	Invalidate();
	
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CDicomMateView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	
	// Redraw on size change
	Invalidate();
}
