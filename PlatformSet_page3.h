#if !defined(AFX_PLATFORMSET_PAGE3_H__4B762386_5310_451A_9899_555D2761D1DA__INCLUDED_)
#define AFX_PLATFORMSET_PAGE3_H__4B762386_5310_451A_9899_555D2761D1DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlatformSet_page3.h : header file
//
#include "DirectoryList.h"

/////////////////////////////////////////////////////////////////////////////
// CPlatformSet_page3 dialog

class CPlatformSet_page3 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPlatformSet_page3)

// Construction
public:
	void GetListBoxContext();
	void CopyStringArray(CStringArray &arrObj, CStringArray &arrSrc);
	void SetDirListToProject();
	void GetDirListFromProject();
	void InsertDirList();
	CPlatformSet_page3();
	~CPlatformSet_page3();

// Dialog Data
	//{{AFX_DATA(CPlatformSet_page3)
	enum { IDD = IDD_PLATFORMSET_PAGE3 };
	CEdit	m_ppp;
	int		m_nSel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPlatformSet_page3)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CStringArray m_arrIncludeDirList;  //存放当前设定的INCLUDE目录
	CStringArray m_arrLibraryDirList;  //存放当前设定的LIBRARY目录

	CDirectoryList *m_pctrlDirectoryList;

	int		m_nCurSel;

	// Generated message map functions
	//{{AFX_MSG(CPlatformSet_page3)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeSelectdirfor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLATFORMSET_PAGE3_H__4B762386_5310_451A_9899_555D2761D1DA__INCLUDED_)
