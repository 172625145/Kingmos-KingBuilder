#if !defined(AFX_PLATFORMSET_PAGE1_H__83B74EB8_6321_4C8C_818A_1A3F74516F2B__INCLUDED_)
#define AFX_PLATFORMSET_PAGE1_H__83B74EB8_6321_4C8C_818A_1A3F74516F2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlatformSet_page1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlatformSet_page1 dialog

class CPlatformSet_page1 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPlatformSet_page1)

// Construction
public:
	CPlatformSet_page1();
	~CPlatformSet_page1();

// Dialog Data
	//{{AFX_DATA(CPlatformSet_page1)
	enum { IDD = IDD_PLATFORMSET_PAGE1 };
	CComboBox	m_ctrlBuildType;
	CString	m_strBuildType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPlatformSet_page1)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPlatformSet_page1)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLATFORMSET_PAGE1_H__83B74EB8_6321_4C8C_818A_1A3F74516F2B__INCLUDED_)
