#if !defined(AFX_MLWORKBAR_H__69AF0D9A_E302_4A3B_A8A5_2D6E8163E728__INCLUDED_)
#define AFX_MLWORKBAR_H__69AF0D9A_E302_4A3B_A8A5_2D6E8163E728__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// mlWorkBar.h : header file
//
#include "WorkSpaceInfo.h"
#include "ProjectInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CAmboWorkBar window

class CAmboWorkBar : public CmlControlBar
{
// Construction
public:
	CAmboWorkBar();

// Attributes
public:

// Operations
public:
	CWorkSpaceInfo *m_pWorkSpaceInfo;
	CProjectInfo *m_pProjectInfo;

	int m_uEnable;

private:


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAmboWorkBar)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnInsertfile();
	void ReadSeriaoNO();
	BOOL CreateNewProject(CString strProjectName, CString strProjectType,CString strLocation, CString strCPUType);
	BOOL CreateNewProject(CString strModulesName, CString strCPUType);
	void CloseWorkSpace();
	void RefreshModules(LPCTSTR lpModulesName);
	BOOL IsWorkSpaceOpen();
	BOOL CreateNewWorkSpace(CString strWorkSpaceName, CString strLocation, CString m_strBSPName,CString strPlatformRoot);
	BOOL OpenDocument (LPCTSTR lpstrDoc);
	BOOL CanCompileFeature();
	int GetSelFeature(CString &strPath);
	virtual ~CAmboWorkBar();

	// Generated message map functions
protected:
	CmlViewTab m_ViewTab;
	//{{AFX_MSG(CAmboWorkBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MLWORKBAR_H__69AF0D9A_E302_4A3B_A8A5_2D6E8163E728__INCLUDED_)
