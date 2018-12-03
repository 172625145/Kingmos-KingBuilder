#if !defined(AFX_CREATESELECTPAGE_H__7CA9D87E_832C_425A_A007_3E096F778EDE__INCLUDED_)
#define AFX_CREATESELECTPAGE_H__7CA9D87E_832C_425A_A007_3E096F778EDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CreateSelectPage.h : header file
//

#define ROUTE_NONE		0
#define ROUTE_WORKSPACE 1
#define ROUTE_PROJECT	2
#define ROUTE_TEXTFILE  3

/////////////////////////////////////////////////////////////////////////////
// CCreateSelectPage dialog

class CCreateSelectPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCreateSelectPage)

// Construction
public:
	CCreateSelectPage();
	~CCreateSelectPage();

	int iSelectRoute;
// Dialog Data
	//{{AFX_DATA(CCreateSelectPage)
	enum { IDD = IDD_CREATESELECT };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCreateSelectPage)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCreateSelectPage)
	afx_msg void OnWorkspace();
	afx_msg void OnProject();
	afx_msg void OnTextfile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREATESELECTPAGE_H__7CA9D87E_832C_425A_A007_3E096F778EDE__INCLUDED_)
