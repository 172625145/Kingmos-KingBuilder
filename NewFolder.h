#if !defined(AFX_NEWFOLDER_H__D587478D_A736_4BAC_B1F8_77B92247DA80__INCLUDED_)
#define AFX_NEWFOLDER_H__D587478D_A736_4BAC_B1F8_77B92247DA80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewFolder.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewFolder dialog

class CNewFolder : public CDialog
{
// Construction
public:
	CNewFolder(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewFolder)
	enum { IDD = IDD_NEWFOLDER };
	CEdit	m_ctrlNewFolderText;
	CString	m_strNewFolderText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewFolder)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewFolder)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWFOLDER_H__D587478D_A736_4BAC_B1F8_77B92247DA80__INCLUDED_)
