
// DicomMate.h : main header file for the DicomMate application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CDicomMateApp:
// See DicomMate.cpp for the implementation of this class
//

class CDicomMateApp : public CWinAppEx
{
public:
	CDicomMateApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	BOOL  m_bHiColorIcons;
	CMultiDocTemplate* m_pDocTemplate;

	CMultiDocTemplate* GetDocTemplate() { return m_pDocTemplate; }

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDicomMateApp theApp;
