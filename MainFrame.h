// MainFrame.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRAME_H__925A2A13_F5FE_4EB7_9624_8C9CBDBBD996__INCLUDED_)
#define AFX_MAINFRAME_H__925A2A13_F5FE_4EB7_9624_8C9CBDBBD996__INCLUDED_

#include "AmboWorkBar.h"	// Added by ClassView
#include "AmboOutBar.h"	// Added by ClassView
#include "CommandBar.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	CAmboOutBar m_outBar;
//	BOOL CreateNewWorkSpace(CString strWorkSpaceName, CString strLocation, UINT uCPUType);
	BOOL CreateNewWorkSpace(CString strWorkSpaceName, CString strLocation, CString m_strBSPName, CString strPlatformRoot);
	BOOL OpenProject (LPCTSTR lpstrDoc);
	void AppFrameClean();
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // control bar embedded members
	BOOL RegisterWindowClass();
	CStatusBar  m_wndStatusBar;
//	CToolBar    m_wndToolBar;
//	CToolBar    m_wndTestToolBar;
	CCommandBar    m_wndToolBar;
	CCommandBar    m_wndEditBar;
	CCommandBar    m_wndBuildBar;
	CAmboWorkBar   m_workBar;

// Generated message map functions
protected:
	
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnViewOutput();
	afx_msg void OnViewStatusBar();
	afx_msg void OnViewWorkspace();
	afx_msg void OnViewEditbar();
	afx_msg void OnViewToolbar();
	afx_msg void OnUpdateViewEditbar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewOutput(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewStatusBar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewToolbar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewWorkspace(CCmdUI* pCmdUI);
	afx_msg void OnViewBuildbar();
	afx_msg void OnUpdateViewBuildbar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWindowNext(CCmdUI* pCmdUI);
	afx_msg void OnWindowNext();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRAME_H__925A2A13_F5FE_4EB7_9624_8C9CBDBBD996__INCLUDED_)
