#if !defined(AFX_AMBOOUTBAR_H__91AFAC3F_B3CE_455C_9595_FD2E525E1DBE__INCLUDED_)
#define AFX_AMBOOUTBAR_H__91AFAC3F_B3CE_455C_9595_FD2E525E1DBE__INCLUDED_

#include "mlOutTab.h"	// Added by ClassView
//#include "mlOutCtrl.h" // Added by ClassView
//nclude "../mlClass/mlCygwin.h"	// Added by ClassView

#include "../mlClass/mlBuild.h"	// Added by ClassView
//#include "mlBuild.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AmboOutBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAmboOutBar window

class CAmboOutBar : public CmlControlBar
{
// Construction
public:
	CAmboOutBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAmboOutBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	CView * GetActiveView();
	void ActiveFindInFilesMessage();
	void ClearFindInFilesMessage();
	void OutputFindmessage(CString strMessage);
	void SetProjectPath(CString &strProjectPath);
	void SetWorkspacePath(CString &strWorkspacePath);
	BOOL Build();
	BOOL Compile(CString &strPath);

	BOOL ReBuild();
	BOOL MakeImage();
	BOOL Clean();
	BOOL Console();
	BOOL CompileDirectory(CString &strPath);
	BOOL CompileModule(CString &strPath);

	BOOL IsRun() { return m_BuildClass.IsRun(); };
	BOOL Stop() { return m_BuildClass.Stop(); };
//	void Execute(LPCTSTR lpszCmd);
	HANDLE m_hReadHandle;
	HANDLE m_hWriteHandle;
//	CmlCygwin m_Cygwin;
	CmlBuild m_BuildClass;
//	BOOL m_bReadPipeStop;
	HANDLE m_hWritePipe;
	HANDLE m_hReadPipe;
	int GetTabCurSel(){return m_OutBar.GetCurSel();};
	COutputMessageEdit& GetTabEdit(int iItem){return m_OutBar.GetTabEdit(iItem);};
	static ULONG CALLBACK BuildThreadProc( LPVOID lpParameter );
	virtual ~CAmboOutBar();

	// Generated message map functions
protected:
	//CmlOutCtrl m_OutCtrl;
	CmlOutTab m_OutBar;
	int m_BuildIndex;
//	int m_DebugIndex;
	int m_FindInFilesIndex;

	HANDLE m_Thread;
	BOOL m_bExit;

//	CString m_csCygdrive;
	//{{AFX_MSG(CAmboOutBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(int nChar, int nRepCnt, int nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	void ActiveBuildMessage();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AMBOOUTBAR_H__91AFAC3F_B3CE_455C_9595_FD2E525E1DBE__INCLUDED_)
