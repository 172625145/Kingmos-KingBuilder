#if !defined(AFX_ACTIVESOFTWARE_H__0F797F03_9151_41FE_A44F_0F6E9398D603__INCLUDED_)
#define AFX_ACTIVESOFTWARE_H__0F797F03_9151_41FE_A44F_0F6E9398D603__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActiveSoftWare.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CActiveSoftWare dialog

class CActiveSoftWare : public CDialog
{
// Construction
public:
	CActiveSoftWare(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CActiveSoftWare)
	enum { IDD = IDD_AVTIVESOFTWARE };
	CEdit	m_ctrlSreialNO;
	CString	m_strSerialNO;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActiveSoftWare)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CActiveSoftWare)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIVESOFTWARE_H__0F797F03_9151_41FE_A44F_0F6E9398D603__INCLUDED_)
