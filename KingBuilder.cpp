// Ambo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

#include <atlbase.h>

#include "Ambo.h"

#include "MainFrame.h"
#include "AmboFrame.h"
#include "AmboDoc.h"
#include "AmboView.h"
#include "OutputView.h"
#include "CreateSelectPage.h"
#include "InputPage.h"
#include "NewPlatform_Setp1.h"
#include "NewPlatform_Setp2.h"
#include "NewPlatform_Setp3.h"

#include "PlatformSet_Page1.h"
#include "PlatformSet_Page2.h"
#include "PlatformSet_Page3.h"
#include "PlatformSet_Page4.h"
#include "PlatformSet_Page5.h"
#include "PlatformSet_Page6.h"

#include "ActiveSoftWare.h"

#include "SysOption.h"
#include "SysSetting.h"
#include <process.h>

#include "NewBSPPage.h"
#include "CPUAndBSPDirPage.h"

#include "BSPConfig.h"

#include "FindInFiles.h"

#include "Infomation.h"

#include "ModuleSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_OPENEXISTWORKSPACE	5001
/////////////////////////////////////////////////////////////////////////////
// CAmboApp

BEGIN_MESSAGE_MAP(CAmboApp, CWinApp)
	//{{AFX_MSG_MAP(CAmboApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_NEW, OnNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_SEARCHINPUT, OnSearchinput)
	ON_COMMAND(IDM_COMPILE, OnCompile)
	ON_UPDATE_COMMAND_UI(IDM_COMPILE, OnUpdateCompile)
	ON_COMMAND(IDM_OPTION, OnOption)
	ON_UPDATE_COMMAND_UI(IDM_OPTION, OnUpdateOption)
	ON_COMMAND(IDM_PRJSETTING, OnPRJSetting)
	ON_UPDATE_COMMAND_UI(IDM_PRJSETTING, OnUpdatePRJSetting)
	ON_COMMAND(IDM_STOPBUILD, OnStopbuild)
	ON_UPDATE_COMMAND_UI(IDM_STOPBUILD, OnUpdateStopbuild)
	ON_COMMAND(IDM_BUILD, OnBuild)
	ON_UPDATE_COMMAND_UI(IDM_BUILD, OnUpdateBuild)
	ON_COMMAND(IDM_COMPILEFEATURE, OnCompileFeature)
	ON_UPDATE_COMMAND_UI(IDM_COMPILEFEATURE, OnUpdateCompileFeature)
	ON_UPDATE_COMMAND_UI(IDM_REFRESHMODULE, OnUpdateRefreshmodule)	
	ON_COMMAND(IDM_REFRESHMODULE, OnRefreshmodule)
	ON_COMMAND(IDM_REBUILD, OnReBuild)
	ON_UPDATE_COMMAND_UI(IDM_REBUILD, OnUpdateReBuild)
	ON_COMMAND(IDM_MAKEIMAGE, OnMakeImage)
	ON_UPDATE_COMMAND_UI(IDM_MAKEIMAGE, OnUpdateMakeImage)
	ON_COMMAND(IDM_CLEAN, OnClean)
	ON_UPDATE_COMMAND_UI(IDM_CLEAN, OnUpdateClean)
	ON_COMMAND(IDM_CONSOLE, OnConsole)
	ON_UPDATE_COMMAND_UI(IDM_CONSOLE, OnUpdateConsole)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE1, OnUpdateRecentFileMenu)
	ON_COMMAND_EX_RANGE(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE16, OnOpenRecentFile)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_MRU_FILE1, OnUpdateRecentProjectMenu)
	ON_COMMAND_EX_RANGE(ID_PROJECT_MRU_FILE1, ID_PROJECT_MRU_FILE16, OnOpenRecentProject)
	ON_COMMAND(ID_BSPWIZARD, OnBspWizard)
	ON_UPDATE_COMMAND_UI(ID_BSPWIZARD, OnUpdateBspWizard)
	ON_COMMAND(ID_NEWWORKSPACE, OnNewworkspace)
	ON_UPDATE_COMMAND_UI(ID_NEWWORKSPACE, OnUpdateNewworkspace)
	ON_COMMAND(ID_OPENWORKSPACE, OnOpenworkspace)
	ON_UPDATE_COMMAND_UI(ID_OPENWORKSPACE, OnUpdateOpenworkspace)
	ON_COMMAND(ID_CLOSEWORKSPACE, OnCloseworkspace)
	ON_UPDATE_COMMAND_UI(ID_CLOSEWORKSPACE, OnUpdateCloseworkspace)
	ON_COMMAND(IDM_ACTIVESOFTWARE, OnActivesoftware)
	ON_UPDATE_COMMAND_UI(IDM_ACTIVESOFTWARE, OnUpdateActivesoftware)
	ON_UPDATE_COMMAND_UI(IDM_INSERTFILE, OnUpdateInsertfile)
	ON_COMMAND(IDM_INSERTFILE, OnInsertfile)
	ON_COMMAND(ID_FINDINFILES, OnFindinfiles)
	ON_UPDATE_COMMAND_UI(ID_FINDINFILES, OnUpdateFindinfiles)
	ON_UPDATE_COMMAND_UI(ID_HELP_USING, OnUpdateHelpUsing)
	ON_COMMAND(ID_HELP_USING, OnHelpUsing)
	ON_COMMAND(IDM_MODULESELECT, OnModuleselect)
	ON_UPDATE_COMMAND_UI(IDM_MODULESELECT, OnUpdateModuleselect)
	ON_COMMAND(ID_WINDOW_CLOSEALL, OnWindowCloseall)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_CLOSEALL, OnUpdateWindowCloseall)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateEditFind)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


#define KINGMOS_MRU_FILE_COUNT	16
#define KINGMOS_MAXDISPLEN		256
#define KINGMOS_MRU_TEXT_COUNT	32

AFX_STATIC_DATA const TCHAR FileSection[] = _T("Recent File List");
AFX_STATIC_DATA const TCHAR FileEntry[] = _T("File%d");

AFX_STATIC_DATA const TCHAR ProjectSection[] = _T("Recent Project List");
AFX_STATIC_DATA const TCHAR ProjectEntry[] = _T("Project%d");

AFX_STATIC_DATA const TCHAR FindTextSection[] = _T("Recent Search List");
AFX_STATIC_DATA const TCHAR FindTextEntry[] = _T("Find%d");

AFX_STATIC_DATA const TCHAR FindFolderSection[] = _T("Recent Search Folder List");
AFX_STATIC_DATA const TCHAR FindFolderEntry[] = _T("Folder%d");

AFX_STATIC_DATA const TCHAR ReplaceTextSection[] = _T("Recent Replace List");
AFX_STATIC_DATA const TCHAR ReplaceTextEntry[] = _T("Replace%d");

AFX_STATIC_DATA const TCHAR ShellNewValueName[] = _T("NullFile");
AFX_STATIC_DATA const TCHAR ShellNewValue[] = _T("");
AFX_STATIC_DATA const TCHAR ShellNewFmt[] = _T("%s\\ShellNew");

AFX_STATIC_DATA const TCHAR ToBeOpenProject[] = _T("To Be OpenProject");
AFX_STATIC_DATA const TCHAR ToBeOpenProjectEntry[] = _T("Project");

AFX_STATIC_DATA const TCHAR MaxStateSection[] = _T("Windows State");
AFX_STATIC_DATA const TCHAR MaxStateEntry[] = _T("Maxin");
/////////////////////////////////////////////////////////////////////////////
// CAmboApp construction

CAmboApp::CAmboApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_pMyRecentFileList = NULL;
	m_pRecentProjectList = NULL;
	m_pRecentFindTextList = NULL;
	m_pRecentFindFolderList = NULL;
	m_pRecentReplaceTextList = NULL;

	m_bMaximized = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAmboApp construction

CAmboApp::~CAmboApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	if (m_pMyRecentFileList != NULL)
		delete m_pMyRecentFileList;
	if (m_pRecentProjectList != NULL)
		delete m_pRecentProjectList;
	if (m_pRecentFindTextList != NULL)
		delete m_pRecentFindTextList;
	if (m_pRecentFindFolderList != NULL)
		delete m_pRecentFindFolderList;
	if (m_pRecentReplaceTextList != NULL)
		delete m_pRecentReplaceTextList;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAmboApp object

CAmboApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAmboApp initialization

BOOL CAmboApp::InitInstance()
{
//	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

//	ShowMessageAtDesktop(10,10,m_lpCmdLine);
//	ShowMessageAtDesktop(10,30,m_pszAppName);
//	ShowMessageAtDesktop(10,50,m_pszExeName);

	HWND hWnd = FindWindow(_T("KingBuilder"),NULL);
	if (hWnd)
	{

		CCommandLineInfo cmdInfo;
		ParseCommandLine(cmdInfo);

		// Dispatch commands specified on the command line
		if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
			cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

		if (!cmdInfo.m_strFileName.IsEmpty())
		{// 我们将打开需要打开的工程
			SetRegistryKey(_T("KingBuilder"));
			WriteToBeOpenProject(cmdInfo.m_strFileName);
			PostMessage(hWnd,ID_OPENEXISTWORKSPACE,0,0);
		}

//		ShowMessageAtDesktop(10,10,"3333333");
		SetForegroundWindow(hWnd);
//		ShowMessageAtDesktop(10,10,"4444444");
		return TRUE;
	}
//	AfxMessageBox("pppppppppp",MB_OK);
	CString strKingBuilderRoot;
	GetKingbuilderRoot(strKingBuilderRoot);
	if (strKingBuilderRoot.IsEmpty())
	{
		CString strShow;
		strShow.LoadString( IDS_NO_FIND_BUILDER_PATH );
		//AfxMessageBox("没有找到KingBuilder路径",MB_OK);
		AfxMessageBox(strShow,MB_OK);
		return FALSE;
	}

/*	CString strKingmosRoot;
	GetKingmosRoot(strKingmosRoot);
	if (strKingmosRoot.IsEmpty())
	{
		AfxMessageBox("没有找到Kingmos路径",MB_OK);
		return FALSE;
	}
*/
	SetCygWinReg();

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
//	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	SetRegistryKey(_T("KingBuilder"));

	LoadStdProfileSettings( 0 );  // Load standard INI file options (including MRU)


	m_pMyRecentFileList = new CMyRecentFileList(0, FileSection, FileEntry,
		KINGMOS_MRU_FILE_COUNT,KINGMOS_MAXDISPLEN);
	m_pMyRecentFileList->ReadList();

	m_pRecentProjectList = new CMyRecentFileList(0, ProjectSection, ProjectEntry,
		KINGMOS_MRU_FILE_COUNT,KINGMOS_MAXDISPLEN);
	m_pRecentProjectList->ReadList();


	m_pRecentFindTextList = new CRecentStringList(0, FindTextSection, FindTextEntry,
		KINGMOS_MRU_TEXT_COUNT);
	m_pRecentFindTextList->ReadList();

	m_pRecentFindFolderList = new CRecentStringList(0, FindFolderSection, FindFolderEntry,
		KINGMOS_MRU_TEXT_COUNT);
	m_pRecentFindFolderList->ReadList();

	
	m_pRecentReplaceTextList = new CRecentStringList(0, ReplaceTextSection, ReplaceTextEntry,
		KINGMOS_MRU_TEXT_COUNT);
	m_pRecentReplaceTextList->ReadList();
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	m_bMaximized = GetProfileInt(MaxStateSection,MaxStateEntry,TRUE);

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_AMBOTYPE,
		RUNTIME_CLASS(CAmboDoc),
		RUNTIME_CLASS(CAmboFrame), // custom MDI child frame
		RUNTIME_CLASS(CAmboView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
//	RegisterShellFileTypes(TRUE);
	RegisterKingmosShellFileTypes();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	if (!ProcessShellCommand(cmdInfo))
	{
		return FALSE;
	}

	// The main window has been initialized, so show and update it.
	m_nCmdShow |= SW_SHOWMAXIMIZED;
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	if ( pMainFrame->m_workBar.m_uEnable )
	{
		AfxMessageBox("程序还没有激活，请通过“帮助-->激活产品”来激活。",MB_OK);
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}	}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CAmboApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CAmboApp message handlers

CDocument* CAmboApp::OpenDocumentFile(LPCTSTR lpszFileName) 
{
	// TODO: Add your specialized code here and/or call the base class
	CDocument* pRet = NULL;
	const TCHAR *lpstrExt = _tcsrchr (lpszFileName, TEXT('.'));
	
	if (lpstrExt != NULL)
	{
		if ((_tcscmp (lpstrExt, TEXT(".osp")) == 0)
			||(_tcscmp (lpstrExt, TEXT(".osw")) == 0))
		{
			OpenWorkSpace(lpszFileName);	
			pRet = (CDocument*)1;
//			SetFocusToEdit();
			return pRet;
		}
	}
	pRet = CWinApp::OpenDocumentFile(lpszFileName);
	SetFocusToEdit();
	return pRet;
}

void CAmboApp::OnFileNew() 
{
	// TODO: Add your command handler code here
	CWinApp::OnFileNew ();
}

void CAboutDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CAmboApp::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	CWnd *pWind = CWnd::GetFocus();
	if(pWind == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	if (pMainFrame->m_outBar.IsChild(pWind))
	{				
		int iItem = pMainFrame->m_outBar.GetTabCurSel();
		COutputMessageEdit &cOutEdit = pMainFrame->m_outBar.GetTabEdit(iItem);		
		if (cOutEdit.ExistSelection())
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	else 
	{
		//CAmboView* pView = (CAmboView*)pMainFrame->GetActiveView();
		CAmboView* pView = (CAmboView*)pMainFrame->GetActiveFrame()->GetActiveView();
		if (pView && pView->IsChild(pWind))
		{
			if (pView->h_Edit->ExistSelection())
			{
				pCmdUI->Enable(TRUE);
				return;
			}
		}
	}
	pCmdUI->Enable(FALSE);
}

void CAmboApp::OnNew() 
{
	// TODO: Add your command handler code here
	CWinApp::OnFileNew ();
	return ;
/*
	CCreateSelectPage createSelect;
//	CInputPage createInput;
	CNewPlatform_Setp1  pageNewPlatform_Step1;
	CNewPlatform_Setp2  pageNewPlatform_Step2;
	CNewPlatform_Setp3  pageNewPlatform_Step3;
	CPropertySheet propertySheet("Create");
	propertySheet.AddPage(&createSelect);
//	propertySheet.AddPage(&createInput);
	propertySheet.AddPage(&pageNewPlatform_Step1);
	propertySheet.AddPage(&pageNewPlatform_Step2);
	propertySheet.AddPage(&pageNewPlatform_Step3);
	//propertySheet.SetWizardButtons(PSWIZB_NEXT|PSWIZB_FINISH);
	propertySheet.SetWizardMode();
	
//	if (propertySheet.DoModal() == IDOK)
	if (propertySheet.DoModal() == ID_WIZFINISH )
	{
		if (createSelect.iSelectRoute == ROUTE_TEXTFILE)
		{ // 创建一个新的文本文件
			CWinApp::OnFileNew ();
			return ;
		}
		else if (createSelect.iSelectRoute == ROUTE_PROJECT)
		{
			// 创建一个新的工程
			return ;
		}
		else if (createSelect.iSelectRoute == ROUTE_WORKSPACE)
		{
			// 在这里已经创建了一个新的环境，并打开该环境
//			OpenDocumentFile(createInput.m_strFileName);
//		    CreateNewWorkSpace(m_strWorkSpaceName, m_strLocation, m_CPUType);
			CString strModuleSetFile;
			strModuleSetFile = pageNewPlatform_Step1.m_strLocation + "\\ModuleSet.txt";
			pageNewPlatform_Step3.m_ModuleRead.SaveCurModuleState(strModuleSetFile);

		    CreateNewWorkSpace(pageNewPlatform_Step1.m_strPlatformName, pageNewPlatform_Step1.m_strLocation, pageNewPlatform_Step2.m_strBSPFileName);
			return ;
		}
	}
	//在这里什么也不做
*/
}

void CAmboApp::OnFileOpen() 
{
	// TODO: Add your command handler code here
	// prompt the user (with all document templates)
	CString newName;
	if (!DoPromptFileName(newName, AFX_IDS_OPENFILE,
	  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, NULL))
		return; // open cancelled

	OpenDocumentFile(newName);
		// if returns NULL, the user has already been alerted	
}

BOOL CAmboApp::DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate)
{
	CFileDialog dlgFile(bOpenFileDialog);

	CString title;
	CString strInitialDir;

	VERIFY(title.LoadString(nIDSTitle));

	dlgFile.m_ofn.Flags |= lFlags;

	CString strFilter;
/*	CString strDefault;
	if (pTemplate != NULL)
	{
		ASSERT_VALID(pTemplate);
		_AfxAppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate, &strDefault);
	}
	else
	{
		// do for all doc template
		POSITION pos = m_templateList.GetHeadPosition();
		BOOL bFirst = TRUE;
		while (pos != NULL)
		{
			CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
			_AfxAppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate,
				bFirst ? &strDefault : NULL);
			bFirst = FALSE;
		}
	}
*/

	// append the WorkSpaces files filter
	if (nIDSTitle == IDS_OPENWORKSPACE)
	{
		CString strWorkSpaceFilter;
		VERIFY(strWorkSpaceFilter.LoadString(IDS_WORKSPACESFILTER));
		strFilter += strWorkSpaceFilter;
		strFilter += (TCHAR)'\0';   // next string please
		strFilter += _T("*.osw");
		strFilter += (TCHAR)'\0';   // last string
		dlgFile.m_ofn.nMaxCustFilter++;

		// append the Project files filter
		CString strProjectFilter;
		VERIFY(strProjectFilter.LoadString(IDS_PROJECTFILTER));
		strFilter += strProjectFilter;
		strFilter += (TCHAR)'\0';   // next string please
		strFilter += _T("*.osp");
		strFilter += (TCHAR)'\0';   // last string
		dlgFile.m_ofn.nMaxCustFilter++;

		GetKingbuilderRoot(strInitialDir);
		strInitialDir+= "\\workspace";
		if (m_pRecentProjectList->GetSize() != 0)
		{
			if((*m_pRecentProjectList)[0].GetLength() != 0)
			{
				strInitialDir = (*m_pRecentProjectList)[0];
			}
		}

		dlgFile.m_ofn.lpstrInitialDir= strInitialDir;

	}

	if (nIDSTitle == AFX_IDS_OPENFILE)
	{
		// append the C,C++ files filter
		CString strCFileFilter;
		VERIFY(strCFileFilter.LoadString(IDS_CFILEFILTER));
		strFilter += strCFileFilter;
		strFilter += (TCHAR)'\0';   // next string please
		strFilter += _T("*.c;*.cpp;*.h");
		strFilter += (TCHAR)'\0';   // last string
		dlgFile.m_ofn.nMaxCustFilter++;

		// append the Text files filter
		CString strTextFilter;
		VERIFY(strTextFilter.LoadString(IDS_TEXTFILTER));
		strFilter += strTextFilter;
		strFilter += (TCHAR)'\0';   // next string please
		strFilter += _T("*.txt");
		strFilter += (TCHAR)'\0';   // last string
		dlgFile.m_ofn.nMaxCustFilter++;

		// append the "*.*" all files filter
		CString allFilter;
		VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
		strFilter += allFilter;
		strFilter += (TCHAR)'\0';   // next string please
		strFilter += _T("*.*");
		strFilter += (TCHAR)'\0';   // last string
		dlgFile.m_ofn.nMaxCustFilter++;
	}

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	int nResult = dlgFile.DoModal();
	fileName.ReleaseBuffer();
	return nResult == IDOK;
}


//BOOL CAmboApp::CreateNewWorkSpace(CString strWorkSpaceName, CString strLocation, UINT uCPUType)
BOOL CAmboApp::CreateNewWorkSpace(CString strWorkSpaceName, CString strLocation, CString m_strBSPName, CString strPlatformRoot)
{
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	BOOL bRet;

		if (pMainFrame == NULL)
			return FALSE;
//		return pMainFrame->CreateNewWorkSpace(strWorkSpaceName, strLocation, uCPUType);
		bRet = pMainFrame->CreateNewWorkSpace(strWorkSpaceName, strLocation, m_strBSPName, strPlatformRoot);
		return bRet;
}

void CAmboApp::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
		// TODO: Add your command update UI handler code here
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	CWnd *pWind = CWnd::GetFocus();
	if (pWind == NULL || pMainFrame == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
		//CAmboView* pView = (CAmboView*)pMainFrame->GetActiveView();
/*	if (pMainFrame->m_outBar.IsChild(pWind))
*/
	CAmboView* pView = (CAmboView*)pMainFrame->GetActiveFrame()->GetActiveView();
	if (pView && pView->IsChild(pWind) )
	{
		if (pView->h_Edit->ExistSelection())
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	pCmdUI->Enable(FALSE);
}

void CAmboApp::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	CWnd *pWind = CWnd::GetFocus();
	if (pWind == NULL)
		return;
	if (pMainFrame->IsChild(pWind))
	{
		//CAmboView* pView = (CAmboView*)pMainFrame->GetActiveView();
		CAmboView* pView = (CAmboView*)pMainFrame->GetActiveFrame()->GetActiveView();
		if (pView && pView->h_Edit == pWind)
		{
			pCmdUI->Enable(IsClipboardFormatAvailable(CF_TEXT));
			return;
		}
	}
	pCmdUI->Enable(FALSE);
}

void CAmboApp::OnEditCopy() 
{
	// TODO: Add your command handler code here
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	CWnd *pWind = CWnd::GetFocus();
	if(pWind == NULL)
		return;
	if (pMainFrame->m_outBar.IsChild(pWind))
	{				
		int iItem = pMainFrame->m_outBar.GetTabCurSel();
		COutputMessageEdit &cOutEdit = pMainFrame->m_outBar.GetTabEdit(iItem);		
		if (cOutEdit.ExistSelection())
			cOutEdit.Copy();
	}
	else 
	{
		//CAmboView* pView = (CAmboView*)pMainFrame->GetActiveView();
		CAmboView* pView = (CAmboView*)pMainFrame->GetActiveFrame()->GetActiveView();
		if (pView && pView->IsChild(pWind))
		{
			if (pView->h_Edit->ExistSelection())
				pView->h_Edit->Copy();
		}
	}
}

void CAmboApp::OnEditCut() 
{
	// TODO: Add your command handler code here
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	CWnd *pWind = CWnd::GetFocus();
	if(pWind == NULL)
		return;
	
	//CAmboView* pView = (CAmboView*)pMainFrame->GetActiveView();
	CAmboView* pView = (CAmboView*)pMainFrame->GetActiveFrame()->GetActiveView();
	if (pView && pView->IsChild(pWind))
	{
		if (pView->h_Edit->ExistSelection())
			pView->h_Edit->Cut();
	}
}	

void CAmboApp::OnEditPaste() 
{
	// TODO: Add your command handler code here
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	CWnd *pWind = CWnd::GetFocus();
	if(pWind == NULL)
		return;
	
	//CAmboView* pView = (CAmboView*)pMainFrame->GetActiveView();
	CAmboView* pView = (CAmboView*)pMainFrame->GetActiveFrame()->GetActiveView();
	if (pView && pView->IsChild(pWind))
	{
			pView->h_Edit->Paste();
			return ;
	}	
}

BOOL CAmboApp::SetCaretPos(CPoint ptCaret)
{
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	ASSERT(pMainFrame);
	CAmboView* pView = (CAmboView*)pMainFrame->GetActiveFrame()->GetActiveView();
	if (pView)
		pView->h_Edit->SetEditCaretPos(ptCaret);
	return TRUE;
}

void CAmboApp::OnSearchinput() 
{
	// TODO: Add your command handler code here
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	CComboBox *pSearchInputComboBox;
	CString strSearch;

	pSearchInputComboBox = pMainFrame->m_wndEditBar.GetComboBox(ID_SEARCHINPUT);

	pSearchInputComboBox->GetWindowText(strSearch);
		//CAmboView* pView = (CAmboView*)pMainFrame->GetActiveView();
	CAmboView* pView = (CAmboView*)pMainFrame->GetActiveFrame()->GetActiveView();
	if (pView)
	{
		SetSearchString(strSearch);
		pView->h_Edit->SearchInFile(strSearch);
	}
}

void CAmboApp::OnCompile()
{
//	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
//	if (pMainFrame)
//	{
//		pMainFrame->m_outBar.Compile();
//	}
	CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

	if (pMainFrame)
	{
		CAmboFrame* pChildFrame = (CAmboFrame*)pMainFrame->MDIGetActive();
		if (pChildFrame)
		{
			CAmboView* pView = (CAmboView*)pChildFrame->GetActiveView();
		
			if (pView)
			{
				CString strFileName;
				strFileName = pView->GetDocument()->GetPathName();
				if (!strFileName.IsEmpty())
					pMainFrame->m_outBar.Compile(strFileName);
				return ;
			}
		}
	}
}
void CAmboApp::OnUpdateCompile(CCmdUI* pCmdUI)
{
	CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

	if (pMainFrame)
	{
		if (pMainFrame->m_workBar.m_pProjectInfo)
		{
			if (pMainFrame->m_workBar.m_pProjectInfo->IsProjectOpen() == TRUE)
			{
				CAmboFrame* pChildFrame = (CAmboFrame*)pMainFrame->MDIGetActive();
				if (pChildFrame)
				{
					CAmboView* pView = (CAmboView*)pChildFrame->GetActiveView();
				
					if (pView)
					{
						pCmdUI->Enable(TRUE);
						return ;
					}
				}
			}
		}
	}
	pCmdUI->Enable(FALSE);
}

void CAmboApp::OnOption()
{
	CSysOption optionDlg;

		if (optionDlg.DoModal() == IDOK)
		{
			CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

			if (pMainFrame)
			{
				CAmboFrame* pChildFrame = (CAmboFrame*)pMainFrame->MDIGetActive();
				if (pChildFrame)
				{
					CAmboView* pView = (CAmboView*)pChildFrame->GetActiveView();
					if (pView)
					{
						CSpecialEdit *hEdit;
							hEdit = pView->h_Edit;
							hEdit->ResetColor(FALSE);
							hEdit->InvalidateRect(NULL);
					}	
				}
			}
		}
}
void CAmboApp::OnUpdateOption(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CAmboApp::OnPRJSetting()
{
/*
	CSysSetting sysSettingDlg;
//	int iVersion = VERSION_B;
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;

//		if (pMainFrame)
//		{
//			iVersion = pMainFrame->m_workBar.m_pWorkSpaceInfo->GetVersion();
//		}
//		sysSettingDlg.SetVersion(iVersion);
		if (sysSettingDlg.DoModal() == IDOK)
		{
//			iVersion = sysSettingDlg.GetVersion();
			if (pMainFrame)
			{
//				pMainFrame->m_workBar.m_pWorkSpaceInfo->SetVersion(iVersion);
				return ;
			}
			
/*			CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

			if (pMainFrame)
			{
				CAmboFrame* pChildFrame = (CAmboFrame*)pMainFrame->MDIGetActive();
				if (pChildFrame)
				{
					CAmboView* pView = (CAmboView*)pChildFrame->GetActiveView();
					if (pView)
					{
						CSpecialEdit *hEdit;
							hEdit = pView->h_Edit;
							hEdit->ResetColor(FALSE);
							hEdit->InvalidateRect(NULL);
					}	
				}
			}

		}
*/
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	if (pMainFrame == NULL)
		return ;
	CPlatformSet_page1  pagePlatformSet_Page1;
	CPlatformSet_page2  pagePlatformSet_Page2;
	CPlatformSet_page3  pagePlatformSet_Page3;
	CPlatformSet_page4  pagePlatformSet_Page4;
	CPlatformSet_page5  pagePlatformSet_Page5;
	CPlatformSet_page6  pagePlatformSet_Page6;
	CPropertySheet propertySheet("PlatformSet");
	propertySheet.AddPage(&pagePlatformSet_Page1);  //General
	if (pMainFrame->m_workBar.m_pWorkSpaceInfo)
	{
		if (pMainFrame->m_workBar.m_pWorkSpaceInfo->IsWorkSpaceOpen() == TRUE)		
		{
			propertySheet.AddPage(&pagePlatformSet_Page2);  //Environment
		}
	}
	propertySheet.AddPage(&pagePlatformSet_Page3); //Directory
	propertySheet.AddPage(&pagePlatformSet_Page4); //Compiler
	propertySheet.AddPage(&pagePlatformSet_Page5); //Assember

	if (pMainFrame->m_workBar.m_pProjectInfo)
	{
		if (pMainFrame->m_workBar.m_pProjectInfo->IsProjectOpen() == TRUE)
		{
			propertySheet.AddPage(&pagePlatformSet_Page6); //Link
		}
	}
	//propertySheet.SetWizardButtons(PSWIZB_NEXT|PSWIZB_FINISH);
	propertySheet.m_psh.dwFlags |= PSH_NOAPPLYNOW | PSH_PROPTITLE;
//	propertySheet.SetWizardMode();
	propertySheet.SetTitle("Platform Setting",0);
	
//	if (propertySheet.DoModal() == ID_WIZFINISH )
	if (propertySheet.DoModal() == IDOK)
	{
		if (pMainFrame->m_workBar.m_pWorkSpaceInfo)
		{
			if (pMainFrame->m_workBar.m_pWorkSpaceInfo->IsWorkSpaceOpen() == TRUE)		
			{
				pMainFrame->m_workBar.m_pWorkSpaceInfo->SetCurrentOptionToBuild();   // 将当前的各种设置赋给Build
			}
		}
		if (pMainFrame->m_workBar.m_pProjectInfo)
		{
			if (pMainFrame->m_workBar.m_pProjectInfo->IsProjectOpen() == TRUE)
			{
				pMainFrame->m_workBar.m_pProjectInfo->SetCurrentOptionToBuild();   // 将当前的各种设置赋给Build
			}
		}
	}
	//在这里什么也不做
}
void CAmboApp::OnUpdatePRJSetting(CCmdUI* pCmdUI)
{
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	if (pMainFrame)
	{
		if (pMainFrame->m_workBar.IsWorkSpaceOpen())
		{
			pCmdUI->Enable(!pMainFrame->m_outBar.IsRun());
			return ;
		}
	}
	pCmdUI->Enable(FALSE);
}



void CAmboApp::OnStopbuild()
{
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	if (pMainFrame)
	{
		pMainFrame->m_outBar.Stop();
	}
}
void CAmboApp::OnUpdateStopbuild(CCmdUI* pCmdUI)
{
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	if (pMainFrame)
	{
		pCmdUI->Enable(pMainFrame->m_outBar.IsRun());
		return ;
	}
	pCmdUI->Enable(FALSE);
}
void CAmboApp::OnBuild()
{
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	if (pMainFrame)
	{
		pMainFrame->m_outBar.Build();
	}
}
void CAmboApp::OnUpdateBuild(CCmdUI* pCmdUI)
{
	BOOL bEnable = FALSE;
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	if (pMainFrame)
	{
		if (pMainFrame->m_workBar.IsWorkSpaceOpen() == TRUE)
		{
			if (pMainFrame->m_outBar.IsRun() == FALSE)
				bEnable = TRUE;
		}
	}
	pCmdUI->Enable(bEnable);
}

void CAmboApp::OnCompileFeature()
{
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	int iResult;
	if (pMainFrame)
	{
		CString strPath;
		iResult = pMainFrame->m_workBar.GetSelFeature(strPath);
		if ( iResult == FEATURE_DIR)
		{
			pMainFrame->m_outBar.CompileDirectory(strPath);
		}
		else if ( iResult == FEATURE_MODULE)
		{
			pMainFrame->m_outBar.CompileModule(strPath);
		}
		else if ( iResult == FEATURE_FILE)
		{
			pMainFrame->m_outBar.Compile(strPath);
		}
		else if ( iResult == FEATURE_ROOT)
		{
			pMainFrame->m_outBar.Build();
		}
	}
}
void CAmboApp::OnUpdateCompileFeature(CCmdUI* pCmdUI)
{
	BOOL bEnable = FALSE;
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	if (pMainFrame)
	{
		if (pMainFrame->m_workBar.CanCompileFeature() == TRUE)
		{
			if (pMainFrame->m_outBar.IsRun() == FALSE)
				bEnable = TRUE;
		}
	}
	pCmdUI->Enable(bEnable);
}

void CAmboApp::OnRefreshmodule()
{
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	int iResult;
	ASSERT(pMainFrame);
	{
		CString strPath;
		iResult = pMainFrame->m_workBar.GetSelFeature(strPath);
		if ( iResult == FEATURE_DIR)
		{
			CString strListName;
			strListName = strPath + "\\LISTS";
			pMainFrame->m_workBar.RefreshModules(strListName);
		}
		else if ( iResult == FEATURE_MODULE)
		{
			CString strModulesName;
			strModulesName = strPath + "\\MODULES";
			pMainFrame->m_workBar.RefreshModules(strModulesName);
		}
		else if ( iResult == FEATURE_ROOT)
		{
			CString strListName;
			strListName = strPath + "\\LISTS";
			pMainFrame->m_workBar.RefreshModules(strListName);
		}
	}
}
void CAmboApp::OnUpdateRefreshmodule(CCmdUI* pCmdUI)
{
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	int iResult;
	BOOL bEnable = FALSE;
	if (pMainFrame)
	{
		CString strPath;
		iResult = pMainFrame->m_workBar.GetSelFeature(strPath);
		if ( iResult == FEATURE_DIR)
		{
			bEnable = TRUE;
		}
		else if ( iResult == FEATURE_MODULE)
		{
			bEnable = TRUE;
		}
		else if ( iResult == FEATURE_FILE)
		{
			bEnable = FALSE;
		}
		else if ( iResult == FEATURE_ROOT)
		{
			bEnable = TRUE;
		}
	}
	pCmdUI->Enable(bEnable);
}

void CAmboApp::OnReBuild()
{
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	if (pMainFrame)
	{
		pMainFrame->m_outBar.ReBuild();
	}
}
void CAmboApp::OnUpdateReBuild(CCmdUI* pCmdUI)
{
	BOOL bEnable = FALSE;
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	if (pMainFrame)
	{
		if (pMainFrame->m_workBar.IsWorkSpaceOpen() == TRUE)
		{
			if (pMainFrame->m_outBar.IsRun() == FALSE)
				bEnable = TRUE;
		}
	}
	pCmdUI->Enable(bEnable);
}
void CAmboApp::OnMakeImage()
{
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	if (pMainFrame)
	{
		pMainFrame->m_outBar.MakeImage();
	}
}
void CAmboApp::OnUpdateMakeImage(CCmdUI* pCmdUI)
{
	BOOL bEnable = FALSE;
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	if (pMainFrame)
	{
		if (pMainFrame->m_workBar.IsWorkSpaceOpen() == TRUE)
		{
			if (pMainFrame->m_outBar.IsRun() == FALSE)
				bEnable = TRUE;
		}
	}
	pCmdUI->Enable(bEnable);
}
void CAmboApp::OnClean()
{
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	if (pMainFrame)
	{
		pMainFrame->m_outBar.Clean();
	}
}
void CAmboApp::OnUpdateClean(CCmdUI* pCmdUI)
{
	BOOL bEnable = FALSE;
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	if (pMainFrame)
	{
		if (pMainFrame->m_workBar.IsWorkSpaceOpen() == TRUE)
		{
			if (pMainFrame->m_outBar.IsRun() == FALSE)
				bEnable = TRUE;
		}
	}
	pCmdUI->Enable(bEnable);
}
void CAmboApp::OnConsole()
{
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	if (pMainFrame)
	{
		pMainFrame->m_outBar.Console();
	}
}
void CAmboApp::OnUpdateConsole(CCmdUI* pCmdUI)
{
	BOOL bEnable = FALSE;
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	if (pMainFrame)
	{
		if (pMainFrame->m_workBar.IsWorkSpaceOpen() == TRUE)
		{
			bEnable = TRUE;
		}
	}
	pCmdUI->Enable(bEnable);
}

void CAmboApp::OnUpdateRecentFileMenu(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	if (m_pMyRecentFileList == NULL) // no MRU files
		pCmdUI->Enable(FALSE);
	else
		m_pMyRecentFileList->UpdateMenu(pCmdUI);
}
void CAmboApp::OnUpdateRecentProjectMenu(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);

	if (m_pRecentProjectList == NULL) // no MRU files
		pCmdUI->Enable(FALSE);
	else
	{
		m_pRecentProjectList->UpdateMenu(pCmdUI);
	}
}

BOOL CAmboApp::OnOpenRecentFile(UINT nID)
{
	ASSERT_VALID(this);
	ASSERT(m_pMyRecentFileList != NULL);

	ASSERT(nID >= ID_FILE_MRU_FILE1);
	ASSERT(nID < ID_FILE_MRU_FILE1 + (UINT)m_pMyRecentFileList->GetSize());
	int nIndex = nID - ID_FILE_MRU_FILE1;
	ASSERT((*m_pMyRecentFileList)[nIndex].GetLength() != 0);

	TRACE2("MRU: open file (%d) '%s'.\n", (nIndex) + 1,
			(LPCTSTR)(*m_pMyRecentFileList)[nIndex]);

	if (OpenDocumentFile((*m_pMyRecentFileList)[nIndex]) == NULL)
		m_pMyRecentFileList->Remove(nIndex);

	return TRUE;
}

BOOL CAmboApp::OnOpenRecentProject(UINT nID)
{

	ASSERT_VALID(this);
	ASSERT(m_pRecentProjectList != NULL);

	ASSERT(nID >= ID_PROJECT_MRU_FILE1);
	ASSERT(nID < ID_PROJECT_MRU_FILE1 + (UINT)m_pRecentProjectList->GetSize());
	int nIndex = nID - ID_PROJECT_MRU_FILE1;
	ASSERT((*m_pRecentProjectList)[nIndex].GetLength() != 0);

	TRACE2("MRU: open file (%d) '%s'.\n", (nIndex) + 1,
			(LPCTSTR)(*m_pRecentProjectList)[nIndex]);

//	OpenDocumentFile((*m_pRecentProjectList)[nIndex]);
//		m_pRecentProjectList->Remove(nIndex);
//	if (OpenDocumentFile((*m_pRecentProjectList)[nIndex]) == NULL)
//		m_pRecentProjectList->Remove(nIndex);
	if (OpenWorkSpace((*m_pRecentProjectList)[nIndex]) == NULL)
		m_pRecentProjectList->Remove(nIndex);
//	SetFocusToEdit();
	return TRUE;
}

int CAmboApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	// if the WorkSpace is Open , then Save the WorkSpace

	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	if (pMainFrame)
	{
		if (pMainFrame->m_workBar.IsWorkSpaceOpen() == TRUE)
		{
			pMainFrame->m_workBar.CloseWorkSpace();
		}
	}
	if (m_pMyRecentFileList)
		m_pMyRecentFileList->WriteList();
	if (m_pRecentProjectList)
		m_pRecentProjectList->WriteList();
	if (m_pRecentFindTextList)
		m_pRecentFindTextList->WriteList();
	if (m_pRecentFindFolderList)
		m_pRecentFindFolderList->WriteList();
	if (m_pRecentReplaceTextList)
		m_pRecentReplaceTextList->WriteList();
	
	WriteProfileInt(MaxStateSection,MaxStateEntry,m_bMaximized);

	return CWinApp::ExitInstance();
}

//void CAmboApp::OnFinalRelease() 
//{
	// TODO: Add your specialized code here and/or call the base class
	
//	CWinApp::OnFinalRelease();
//}

//void CAmboApp::OnAppExit() 
//{
	// TODO: Add your command handler code here
//	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
//	if (pMainFrame)
//	{
//		if (pMainFrame->m_outBar.IsRun())
//		{
//			if (AfxMessageBox("程序正在运行，确认退出",MB_OKCANCEL) == IDOK)
//			{
//				pMainFrame->m_outBar.Stop();
//			}
//			else
//				return ;
//		}
//	}
//	
//}

void CAmboApp::OnAppExit() 
{
	// TODO: Add your command handler code here
	CWinApp::OnAppExit();	
}

void CAmboApp::AddToRecentFileList(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	ASSERT_VALID(this);
	ASSERT(lpszPathName != NULL);
	ASSERT(AfxIsValidString(lpszPathName));

	if (m_pMyRecentFileList != NULL)
		m_pMyRecentFileList->Add(lpszPathName);
}

void CAmboApp::AddToRecentProjectList(LPCTSTR lpszPathName)
{
	// TODO: Add your specialized code here and/or call the base class
	ASSERT_VALID(this);
	ASSERT(lpszPathName != NULL);
	ASSERT(AfxIsValidString(lpszPathName));

	if (m_pRecentProjectList != NULL)
		m_pRecentProjectList->Add(lpszPathName);
}

void CAmboApp::OnBspWizard() 
{
	// TODO: Add your command handler code here
	CNewBSPPage dNewBSPPage;
	CCPUAndBSPDirPage dCPUAndBSPDirPage;
	CPropertySheet BSPWizardSheet("CreateBSP");
	BSPWizardSheet.AddPage(&dNewBSPPage);
	BSPWizardSheet.AddPage(&dCPUAndBSPDirPage);
	//propertySheet.SetWizardButtons(PSWIZB_NEXT|PSWIZB_FINISH);
	BSPWizardSheet.SetWizardMode();
	
	if (BSPWizardSheet.DoModal() == ID_WIZFINISH )
	{
		CBSPConfig BSPConfig;
		CString strVersion;

		BSPConfig.SetBSPName(dNewBSPPage.m_strNewBSPName);
		BSPConfig.SetVendor(dNewBSPPage.m_strVendor);

		strVersion = dNewBSPPage.m_strVersion0;
		strVersion += ".";
		strVersion += dNewBSPPage.m_strVersion1;
		strVersion += ".";
		strVersion += dNewBSPPage.m_strVersion2;
		strVersion += ".";
		strVersion += dNewBSPPage.m_strVersion3;
		BSPConfig.SetVersion(strVersion);
		BSPConfig.SetDescription(dNewBSPPage.m_strDescription);
		BSPConfig.SetPlatformName(dCPUAndBSPDirPage.m_strBSPPlatformName);
		BSPConfig.SetCPUType(dCPUAndBSPDirPage.m_strCPUType);

		BSPConfig.SaveBSP(""); // 如果不输入文件名，则将默认文件名存入系统目录下

	}
	
}

void CAmboApp::OnUpdateBspWizard(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}

void CAmboApp::GetKingbuilderRoot(CString &strRoot)
{
	CRegKey reg;
	
	LONG lKey = reg.Open(HKEY_LOCAL_MACHINE, TEXT("Software\\Kingmos\\kingbuilder\\1.00.000"));
	if (lKey == ERROR_SUCCESS)
	{
		DWORD dwCount = MAX_PATH;
//		TCHAR lpKingBuilderRoot[MAX_PATH];
		reg.QueryValue(strRoot.GetBuffer(MAX_PATH),TEXT("KINGBUILDER_ROOT"),&dwCount);
		reg.Close();
		strRoot.ReleaseBuffer();
		int nCount = strRoot.ReverseFind('\\');
		strRoot = strRoot.Left(nCount);
		return;
	}
	lKey = reg.Open(HKEY_CURRENT_USER, TEXT("Software\\Kingmos\\kingbuilder\\1.00.000"));
	if (lKey == ERROR_SUCCESS)
	{
		DWORD dwCount = MAX_PATH;
//		TCHAR lpKingBuilderRoot[MAX_PATH];
		reg.QueryValue(strRoot.GetBuffer(MAX_PATH),TEXT("KINGBUILDER_ROOT"),&dwCount);
		reg.Close();
		strRoot.ReleaseBuffer();
		int nCount = strRoot.ReverseFind('\\');
		strRoot = strRoot.Left(nCount);
		return;
	}
	strRoot = "";
}

void CAmboApp::GetKingmosRoot(CString &strRoot)
{
/*
	CRegKey reg;
	
	LONG lKey = reg.Open(HKEY_LOCAL_MACHINE, TEXT("Software\\Kingmos\\Kingmos"));
	if (lKey == ERROR_SUCCESS)
	{
		DWORD dwCount = MAX_PATH;
//		TCHAR lpKingBuilderRoot[MAX_PATH];
		reg.QueryValue(strRoot.GetBuffer(MAX_PATH),TEXT("KINGMOS_ROOT"),&dwCount);
		reg.Close();
		strRoot.ReleaseBuffer();
		strRoot.TrimRight(' ');
		return;
	}

	lKey = reg.Open(HKEY_CURRENT_USER, TEXT("Software\\Kingmos\\Kingmos"));
	if (lKey == ERROR_SUCCESS)
	{
		DWORD dwCount = MAX_PATH;
//		TCHAR lpKingBuilderRoot[MAX_PATH];
		reg.QueryValue(strRoot.GetBuffer(MAX_PATH),TEXT("KINGMOS_ROOT"),&dwCount);
		reg.Close();
		strRoot.ReleaseBuffer();
		strRoot.TrimRight(' ');
		return;
	}
	strRoot = "";
	*/
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;

		if (pMainFrame)
		{
			if (pMainFrame->m_workBar.m_pWorkSpaceInfo)
			{
				if (pMainFrame->m_workBar.m_pWorkSpaceInfo->IsWorkSpaceOpen())
				{
					pMainFrame->m_workBar.m_pWorkSpaceInfo->GetKingmosRoot(strRoot);
				}
			}
		}
}

int CAmboApp::GetModuleValue(CString strModuleName)
{
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;

		if (pMainFrame)
		{
			return pMainFrame->m_workBar.m_pWorkSpaceInfo->GetModuleValue(strModuleName);
		}
		return 0;
}

void CAmboApp::OnNewworkspace() 
{
	// TODO: Add your command handler code here
	CCreateSelectPage createSelect;
	CInputPage createInput;
	CNewPlatform_Setp1  pageNewPlatform_Step1;
	CNewPlatform_Setp2  pageNewPlatform_Step2;
#ifdef NEED_STEP3   //lilin add code, 2005-01-05
	CNewPlatform_Setp3  pageNewPlatform_Step3;
#endif
	CPropertySheet propertySheet("Create");
	propertySheet.AddPage(&createSelect);
	propertySheet.AddPage(&createInput);
	propertySheet.AddPage(&pageNewPlatform_Step1);
	propertySheet.AddPage(&pageNewPlatform_Step2);
#ifdef NEED_STEP3   //lilin add code, 2005-01-05
	propertySheet.AddPage(&pageNewPlatform_Step3);
#endif
	//propertySheet.SetWizardButtons(PSWIZB_NEXT|PSWIZB_FINISH);
	propertySheet.SetWizardMode();
	

	BOOL bRet = FALSE;
//	if (propertySheet.DoModal() == IDOK)
	if (propertySheet.DoModal() == ID_WIZFINISH )
	{
/*		if (createSelect.iSelectRoute == ROUTE_TEXTFILE)
		{ // 创建一个新的文本文件
			CWinApp::OnFileNew ();
			return ;
		}
*/
		if (createSelect.iSelectRoute == ROUTE_PROJECT)
		{
			// 创建一个新的工程
//		    CreateNewProject(m_strWorkSpaceName, m_strLocation, m_CPUType);
			CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
			if (pMainFrame)
			{
				if (createInput.m_bLoadFromSource)
				{
					bRet = pMainFrame->m_workBar.CreateNewProject(createInput.m_strSourceLocation, createInput.m_strCPUType);
				}
				else
				{
					bRet = pMainFrame->m_workBar.CreateNewProject(createInput.m_strWorkSpaceName, createInput.m_strProjectType,createInput.m_strLocation, createInput.m_strCPUType);
				}
				if (bRet)
				{
					if (pMainFrame->m_workBar.m_pProjectInfo)
						AddToRecentProjectList(pMainFrame->m_workBar.m_pProjectInfo->m_strFileName);
				}
			}
			return ;
		}
		else if (createSelect.iSelectRoute == ROUTE_WORKSPACE)
		{
			// 在这里已经创建了一个新的环境，并打开该环境
//			OpenDocumentFile(createInput.m_strFileName);
//		    CreateNewWorkSpace(m_strWorkSpaceName, m_strLocation, m_CPUType);
			CString strModuleSetFile;
			strModuleSetFile = pageNewPlatform_Step1.m_strLocation + "\\script\\";
			strModuleSetFile += MODULESETFILE;
#ifdef NEED_STEP3   //lilin add code, 2005-01-05
			pageNewPlatform_Step3.m_ModuleRead.SaveCurModuleState(strModuleSetFile);
#endif 

		    bRet = CreateNewWorkSpace(pageNewPlatform_Step1.m_strPlatformName, pageNewPlatform_Step1.m_strLocation, pageNewPlatform_Step2.m_strBSPFileName,pageNewPlatform_Step1.m_strPlatformRoot);
			CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
			if (pMainFrame)
			{
				if (bRet)
				{
					if (pMainFrame->m_workBar.m_pWorkSpaceInfo)
						AddToRecentProjectList(pMainFrame->m_workBar.m_pWorkSpaceInfo->m_strFileName);
				}
			}
		}
	}
	//在这里什么也不做
	
}

void CAmboApp::OnUpdateNewworkspace(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//	pCmdUI->Enable(TRUE);		
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	if (pMainFrame)
	{
		pCmdUI->Enable(!pMainFrame->m_workBar.m_uEnable);
	}
}

void CAmboApp::OnOpenworkspace() 
{
	// TODO: Add your command handler code here
	CString newName;
	if (!DoPromptFileName(newName, IDS_OPENWORKSPACE,
	  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, NULL))
		return; // open cancelled

//	OpenDocumentFile(newName);
	OpenWorkSpace(newName);	
//	WriteToBeOpenProject(newName);
//	SetFocusToEdit();

}

void CAmboApp::OnUpdateOpenworkspace(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//	pCmdUI->Enable(TRUE);		
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	if (pMainFrame)
	{
		pCmdUI->Enable(!pMainFrame->m_workBar.m_uEnable);
	}
}

void CAmboApp::OnCloseworkspace() 
{
	// TODO: Add your command handler code here
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	if (pMainFrame)
	{
		pMainFrame->m_workBar.CloseWorkSpace();
	}
}

void CAmboApp::OnUpdateCloseworkspace(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BOOL bEnable = FALSE;
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	if (pMainFrame)
	{
		if (pMainFrame->m_workBar.IsWorkSpaceOpen() == TRUE)
		{
			bEnable = TRUE;
		}
	}
	pCmdUI->Enable(bEnable);
}

void CAmboApp::GetVersion(CString &strVersion)
{
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;

	strVersion = "";
	if (pMainFrame->m_workBar.m_pWorkSpaceInfo)
	{
		if (pMainFrame->m_workBar.m_pWorkSpaceInfo->IsWorkSpaceOpen())
		{
			pMainFrame->m_workBar.m_pWorkSpaceInfo->GetVersion(strVersion);
		}
	}
}

void CAmboApp::GetActiveFileName(CString &strFileNameDef)
{
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
		if (pMainFrame)
		{
			CAmboView* pView = (CAmboView*)pMainFrame->GetActiveFrame()->GetActiveView();
			if (pView != NULL)
			{
				CDocument * pDoc = pView->GetDocument();

				if (pDoc)
					strFileNameDef = pDoc->GetPathName();
			}
		}	
}

void CAmboApp::SetWorkspacePath(CString &strWorkspacePath)
{
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	if (pMainFrame)
	{
		pMainFrame->m_outBar.SetWorkspacePath(strWorkspacePath);
	}
}

void CAmboApp::SetProjectPath(CString &strProjectPath)
{
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	if (pMainFrame)
	{
		pMainFrame->m_outBar.SetProjectPath(strProjectPath);
	}
}

BOOL CAmboApp::OpenWorkSpace(LPCTSTR lpszFileName)
{
	const TCHAR *lpstrExt = _tcsrchr (lpszFileName, TEXT('.'));
	
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	ASSERT(pMainFrame);

	if (pMainFrame->m_outBar.IsRun())
	{
		CString strShow;
		strShow.LoadString( IDS_STOP_BUILDING );
		AfxMessageBox( strShow, MB_OK );
		return FALSE;
	}
	if (lpstrExt != NULL)
	{
		if ((_tcscmp (lpstrExt, TEXT(".osp")) == 0)
			||(_tcscmp (lpstrExt, TEXT(".osw")) == 0))
		{
//			CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
			if (pMainFrame->OpenProject (lpszFileName)== FALSE)
			{
				return FALSE;
			}
			AddToRecentProjectList(lpszFileName);
			SetFocusToEdit();
			return TRUE;
		}
	}
	return FALSE;
}

void CAmboApp::OnActivesoftware() 
{
	// TODO: Add your command handler code here
#ifdef NEED_ACTIVE   // 2006-01-05, by lilin
	CActiveSoftWare dlgActiveSoftWare;	
	dlgActiveSoftWare.DoModal();
#endif
}

void CAmboApp::OnUpdateActivesoftware(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}

void CAmboApp::OnUpdateInsertfile(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	// TODO: Add your command update UI handler code here
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	if (pMainFrame)
	{
		if (pMainFrame->m_workBar.m_pProjectInfo== NULL)
		{
			pCmdUI->Enable(FALSE);
			return ;
		}

		if (pMainFrame->m_workBar.m_pProjectInfo->IsProjectOpen() == TRUE)
		{
			pCmdUI->Enable(TRUE);
			return ;
		}
	}
	pCmdUI->Enable(FALSE);
}

void CAmboApp::OnInsertfile() 
{
	// TODO: Add your command handler code here
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	if (pMainFrame)
	{
		if (pMainFrame->m_workBar.m_pProjectInfo== NULL)
		{
			return ;
		}

		pMainFrame->m_workBar.OnInsertfile();
	}
}

void CAmboApp::OnFindinfiles() 
{
	// TODO: Add your command handler code here
	CFindInFiles dlgFindInFiles;

		if (m_grep.IsComplete() == FALSE)
		{
			//正在进行查找，停止查找
			m_grep.Break();
			return ;
		}

		if (dlgFindInFiles.DoModal() == IDOK)
		{
			((CAmboApp *)AfxGetApp())->SetSearchString(dlgFindInFiles.m_strFindText);

			m_grep.SetFilter(dlgFindInFiles.m_strFindFilter);
			m_grep.SetSearchFolder(dlgFindInFiles.m_FindFolder);
			m_grep.SetSearchString(dlgFindInFiles.m_strFindText);
			m_grep.SetLookInSubFolder(dlgFindInFiles.m_bLookInSubFolder);
			m_grep.SetCaseMatch(dlgFindInFiles.m_bCaseMatch);
			m_grep.SetMatchWholeWordOnly(dlgFindInFiles.m_bMatchWholeWord);
			m_grep.Go();
		}	
}

void CAmboApp::OnUpdateFindinfiles(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}

void CAmboApp::InitialFindTextComboBox(CComboBox *pComboBox)
{
	CString strName;
	int iMRU,nSize;
	
	nSize = m_pRecentFindTextList->GetSize();
	for (iMRU = 0; iMRU < nSize; iMRU++)
	{
		if (!m_pRecentFindTextList->GetAt(strName, iMRU))
			break;
		pComboBox->AddString(strName);
	}
	pComboBox->SetCurSel(0);
}

void CAmboApp::SetSearchString(CString strSearch)
{
	CComboBox *pSearchInputComboBox;
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;

	if (strSearch.IsEmpty())
		return ;

	pSearchInputComboBox = pMainFrame->m_wndEditBar.GetComboBox(ID_SEARCHINPUT);

//	pSearchInputComboBox->GetWindowText(strSearch);
	//如果有查询到指定的字符串，则设定为当前串，否则插入该字符串
//	if (pSearchInputComboBox->SelectString(0, strSearch) == CB_ERR )
	if (SelectStringInComboBox(pSearchInputComboBox,strSearch) == FALSE)
	{ // 没有找到
//		int iIndex = pSearchInputComboBox->AddString(strSearch);
		int iIndex = pSearchInputComboBox->InsertString(0,strSearch);
		pSearchInputComboBox->SetCurSel(iIndex);
	}
	m_pRecentFindTextList->Add(strSearch);
}

void CAmboApp::InitialFindFolderComboBox(CComboBox *pComboBox)
{
	CString strName;
	int iMRU,nSize;
	
	nSize = m_pRecentFindFolderList->GetSize();
	for (iMRU = 0; iMRU < nSize; iMRU++)
	{
		if (!m_pRecentFindFolderList->GetAt(strName, iMRU))
			break;
		pComboBox->AddString(strName);
	}
	if (pComboBox->GetCount() == 0)
	{
		CString strLocation;
		GetProjectLocation(strLocation);
		if (strLocation.IsEmpty())
		{
			GetKingbuilderRoot(strLocation);
		}
		pComboBox->AddString(strLocation);
		m_pRecentFindFolderList->Add(strLocation);
	}
	pComboBox->SetCurSel(0);
}

void CAmboApp::GetProjectLocation(CString &strLocation)
{
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	if (pMainFrame)
	{
		if (pMainFrame->m_workBar.m_pWorkSpaceInfo)
		{
			if (pMainFrame->m_workBar.m_pWorkSpaceInfo->IsWorkSpaceOpen())
			{
				strLocation = pMainFrame->m_workBar.m_pWorkSpaceInfo->m_strWorkSpaceRoot;
			}
		}
		if (pMainFrame->m_workBar.m_pProjectInfo)
		{
			if (pMainFrame->m_workBar.m_pProjectInfo->IsProjectOpen())
			{
				strLocation = pMainFrame->m_workBar.m_pProjectInfo->m_strProjectRoot;
			}
		}
	}
}

void CAmboApp::InitialReplaceTextComboBox(CComboBox *pComboBox)
{
	CString strName;
	int iMRU,nSize;
	
	nSize = m_pRecentReplaceTextList->GetSize();
	for (iMRU = 0; iMRU < nSize; iMRU++)
	{
		if (!m_pRecentReplaceTextList->GetAt(strName, iMRU))
			break;
		pComboBox->AddString(strName);
	}
	pComboBox->SetCurSel(0);
}

void CAmboApp::SetReplaceText(CString strRepaceText)
{
	if (strRepaceText.IsEmpty())
		return ;
	m_pRecentReplaceTextList->Add(strRepaceText);
}


BOOL CAmboApp::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWinApp::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CAmboApp::OnUpdateHelpUsing(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}

void CAmboApp::OnHelpUsing() 
{
	// TODO: Add your command handler code here
	// 运行帮助
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startInfo;
//	CreateProcess("IExplorer.exe	","..\\help\\main.htm",NULL,NULL,);
/*	m_bRun = ::CreateProcess (NULL, (LPTSTR)csShell.GetBuffer(csShell.GetLength()),	
		NULL, NULL, TRUE,
		CREATE_NEW_PROCESS_GROUP | DETACHED_PROCESS | CREATE_SUSPENDED,
		NULL, NULL, &si,&pi);//
*/
	memset(&startInfo,0,sizeof(STARTUPINFO));

	
	CString strLocation;
	CString KeyValue;
	CString Command;
	GetKingbuilderRoot(strLocation);
	strLocation += "\\Help\\Kingmos help.chm";
//	LPTSTR lpCommand = strLocation.LockBuffer();
//	Command = "IEXPLORE.EXE ";
	// Get IExplorer Application
	CRegKey reg;
	LONG lKey = reg.Open(HKEY_CLASSES_ROOT, TEXT("chm.file\\shell\\open\\command"));
	if (lKey == ERROR_SUCCESS)
	{
		DWORD dwCount = MAX_PATH;
//		TCHAR lpKingBuilderRoot[MAX_PATH];
		reg.QueryValue(KeyValue.GetBuffer(MAX_PATH),TEXT(""),&dwCount);
		reg.Close();
		KeyValue.ReleaseBuffer();
		int iStart,iNext;
		iStart = KeyValue.Find('"',0);
		if ( iStart == -1)
		{
			Command = KeyValue;
		}
		else
		{
			iStart ++;
			iNext = KeyValue.Find('"',iStart );
			Command = KeyValue.Mid(iStart,iNext - iStart);
		}
	}
	Command += " ";
	Command += strLocation;
	startInfo.cb = sizeof(STARTUPINFO);
	if (::CreateProcess(NULL/*"C:\\Program Files\\Internet Explorer\\IEXPLORE.EXE"*/,(LPTSTR)(LPCTSTR)Command,NULL,NULL,TRUE,CREATE_NEW_PROCESS_GROUP,NULL,NULL,&startInfo,&processInfo) == 0)
	{
		LPVOID lpMsgBuf;
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL 
		);
		// Process any inserts in lpMsgBuf.
		// ...
		// Display the string.
		MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
		// Free the buffer.
		LocalFree( lpMsgBuf );
	}
//	strLocation.ReleaseBuffer();
}

void CAmboApp::SetCygWinReg()
{
	CRegKey reg;
	
	LONG lKey = reg.Create(HKEY_CURRENT_USER, TEXT("Software\\Cygnus Solutions\\Cygwin\\mounts v2"));
	if (lKey == ERROR_SUCCESS)
	{
		reg.SetValue("/cygdrive","cygdrive prefix");
		reg.SetValue(0x00000020,"cygdrive flags");
		reg.Close();
	}
	lKey = reg.Create(HKEY_CURRENT_USER, TEXT("Software\\Cygnus Solutions\\Cygwin\\mounts v2\\/"));
	if (lKey == ERROR_SUCCESS)
	{
		CString strCygwinRoot;
		GetKingbuilderRoot(strCygwinRoot);
		strCygwinRoot += "\\cygwin";
		reg.SetValue(strCygwinRoot,"native");
		reg.SetValue((DWORD)0x00000000,"flags");
		reg.Close();
	}

	lKey = reg.Create(HKEY_LOCAL_MACHINE, TEXT("Software\\Cygnus Solutions\\Cygwin\\mounts v2"));
	if (lKey == ERROR_SUCCESS)
	{
		reg.SetValue("/cygdrive","cygdrive prefix");
		reg.SetValue(0x00000020,"cygdrive flags");
		reg.Close();
	}
	lKey = reg.Create(HKEY_LOCAL_MACHINE, TEXT("Software\\Cygnus Solutions\\Cygwin\\mounts v2\\/"));
	if (lKey == ERROR_SUCCESS)
	{
		CString strCygwinRoot;
		GetKingbuilderRoot(strCygwinRoot);
		strCygwinRoot += "\\cygwin";
		reg.SetValue(strCygwinRoot,"native");
		reg.SetValue((DWORD)0x00000000,"flags");
		reg.Close();
	}
}

BOOL CAmboApp::ProcessShellCommand(CCommandLineInfo& rCmdInfo)
{
	BOOL bResult = TRUE;

	switch (rCmdInfo.m_nShellCommand)
	{
	case CCommandLineInfo::FileOpen:
//		if (!OpenDocumentFile(rCmdInfo.m_strFileName))
//			bResult = FALSE;
		return OpenShellFileName(rCmdInfo.m_strFileName);
	}
	return CWinApp::ProcessShellCommand(rCmdInfo);
}

BOOL CAmboApp::OpenShellFileName(CString lpszFileName)
{

	if (lpszFileName.IsEmpty())
		return TRUE;

	const TCHAR *lpstrExt = _tcsrchr (lpszFileName, TEXT('.'));
	
	if (lpstrExt != NULL)
	{
		if ((_tcscmp (lpstrExt, TEXT(".osp")) == 0)
			||(_tcscmp (lpstrExt, TEXT(".osw")) == 0))
		{
			CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
			if (pMainFrame->OpenProject (lpszFileName)== FALSE)
			{
				return FALSE;
			}
			AddToRecentProjectList(lpszFileName);
			return TRUE;
		}
	}
	if (!OpenDocumentFile(lpszFileName))
		return FALSE;
	return TRUE;
}
void CAmboApp::RegisterKingmosShellFileTypes()
{
	CRegKey reg;

	CString strPathName, strTemp;

//	AfxGetModuleFileName(AfxGetInstanceHandle(), strPathName);

	// HKEY_CLASSES_ROOT\.osw
	// 默认 KingBuilder.prj

	LONG lKey = reg.Create(HKEY_CLASSES_ROOT, TEXT(".osw"));
	if (lKey == ERROR_SUCCESS)
	{
		reg.SetValue("KingBuilder.prj","");
		reg.Close();
	}
	lKey = reg.Create(HKEY_CLASSES_ROOT, TEXT(".osp"));
	if (lKey == ERROR_SUCCESS)
	{
		reg.SetValue("KingBuilder.prj","");
		reg.Close();
	}
	// HKEY_CLASSES_ROOT\.osp
	// 默认 KingBuilder.prj

	// HKEY_CLASSES_ROOT\KingBuilder.prj\DefaultIcon
	// 默认	C:\kingbuilder\bin\kingbuilder.exe,1
	lKey = reg.Create(HKEY_CLASSES_ROOT, TEXT("KingBuilder.prj\\"));
	if (lKey == ERROR_SUCCESS)
	{
		reg.SetValue("C:\\kingbuilder\\bin\\kingbuilder.exe,1","");
		reg.Close();
	}
	// HKEY_CLASSES_ROOT\KingBuilder.prj\shell\open\ddeexec
	// 默认 "[open (\"%1\")]"
	// HKEY_CLASSES_ROOT\KingBuilder.prj\shell\open\command
	// 默认 "C:\kingbuilder\bin\kingbuilder.exe" /dde
//	lKey = reg.Create(HKEY_CLASSES_ROOT, TEXT("KingBuilder.prj\\shell\\open\\ddeexec"));
//	if (lKey == ERROR_SUCCESS)
//	{
//		reg.SetValue("[open (\"%1\")]","");
//		reg.Close();
//	}
	lKey = reg.Create(HKEY_CLASSES_ROOT, TEXT("KingBuilder.prj\\shell\\open\\command"));
	if (lKey == ERROR_SUCCESS)
	{
		reg.SetValue("\"C:\\kingbuilder\\bin\\kingbuilder.exe\" \"%1\"","");
		reg.Close();
	}

}


void CAmboApp::ShowMessageAtDesktop(int x,int y,CString strMsg)
{
//	CWnd *pDesktop;
//	HWND hDesktop;
	HDC  hdc;


//		hDesktop = ::GetDesktopWindow();

		hdc = ::GetDC( NULL);
		::TextOut(hdc,x,y,(LPCTSTR)strMsg,strMsg.GetLength());
		ReleaseDC(NULL,hdc);
}

//DEL BOOL CAmboApp::RegisterWindowClass()
//DEL {
//DEL 
//DEL }

BOOL CAmboApp::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == ID_OPENEXISTWORKSPACE)
	{
		// Will Open a Exist WorkSpace
		CString newName;

//		newName.Format("%d",pMsg->lParam);
		ReadToBeOpenProject(newName);
		OpenWorkSpace(newName);	
		return TRUE;
	}
	
	return CWinApp::PreTranslateMessage(pMsg);
}

void CAmboApp::WriteToBeOpenProject(CString strFilename)
{
	WriteProfileString((LPCTSTR)ToBeOpenProject, ToBeOpenProjectEntry,
				(LPCTSTR)strFilename);

}

void CAmboApp::ReadToBeOpenProject(CString &strFilename)
{
		strFilename = GetProfileString(
			ToBeOpenProject, ToBeOpenProjectEntry, &afxChNil);
}

void CAmboApp::GetSystemEnvironmentValue(CString strEnvVar, CString &strValue)
{
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	ASSERT(pMainFrame);

	if (pMainFrame->m_workBar.m_pWorkSpaceInfo)
	{
		if (pMainFrame->m_workBar.m_pWorkSpaceInfo->IsWorkSpaceOpen())
		{
			pMainFrame->m_workBar.m_pWorkSpaceInfo->GetSystemEnvironmentValue(strEnvVar, strValue);
		}
	}
}

void CAmboApp::OnModuleselect() 
{
	// TODO: Add your command handler code here
#ifdef NEED_MODULE_SELECT   // add code by lilin, 2005-01-05
	CModuleSelect	dlgModuleSelect;

	dlgModuleSelect.DoModal();
#endif
}

void CAmboApp::OnUpdateModuleselect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	ASSERT (pMainFrame);
	if (pMainFrame->m_workBar.m_pWorkSpaceInfo)
	{
		if (pMainFrame->m_workBar.m_pWorkSpaceInfo->IsWorkSpaceOpen() == TRUE)		
		{
			pCmdUI->Enable(!pMainFrame->m_outBar.IsRun());
			return ;
		}
	}
	pCmdUI->Enable(FALSE);
}

BOOL CAmboApp::GetWorkSpaceRoot(CString &strModuleSetFile)
{
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;

	ASSERT(pMainFrame);

	if (pMainFrame->m_workBar.m_pWorkSpaceInfo)
	{
		if (pMainFrame->m_workBar.m_pWorkSpaceInfo->IsWorkSpaceOpen() == TRUE)		
		{
			strModuleSetFile = pMainFrame->m_workBar.m_pWorkSpaceInfo->m_strWorkSpaceRoot;
			return TRUE;
		}
	}
	return FALSE;
}

void CAmboApp::RefreshModuleSet()
{
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;

	ASSERT(pMainFrame);

	if (pMainFrame->m_workBar.m_pWorkSpaceInfo)
	{
		if (pMainFrame->m_workBar.m_pWorkSpaceInfo->IsWorkSpaceOpen() == TRUE)		
		{
			pMainFrame->m_workBar.m_pWorkSpaceInfo->RefreshModuleSet();

			// Refresh MatterView
			CString strListName;
			GetKingmosRoot(strListName);
			strListName += "\\LISTS";
			pMainFrame->m_workBar.RefreshModules(strListName);
		}
	}
}

BOOL CAmboApp::GetWindowMax()
{
	return m_bMaximized;
}

void CAmboApp::SetWindowMax(BOOL bMaximized)
{
	m_bMaximized = bMaximized;
}

void CAmboApp::OnWindowCloseall() 
{
	// TODO: Add your command handler code here
   POSITION pos;
   
    // 得到文档模板
    pos = GetFirstDocTemplatePosition();
	if (pos == NULL)
		return;
	CDocTemplate * pDocTemplate = GetNextDocTemplate(pos);

	if (pDocTemplate == NULL)
		return;
	pDocTemplate->CloseAllDocuments(TRUE);
}

void CAmboApp::OnUpdateWindowCloseall(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}


void CAmboApp::SetFocusToEdit()
{
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	ASSERT(pMainFrame);

	CAmboView* pView = (CAmboView*)pMainFrame->GetActiveFrame()->GetActiveView();
	if (pView)
	{
		pView->h_Edit->SetFocus();
	}
}

void CAmboApp::OnEditFind() 
{
	// TODO: Add your command handler code here
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	CWnd *pWind = CWnd::GetFocus();
	if(pWind == NULL)
	{
		return;
	}
	if (pMainFrame->m_outBar.IsChild(pWind))
	{				
		CMessageView* pView = (CMessageView*)pMainFrame->m_outBar.GetActiveView();
		if (pView && pView->IsChild(pWind))
		{
			pView->OnEditFind();
			return;
		}
		return;
	}
	else 
	{
		CSpecialView* pView = (CSpecialView*)pMainFrame->GetActiveFrame()->GetActiveView();
		if (pView && pView->IsChild(pWind))
		{
			pView->OnEditFind();
			return;
		}
		return;
	}
}

void CAmboApp::OnUpdateEditFind(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMainFrame* pMainFrame = (CMainFrame *)m_pMainWnd;
	CWnd *pWind = CWnd::GetFocus();
	if(pWind == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	if (pMainFrame->m_outBar.IsChild(pWind))
	{				
		pCmdUI->Enable(TRUE);
		return;
	}
	else 
	{
		CSpecialView* pView = (CSpecialView*)pMainFrame->GetActiveFrame()->GetActiveView();
		if (pView && pView->IsChild(pWind))
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	pCmdUI->Enable(FALSE);	
}

BOOL CAmboApp::SelectStringInComboBox(CComboBox *pSearchInputComboBox,CString strSearch)
{
	int nIndex,nCount;
	CString rString;

		nCount = pSearchInputComboBox->GetCount();
		for (nIndex = 0; nIndex < nCount ;nIndex ++)
		{
			pSearchInputComboBox->GetLBText( nIndex, rString ) ;
			if (rString == strSearch)
			{
				pSearchInputComboBox->SetCurSel(nIndex);
				return TRUE;
			}
		}
		return FALSE;
}
void CAmboApp::CheckAllDoc()
{
	POSITION posTemp = GetFirstDocTemplatePosition();
	CDocTemplate * pDocTemp;

	while( posTemp )
	{
		pDocTemp = GetNextDocTemplate( posTemp );
		if( pDocTemp )
		{
			POSITION posDoc = pDocTemp->GetFirstDocPosition();
			while( posDoc )
			{
				CAmboDoc * pDoc = (CAmboDoc*)pDocTemp->GetNextDoc(posDoc);
				if( pDoc )
				{
					pDoc->CheckFileChanged();
				}
			}
		}
	}
}

