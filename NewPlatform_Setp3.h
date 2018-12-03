#if !defined(AFX_NEWPLATFORM_SETP3_H__1165E650_7966_4C20_A496_D18805FACF6B__INCLUDED_)
#define AFX_NEWPLATFORM_SETP3_H__1165E650_7966_4C20_A496_D18805FACF6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewPlatform_Setp3.h : header file
//
#include "ModuleRead.h"

/////////////////////////////////////////////////////////////////////////////
// CNewPlatform_Setp3 dialog

class CNewPlatform_Setp3 : public CPropertyPage
{
	DECLARE_DYNCREATE(CNewPlatform_Setp3)

// Construction
public:
	CNewPlatform_Setp3();
	~CNewPlatform_Setp3();

// Dialog Data
	//{{AFX_DATA(CNewPlatform_Setp3)
	enum { IDD = IDD_NEWPLATFORM_STEP3 };
	CTreeCtrl	m_ctrlFeature;
	//}}AFX_DATA

	CImageList * m_pImageList;
	CModuleRead m_ModuleRead;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CNewPlatform_Setp3)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CNewPlatform_Setp3)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickFeature(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedFeature(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void InsertFeature(LPFEATURESTRUCT lpCurFeature,HTREEITEM hParent = TVI_ROOT);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPLATFORM_SETP3_H__1165E650_7966_4C20_A496_D18805FACF6B__INCLUDED_)
