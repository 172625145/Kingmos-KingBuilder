#if !defined(AFX_INPUTPAGE_H__B327684A_483A_4DEA_B006_BBF5C2653EB0__INCLUDED_)
#define AFX_INPUTPAGE_H__B327684A_483A_4DEA_B006_BBF5C2653EB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputPage dialog

class CInputPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CInputPage)

// Construction
public:
	CInputPage();
	~CInputPage();
	CString	m_strFileName;
	BOOL Browse(CString &csPath);
// Dialog Data
	//{{AFX_DATA(CInputPage)
	enum { IDD = IDD_CREATEINPUT };
	CEdit	m_ctrlWorkSpaceName;
	CEdit	m_crtlSourceLocation;
	CButton	m_ctrlSourceBrowse;
	CEdit	m_ctrlLocation;
	CButton	m_ctrlBrowse;
	CListBox	m_ctrlProjectType;
	CComboBox	m_ctrlCPUSelect;
	int		m_CPUType;
	CString	m_strCPUType;
	CString	m_strLocation;
	CString	m_strWorkSpaceName;
	BOOL	m_bLoadFromSource;
	CString	m_strSourceLocation;
	//}}AFX_DATA
	CString m_strProjectType;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CInputPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString	m_strLocationRoot;
	BOOL CreateMultiDirectory(LPCTSTR lpPathName);
	BOOL IsExistDirectory(CString strDirectory);
	// Generated message map functions
	//{{AFX_MSG(CInputPage)
	afx_msg void OnBrowes();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCpuselect();
	afx_msg void OnLoadfromsource();
	afx_msg void OnSourcebrowes();
	afx_msg void OnChangeWorkspacesname();
	afx_msg void OnSelchangeProjecttype();
	afx_msg void OnChangeLocation();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTPAGE_H__B327684A_483A_4DEA_B006_BBF5C2653EB0__INCLUDED_)
