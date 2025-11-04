
// DicomMateDoc.h : interface of the CDicomMateDoc class
//


#pragma once

#include "DicomImage.h"

class CDicomMateDoc : public CDocument
{
protected: // create from serialization only
	CDicomMateDoc() noexcept;
	DECLARE_DYNCREATE(CDicomMateDoc)

// Attributes
public:
	CDicomImage m_dicomImage;

// Operations
public:
	BOOL LoadDicomFile(const CString& filePath);
	CDicomImage* GetDicomImage() { return &m_dicomImage; }

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CDicomMateDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
