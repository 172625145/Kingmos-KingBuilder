#if !defined(AFX_NEWPLATFORM_SETP2_H__0001D934_5B7F_4CF8_A932_C47563308EE5__INCLUDED_)
#define AFX_NEWPLATFORM_SETP2_H__0001D934_5B7F_4CF8_A932_C47563308EE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewPlatform_Setp2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewPlatform_Setp2 dialog

class CNewPlatform_Setp2 : public CPropertyPage
{
	DECLARE_DYNCREATE(CNewPlatform_Setp2)

// Construction
public:
	CNewPlatform_Setp2();
	~CNewPlatform_Setp2();

public:
	CString m_strBSPFileName;
// Dialog Data
	//{{AFX_DATA(CNewPlatform_Setp2)
	enum { IDD = IDD_NEWPLATFORM_STEP2 };
	CListBox	m_listBSPs;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CNewPlatform_Setp2)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CNewPlatform_Setp2)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeBsplist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPLATFORM_SETP2_H__0001D934_5B7F_4CF8_A932_C47563308EE5__INCLUDED_)
