#if !defined(AFX_FINDINFILES_H__9E745B29_68F9_4F23_B3BD_25671E5FB144__INCLUDED_)
#define AFX_FINDINFILES_H__9E745B29_68F9_4F23_B3BD_25671E5FB144__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindInFiles.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFindInFiles dialog

class CFindInFiles : public CDialog
{
// Construction
public:
	BOOL Browse(CString &csPath);
	CFindInFiles(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFindInFiles)
	enum { IDD = IDD_FINDINFILES };
	CButton	m_ctrlFind;
	CComboBox	m_ctrlFindFolder;
	CComboBox	m_ctrlFindFilter;
	CComboBox	m_ctrlFindText;
	CString	m_strFindFilter;
	CString	m_FindFolder;
	CString	m_strFindText;
	BOOL	m_bLookInSubFolder;
	BOOL	m_bCaseMatch;
	BOOL	m_bMatchWholeWord;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindInFiles)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFindInFiles)
	virtual BOOL OnInitDialog();
	afx_msg void OnFind();
	afx_msg void OnEditchangeFindfilter();
	afx_msg void OnEditchangeFindfolder();
	afx_msg void OnEditchangeFindtextcombo();
	afx_msg void OnBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDINFILES_H__9E745B29_68F9_4F23_B3BD_25671E5FB144__INCLUDED_)
