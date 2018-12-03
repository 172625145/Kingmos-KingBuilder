#if !defined(AFX_NEWPLATFORM_SETP1_H__CB8B9D3A_B097_400E_B6D9_163451E4A039__INCLUDED_)
#define AFX_NEWPLATFORM_SETP1_H__CB8B9D3A_B097_400E_B6D9_163451E4A039__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewPlatform_Setp1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewPlatform_Setp1 dialog

class CNewPlatform_Setp1 : public CPropertyPage
{
	DECLARE_DYNCREATE(CNewPlatform_Setp1)

// Construction
public:
	BOOL Browse(CString &csPath);
	CNewPlatform_Setp1();
	~CNewPlatform_Setp1();

// Dialog Data
	//{{AFX_DATA(CNewPlatform_Setp1)
	enum { IDD = IDD_NEWPLATFORM_STEP1 };
	CString	m_strPlatformName;
	CString	m_strLocation;
	CString	m_strPlatformRoot;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CNewPlatform_Setp1)
	public:
	virtual LRESULT OnWizardNext();
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_strLocationRoot;
	// Generated message map functions
	//{{AFX_MSG(CNewPlatform_Setp1)
	afx_msg void OnBrowres();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangePlatformname();
	afx_msg void OnChangeLocation();
	afx_msg void OnChangePlatformroot();
	afx_msg void OnBrowres2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	BOOL CreateMultiDirectory(LPCTSTR lpPathName);

private:
	BOOL IsExistDirectory(CString strDirectory);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPLATFORM_SETP1_H__CB8B9D3A_B097_400E_B6D9_163451E4A039__INCLUDED_)
