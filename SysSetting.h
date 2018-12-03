#if !defined(AFX_SYSSETTING_H__DFC73AA6_9C41_499B_AC4A_3AFE82804556__INCLUDED_)
#define AFX_SYSSETTING_H__DFC73AA6_9C41_499B_AC4A_3AFE82804556__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSysSetting dialog

class CSysSetting : public CDialog
{
// Construction
public:
	int GetVersion();
	void SetVersion(int iVersion);
	CSysSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSysSetting)
	enum { IDD = IDD_SYSTEMSETTING };
	CComboBox	m_VersionSel;
	//}}AFX_DATA
private:

	int m_iVersion;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSysSetting)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSSETTING_H__DFC73AA6_9C41_499B_AC4A_3AFE82804556__INCLUDED_)
