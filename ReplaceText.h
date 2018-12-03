#if !defined(AFX_REPLACETEXT_H__48996069_2B99_4A59_A6BA_AC00D25694E8__INCLUDED_)
#define AFX_REPLACETEXT_H__48996069_2B99_4A59_A6BA_AC00D25694E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReplaceText.h : header file
//
#include "SpecialEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CReplaceText dialog

class CReplaceText : public CDialog
{
// Construction
public:
	void InsertReplaceTextCombo(CString strReplaceText);
	CSpecialEdit * m_pEdit;
	CReplaceText(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CReplaceText)
	enum { IDD = IDD_REPLACEDLG };
	CComboBox	m_ctrlReplaceText;
	CComboBox	m_ctrlFindText;
	CButton	m_ctrlReplaceAll;
	CButton	m_ctrlReplace;
	CButton	m_ctrlFindNext;
	CString	m_strSearch;
	BOOL	m_bMatchAllWordOnly;
	BOOL	m_bMatchCase;
	CString	m_strReplaceText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReplaceText)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	BOOL m_bCanReplace;
	// Generated message map functions
	//{{AFX_MSG(CReplaceText)
	afx_msg void OnFindNext();
	afx_msg void OnEditchangeFindtextcombo();
	afx_msg void OnReplace();
	afx_msg void OnReplaceall();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPLACETEXT_H__48996069_2B99_4A59_A6BA_AC00D25694E8__INCLUDED_)
