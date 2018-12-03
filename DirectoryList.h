#if !defined(AFX_DIRECTORYLIST_H__795D01CC_1522_4935_976B_6C14290309B2__INCLUDED_)
#define AFX_DIRECTORYLIST_H__795D01CC_1522_4935_976B_6C14290309B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DirectoryList.h : header file
//


#define FILE_LIST		0
#define DIRECTROY_LIST	1
/////////////////////////////////////////////////////////////////////////////
// CDirectoryList window

class CDirectoryList : public CWnd
{
// Construction
public:
	CDirectoryList();

// Attributes
public:
	CListBox * m_pListBox;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDirectoryList)
	public:
	protected:
//	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetSpecialFilterString(CString strSpecialFilter);
	void SetSpecialString(CString strSpecial);
	void SetContentType(int iType);
	CString m_strTitle;
	CString m_strSpecial;
	CString m_strSpecialFilter;
	int m_iContent; //The Content is File or Directroy
	void SetTitle(CString strTitle);
	virtual ~CDirectoryList();

	// Generated message map functions
protected:
	BOOL Browse(CString &csPath);
	BOOL DoPromptFileName(CString& fileName);

	CBitmapButton  * m_pInsert;
	CBitmapButton  * m_pDelete;
	CBitmapButton  * m_pUpLine;
	CBitmapButton  * m_pDownLine;
	//{{AFX_MSG(CDirectoryList)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct);
	afx_msg void OnInsert();
	afx_msg void OnDelete();
	afx_msg void OnUpLine();
	afx_msg void OnDownLine();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIRECTORYLIST_H__795D01CC_1522_4935_976B_6C14290309B2__INCLUDED_)
