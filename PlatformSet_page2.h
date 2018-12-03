#if !defined(AFX_PLATFORMSET_PAGE2_H__0B5E9874_3040_4524_A52E_F99CE4A6EFC0__INCLUDED_)
#define AFX_PLATFORMSET_PAGE2_H__0B5E9874_3040_4524_A52E_F99CE4A6EFC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlatformSet_page2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlatformSet_page2 dialog

class CPlatformSet_page2 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPlatformSet_page2)

// Construction
public:
	CPlatformSet_page2();
	~CPlatformSet_page2();

// Dialog Data
	//{{AFX_DATA(CPlatformSet_page2)
	enum { IDD = IDD_PLATFORMSET_PAGE2 };
	CListCtrl	m_ctrlEnvironment;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPlatformSet_page2)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPlatformSet_page2)
	virtual BOOL OnInitDialog();
	afx_msg void OnNewenv();
	afx_msg void OnEditenv();
	afx_msg void OnDelenv();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLATFORMSET_PAGE2_H__0B5E9874_3040_4524_A52E_F99CE4A6EFC0__INCLUDED_)
