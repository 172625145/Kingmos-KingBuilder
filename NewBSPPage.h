#if !defined(AFX_NEWBSPPAGE_H__1C9BEF9A_A79F_4514_A958_E8DCDEBCF466__INCLUDED_)
#define AFX_NEWBSPPAGE_H__1C9BEF9A_A79F_4514_A958_E8DCDEBCF466__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewBSPPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewBSPPage dialog

class CNewBSPPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CNewBSPPage)

// Construction
public:
	CNewBSPPage();
	~CNewBSPPage();

// Dialog Data
	//{{AFX_DATA(CNewBSPPage)
	enum { IDD = IDD_NEWBSP };
	CString	m_strDescription;
	CString	m_strNewBSPName;
	CString	m_strVendor;
	CString	m_strVersion0;
	CString	m_strVersion1;
	CString	m_strVersion2;
	CString	m_strVersion3;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CNewBSPPage)
	public:
	virtual BOOL OnWizardFinish();
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CNewBSPPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWBSPPAGE_H__1C9BEF9A_A79F_4514_A958_E8DCDEBCF466__INCLUDED_)
