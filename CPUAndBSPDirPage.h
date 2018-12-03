#if !defined(AFX_CPUANDBSPDIRPAGE_H__44DB96C2_01BC_4177_A723_AEF4338405FE__INCLUDED_)
#define AFX_CPUANDBSPDIRPAGE_H__44DB96C2_01BC_4177_A723_AEF4338405FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPUAndBSPDirPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCPUAndBSPDirPage dialog

class CCPUAndBSPDirPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCPUAndBSPDirPage)

// Construction
public:
	CCPUAndBSPDirPage();
	~CCPUAndBSPDirPage();

// Dialog Data
	//{{AFX_DATA(CCPUAndBSPDirPage)
	enum { IDD = IDD_CPUANDBSPDIR };
	CListBox	m_ListCPUType;
	CString	m_strBSPPlatformName;
	CString	m_strCPUType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCPUAndBSPDirPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCPUAndBSPDirPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPUANDBSPDIRPAGE_H__44DB96C2_01BC_4177_A723_AEF4338405FE__INCLUDED_)
