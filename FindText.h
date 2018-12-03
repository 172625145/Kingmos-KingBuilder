#if !defined(AFX_FINDTEXT_H__0486878A_FE68_4320_9656_ADD427C5F8EF__INCLUDED_)
#define AFX_FINDTEXT_H__0486878A_FE68_4320_9656_ADD427C5F8EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindText.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFindText dialog

#define FIND_FINDNEXT	1
#define FIND_MARKALL	2

class CFindText : public CDialog
{
// Construction
public:
	void CheckEdit();
	int nFindStyle;
	CFindText(CWnd* pParent = NULL);   // standard constructor

//	CComboBox	*m_pSearchComboBox;
// Dialog Data
	//{{AFX_DATA(CFindText)
	enum { IDD = IDD_FINDTEXTDLG };
	CButton	m_FindNext;
	CComboBox	m_ComboBox;
	CButton	m_MarkAll;
	CString	m_strText;
	BOOL	m_bMatchAllWordOnly;
	BOOL	m_bMatchCase;
	int		m_iDir;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindText)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFindText)
	afx_msg void OnFindnext();
	afx_msg void OnMarkall();
	afx_msg void OnEditchangeCombo1();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDTEXT_H__0486878A_FE68_4320_9656_ADD427C5F8EF__INCLUDED_)


