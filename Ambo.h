// Ambo.h : main header file for the AMBO application
//

#if !defined(AFX_AMBO_H__BDC6D7AB_D032_41AE_887E_8372DA79A542__INCLUDED_)
#define AFX_AMBO_H__BDC6D7AB_D032_41AE_887E_8372DA79A542__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "myFilelist.h"
#include "RecentStringList.h"
#include "grep.h"

#include "resource.h"       // main symbols

#define MODULESETFILE  "ModuleSet.env"
/////////////////////////////////////////////////////////////////////////////
// CAmboApp:
// See Ambo.cpp for the implementation of this class
//

class CAmboApp : public CWinApp
{
public:
	void CheckAllDoc( void );
	BOOL SelectStringInComboBox(CComboBox *pSearchInputComboBox,CString strSearch);
	void SetWindowMax(BOOL bMaximized);
	BOOL GetWindowMax();
	void RefreshModuleSet();
	BOOL GetWorkSpaceRoot(CString &strModuleSetFile);
	void GetSystemEnvironmentValue(CString strEnvVar,CString &strValue);
	void ShowMessageAtDesktop(int x,int y,CString strMsg);
	void RegisterKingmosShellFileTypes();
	BOOL ProcessShellCommand(CCommandLineInfo& rCmdInfo);
	BOOL OpenShellFileName(CString lpszFileName);
	void SetReplaceText(CString strRepaceText);
	void InitialReplaceTextComboBox(CComboBox *pComboBox);
	void GetProjectLocation(CString &strLocation);
	void InitialFindFolderComboBox(CComboBox *pComboBox);
	void SetSearchString(CString strSearch);
	void InitialFindTextComboBox(CComboBox *pComboBox);
	BOOL OpenWorkSpace(LPCTSTR lpszFileName);
	void SetProjectPath(CString &strProjectPath);
	void SetWorkspacePath(CString &strWorkspacePath);
	void GetActiveFileName(CString &strFileNameDef);
	void GetVersion(CString &strVersion);
	int GetModuleValue(CString strModuleName);
	void GetKingmosRoot(CString &strRoot);
	void GetKingbuilderRoot(CString &strRoot);
	void AddToRecentProjectList(LPCTSTR lpszPathName);
	BOOL SetCaretPos(CPoint ptCaret);
//	BOOL CreateNewWorkSpace(CString strWorkSpaceName,CString strLocation,UINT uCPUType);
	BOOL CreateNewWorkSpace(CString strWorkSpaceName,CString strLocation,CString m_strBSPName, CString strPlatformRoot);
	BOOL DoPromptFileName(CString& fileName,UINT nIDSTitle,DWORD lFlags,BOOL bOpenFileDialog,CDocTemplate * pTemplate);
	CAmboApp();
	~CAmboApp();

	CGrep m_grep;	
	CMyRecentFileList* m_pMyRecentFileList;
	CMyRecentFileList* m_pRecentProjectList;
	CRecentStringList* m_pRecentFindTextList;
	CRecentStringList* m_pRecentFindFolderList;
	CRecentStringList* m_pRecentReplaceTextList;
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAmboApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	virtual void AddToRecentFileList(LPCTSTR lpszPathName);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CAmboApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnNew();
	afx_msg void OnFileOpen();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnSearchinput();
	afx_msg void OnCompile();
	afx_msg void OnUpdateCompile(CCmdUI* pCmdUI);
	afx_msg void OnOption();
	afx_msg void OnUpdateOption(CCmdUI* pCmdUI);
	afx_msg void OnPRJSetting();
	afx_msg void OnUpdatePRJSetting(CCmdUI* pCmdUI);
	afx_msg void OnStopbuild();
	afx_msg void OnUpdateStopbuild(CCmdUI* pCmdUI);
	afx_msg void OnBuild();
	afx_msg void OnUpdateBuild(CCmdUI* pCmdUI);
	afx_msg void OnCompileFeature();
	afx_msg void OnUpdateCompileFeature(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRefreshmodule(CCmdUI* pCmdUI);
	afx_msg void OnRefreshmodule();
	afx_msg void OnReBuild();
	afx_msg void OnUpdateReBuild(CCmdUI* pCmdUI);
	afx_msg void OnMakeImage();
	afx_msg void OnUpdateMakeImage(CCmdUI* pCmdUI);
	afx_msg void OnClean();
	afx_msg void OnUpdateClean(CCmdUI* pCmdUI);
	afx_msg void OnConsole();
	afx_msg void OnUpdateConsole(CCmdUI* pCmdUI);
	afx_msg void OnAppExit();
	afx_msg void OnUpdateRecentFileMenu(CCmdUI* pCmdUI);
	afx_msg BOOL OnOpenRecentFile(UINT nID);
	afx_msg void OnUpdateRecentProjectMenu(CCmdUI* pCmdUI);
	afx_msg BOOL OnOpenRecentProject(UINT nID);
	afx_msg void OnBspWizard();
	afx_msg void OnUpdateBspWizard(CCmdUI* pCmdUI);
	afx_msg void OnNewworkspace();
	afx_msg void OnUpdateNewworkspace(CCmdUI* pCmdUI);
	afx_msg void OnOpenworkspace();
	afx_msg void OnUpdateOpenworkspace(CCmdUI* pCmdUI);
	afx_msg void OnCloseworkspace();
	afx_msg void OnUpdateCloseworkspace(CCmdUI* pCmdUI);
	afx_msg void OnActivesoftware();
	afx_msg void OnUpdateActivesoftware(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInsertfile(CCmdUI* pCmdUI);
	afx_msg void OnInsertfile();
	afx_msg void OnFindinfiles();
	afx_msg void OnUpdateFindinfiles(CCmdUI* pCmdUI);
	afx_msg void OnUpdateHelpUsing(CCmdUI* pCmdUI);
	afx_msg void OnHelpUsing();
	afx_msg void OnModuleselect();
	afx_msg void OnUpdateModuleselect(CCmdUI* pCmdUI);
	afx_msg void OnWindowCloseall();
	afx_msg void OnUpdateWindowCloseall(CCmdUI* pCmdUI);
	afx_msg void OnEditFind();
	afx_msg void OnUpdateEditFind(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void SetFocusToEdit();
	BOOL m_bMaximized;
	void ReadToBeOpenProject(CString &strFilename);
	void WriteToBeOpenProject(CString strFilename);
	void SetCygWinReg();
};

#define KEYNUM  20
#define OBJECTNUM  (((0x5A + 0x41) /2 ) * KEYNUM )

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AMBO_H__BDC6D7AB_D032_41AE_887E_8372DA79A542__INCLUDED_)
