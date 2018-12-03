#if !defined(AFX_SETENV_H__F569CCF3_758C_496D_9A36_209D059DDCB4__INCLUDED_)
#define AFX_SETENV_H__F569CCF3_758C_496D_9A36_209D059DDCB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetEnv.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetEnv dialog

class CSetEnv : public CDialog
{
// Construction
public:
	CSetEnv(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetEnv)
	enum { IDD = IDD_SETENV };
	CString	m_strVariable;
	CString	m_strValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetEnv)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetEnv)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETENV_H__F569CCF3_758C_496D_9A36_209D059DDCB4__INCLUDED_)
