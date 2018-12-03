#if !defined(AFX_PLATFORMSET_PAGE4_H__78EB31B0_F215_42AC_89B7_7B3062F5D8BC__INCLUDED_)
#define AFX_PLATFORMSET_PAGE4_H__78EB31B0_F215_42AC_89B7_7B3062F5D8BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlatformSet_page4.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlatformSet_page4 dialog

class CPlatformSet_page4 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPlatformSet_page4)

// Construction
public:
	void SetCurrentSetting();
	CPlatformSet_page4();
	~CPlatformSet_page4();

// Dialog Data
	//{{AFX_DATA(CPlatformSet_page4)
	enum { IDD = IDD_PLATFORMSET_PAGE4 };
	CString	m_strPreProcessor;
	CString	m_ProjectOptions;
	int		m_nWarningLevel;
	int		m_nOptimizations;
	int		m_nCategory;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPlatformSet_page4)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPlatformSet_page4)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void LoadCurrentSetting();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLATFORMSET_PAGE4_H__78EB31B0_F215_42AC_89B7_7B3062F5D8BC__INCLUDED_)
