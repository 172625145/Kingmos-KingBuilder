#if !defined(AFX_PLATFORMSET_PAGE6_H__83ADC7B9_043D_4F18_8DD9_6DB44CFE823B__INCLUDED_)
#define AFX_PLATFORMSET_PAGE6_H__83ADC7B9_043D_4F18_8DD9_6DB44CFE823B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlatformSet_page6.h : header file
//
#include "DirectoryList.h"

/////////////////////////////////////////////////////////////////////////////
// CPlatformSet_page6 dialog

class CPlatformSet_page6 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPlatformSet_page6)

// Construction
public:
	void LoadCurrentSetting();
	void SetCurrentSetting();
	CPlatformSet_page6();
	~CPlatformSet_page6();

// Dialog Data
	//{{AFX_DATA(CPlatformSet_page6)
	enum { IDD = IDD_PLATFORMSET_PAGE6 };
//	CEdit	m_ppp;
	CString	m_strOutputFileName;
	int		m_nCategory;
	BOOL	m_bOutputMap;
	CString	m_strProjectOption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPlatformSet_page6)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
//	CDirectoryList *m_pctrlDirectoryList;
	// Generated message map functions
	//{{AFX_MSG(CPlatformSet_page6)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLATFORMSET_PAGE6_H__83ADC7B9_043D_4F18_8DD9_6DB44CFE823B__INCLUDED_)
