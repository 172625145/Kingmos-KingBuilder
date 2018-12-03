#if !defined(AFX_PLATFORMSET_PAGE5_H__9FFB02B6_0AFC_46A5_B01A_43471E5A2F4A__INCLUDED_)
#define AFX_PLATFORMSET_PAGE5_H__9FFB02B6_0AFC_46A5_B01A_43471E5A2F4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlatformSet_page5.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlatformSet_page5 dialog

class CPlatformSet_page5 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPlatformSet_page5)

// Construction
public:
	void LoadCurrentSetting();
	void SetCurrentSetting();
	CPlatformSet_page5();
	~CPlatformSet_page5();

// Dialog Data
	//{{AFX_DATA(CPlatformSet_page5)
	enum { IDD = IDD_PLATFORMSET_PAGE5 };
	int		m_iGategory;
	CString	m_strPreProcessor;
	CString	m_strProjectOptions;
	int		m_nWarningLevel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPlatformSet_page5)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPlatformSet_page5)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLATFORMSET_PAGE5_H__9FFB02B6_0AFC_46A5_B01A_43471E5A2F4A__INCLUDED_)
