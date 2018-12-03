#if !defined(AFX_MODULESELECT_H__F739E982_ED7F_4DA1_8478_66E2C2F28E8B__INCLUDED_)
#define AFX_MODULESELECT_H__F739E982_ED7F_4DA1_8478_66E2C2F28E8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModuleSelect.h : header file
//
#include "ModuleRead.h"

/////////////////////////////////////////////////////////////////////////////
// CModuleSelect dialog

class CModuleSelect : public CDialog
{
// Construction
public:
	void InsertFeature(LPFEATURESTRUCT lpCurFeature,HTREEITEM hParent = TVI_ROOT);
	CModuleSelect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModuleSelect)
	enum { IDD = IDD_MODULESELECT };
	CTreeCtrl	m_ctrlFeature;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModuleSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList * m_pImageList;
	CModuleRead m_ModuleRead;

	// Generated message map functions
	//{{AFX_MSG(CModuleSelect)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnClickFeature(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODULESELECT_H__F739E982_ED7F_4DA1_8478_66E2C2F28E8B__INCLUDED_)
