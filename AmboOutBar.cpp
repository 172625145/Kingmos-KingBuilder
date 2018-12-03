// AmboOutBar.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "AmboOutBar.h"
#include "OutputView.h"
#include "MainFrame.h"
#include "AmboDoc.h"
#include "AmboFrame.h"
#include "AmboView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAmboOutBar
DWORD WINAPI BuildThreadProc(LPVOID lpParameter);


CAmboOutBar::CAmboOutBar()
{
//	m_csCygdrive = TEXT("/cygdrive");
//	SetWorkspacePath(CString &strWorkspacePath);	//设置workspace路径
//	SetProjectPath(CString &strProjectPath);		//设置project路径
	CString strBuilderPath,strKingmsoPath;
	((CAmboApp *)AfxGetApp())->GetKingbuilderRoot(strBuilderPath);
	((CAmboApp *)AfxGetApp())->GetKingmosRoot(strKingmsoPath);
	m_BuildClass.SetKingmosPath(strKingmsoPath);		//设置kingmos路径

	//MyString * lpsz = (MyString *)&strBuilderPath;

	//_CrtIsValidHeapPointer( lpsz->GetPtr() ); 

	m_BuildClass.SetBuilderPath(strBuilderPath);		//设置builder路径


}

CAmboOutBar::~CAmboOutBar()
{
	m_bExit = TRUE;
	if (m_Thread)
	{
	}
}


BEGIN_MESSAGE_MAP(CAmboOutBar, CmlControlBar)
	//{{AFX_MSG_MAP(CAmboOutBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAmboOutBar message handlers

int CAmboOutBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CmlControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
/*	if (! m_OutCtrl.Create (NULL, NULL, WS_VISIBLE|WS_CHILD,
		CRect(0, 0, 0, 0), this, 32))
	{
		TRACE0("Failed to create Tab view \n");
		return -1;
	}
*/
	if (! m_OutBar.Create (WS_VISIBLE|WS_CHILD| WS_TABSTOP|TCS_SINGLELINE|TCS_BOTTOM |TCS_FOCUSNEVER ,
		CRect(0, 0, 0, 0), this, 101))
	{
		return -1;
	}
	
	// set "normal" GUI-font
	CFont *font = CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT));
//	m_OutCtrl.SetFont(font);
	m_OutBar.SetFont(font);

//	m_BuildIndex = m_OutBar.AddView (TEXT ("Build"), RUNTIME_CLASS(CmlBuildView));
//	m_DebugIndex = m_OutBar.AddView (TEXT ("Debug"), RUNTIME_CLASS(CmlBuildView));
	m_BuildIndex = m_OutBar.AddView (TEXT ("Build"), RUNTIME_CLASS(COutputView),0);
	m_FindInFilesIndex = m_OutBar.AddView (TEXT ("Find in Files"), RUNTIME_CLASS(COutputView),1);
	m_OutBar.SetWindowsFileInfo(m_FindInFilesIndex,TRUE);
//	m_DebugIndex = m_OutBar.AddView (TEXT ("Debug"), RUNTIME_CLASS(COutputView),1);
//	m_OutBar.SetActiveView (0);

/*	m_OutBar.AddString(m_BuildIndex,"Hello Build");
	m_OutBar.AddString(m_DebugIndex,"Hello Debug");
	m_OutBar.Empty (m_BuildIndex);
	m_OutBar.Empty (m_DebugIndex);
	m_OutBar.AddString(m_BuildIndex,"Hello Build1");
	m_OutBar.AddString(m_DebugIndex,"Hello Debug1");
*/
	m_hReadPipe = CreateNamedPipe(
	  TEXT("\\\\.\\pipe\\AmboReadPipe"),                 // pipe name
	  PIPE_ACCESS_INBOUND,                           // pipe open mode
	  PIPE_TYPE_MESSAGE|PIPE_READMODE_MESSAGE|PIPE_WAIT,       // pipe-specific modes
	  10,                        // maximum number of instances
	  1024,                       // output buffer size
	  1024,                        // input buffer size
	  500,                      // time-out interval
	  NULL// SD
	);
	m_hWritePipe = CreateNamedPipe(
	  TEXT("\\\\.\\pipe\\AmboWritePipe"),                 // pipe name
	  PIPE_ACCESS_OUTBOUND,                           // pipe open mode
	  PIPE_TYPE_MESSAGE|PIPE_READMODE_MESSAGE|PIPE_WAIT,       // pipe-specific modes
	  10,                        // maximum number of instances
	  1024,                       // output buffer size
	  1024,                        // input buffer size
	  500,                      // time-out interval
	  NULL// SD
	);
	m_hWriteHandle = CreateFile(TEXT("\\\\.\\pipe\\AmboReadPipe"),
		GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	m_hReadHandle = CreateFile(TEXT("\\\\.\\pipe\\AmboWritePipe"),
		GENERIC_READ,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if(!m_hReadPipe || !m_hReadHandle || !m_hWritePipe || !m_hWriteHandle){
		MessageBox(TEXT("Create pipe fail!"), TEXT("ERROR"), MB_OK);		
		
		return -1;
	}


//	m_bReadPipeStop = FALSE;
	m_bExit = FALSE;

	m_BuildClass.SetPipeInfo(m_hWriteHandle, m_hReadHandle, m_hWritePipe,m_hReadPipe);
	m_Thread = CreateThread(NULL,0,BuildThreadProc,this,0,0);
///	TRACE("Will wait ReadPipe Connecting ...");
//	WaitNamedPipe("\\\\.\\pipe\\AmboReadPipe",NMPWAIT_WAIT_FOREVER);
//	TRACE("The ReadPipe had Connected");
//	CreateThread(NULL,0,BuildThreadProc,this,0,0);
//	Execute (TEXT("gcc --verbose"));
//	Execute(lpszCmd);

	return 0;
}

void CAmboOutBar::OnSize(UINT nType, int cx, int cy) 
{
	CmlControlBar::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect (&rect);
	m_OutBar.MoveWindow (&rect);
}
void CAmboOutBar::OnKeyDown(int nChar, int nRepCnt, int nFlags) 
{
	// TODO: Add your message handler code here and/or call default
//		TestError();
		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

ULONG CALLBACK CAmboOutBar::BuildThreadProc( LPVOID lpParameter )
{
	CAmboOutBar *pOutBar = (CAmboOutBar *)lpParameter;
	TCHAR buffer[1025];
	DWORD dwReadlen = 0;
	DWORD dwAvail;


	while(1)
	{
		if (::PeekNamedPipe(pOutBar->m_hReadPipe, NULL, 0, 0, &dwAvail, NULL))
		{
			if (dwAvail != 0)
			{
				ReadFile(pOutBar->m_hReadPipe,buffer,1024,&dwReadlen,NULL);
				if (dwReadlen)
				{
					buffer[dwReadlen] =0;
	//				pOutBar->m_OutBar.AddString(pOutBar->m_BuildIndex,"F:\\WORK\\eBuilder\\Ambo\\Builder\\OutputMessageEdit.cpp:959: error C2146: syntax error : missing ';' before identifier 'LineInfo1'\n");
					pOutBar->m_OutBar.AddString(pOutBar->m_BuildIndex,buffer);
				}
			}
		}
		if (pOutBar->m_bExit)
		{
//			::KillTimer (GetSafeHwnd(), UPDATE_TIME);
			break;
		}
		Sleep(10);
//		if (pOutBar->bReadPipeStop == TRUE)
//			break;
	}
	return 0;
}

//void CAmboOutBar::Execute(LPCTSTR lpszCmd)
//{
//	::SetTimer (GetSafeHwnd (), UPDATE_TIME, 100, NULL);

/*	m_OutBar.GetTabEdit(m_BuildIndex).Clear();
	CreateThread(NULL,0,BuildThreadProc,this,0,0);
	m_Cygwin.Run (lpszCmd, m_hWriteHandle, m_hReadHandle, m_hWritePipe);
*/
/*
	CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

	if (pMainFrame)
	{
		if (pMainFrame->m_workBar.m_pProjectInfo)
		{
			m_Cygwin.SetProjectPath(pMainFrame->m_workBar.m_pProjectInfo->m_strProjectRoot);
		}
		else if (pMainFrame->m_workBar.m_pWorkSpaceInfo)
		{
			//已经打开了一个WorkSpace,我们将编译这个WorkSpace
			m_Cygwin.SetProjectPath(pMainFrame->m_workBar.m_pWorkSpaceInfo->m_strWorkSpaceRoot);
		}
		else
		{
			return;
		}
	}
	else
	{
		return ;
	}

	m_OutBar.GetTabEdit(m_BuildIndex).Clear();
	CreateThread(NULL,0,BuildThreadProc,this,0,0);
	if (pMainFrame->m_workBar.m_pWorkSpaceInfo->GetBuildFlag())
	{
		m_Cygwin.Run (lpszCmd, m_hWriteHandle, m_hReadHandle, m_hWritePipe);
	}
	else
	{
		m_Cygwin.Run ("kbuild -b", m_hWriteHandle, m_hReadHandle, m_hWritePipe);
		pMainFrame->m_workBar.m_pWorkSpaceInfo->SetBuildFlag(TRUE);
	}
*/
//}


// 编译当前打开的文件
BOOL CAmboOutBar::Compile(CString &strPath)
{
	/*CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

	if (pMainFrame)
	{
		CAmboFrame* pChildFrame = (CAmboFrame*)pMainFrame->MDIGetActive();
		if (pChildFrame)
		{
			CAmboView* pView = (CAmboView*)pChildFrame->GetActiveView();
		
			if (pView)
			{
				CAmboDoc*pDoc = pView->GetDocument();
				CString strFileName;
				CString strCmd;
				strFileName = pDoc->GetPathName();
				//MessageBox(strFileName);
//				strCmd = "gcc ";
//				strCmd += strFileName;
//				Execute(strCmd);
				Execute (TEXT("gcc --verbose"));
				return TRUE;
			}
		}
	}*/
	/*
	CString strCurPath,strFileName;
	int nFileNamePos = strPath.GetLength();
	LPTSTR lpPos = strPath.LockBuffer() + nFileNamePos;
	while(nFileNamePos>0)
	{
		if (*lpPos == '\\')
			break;
		lpPos --;
		nFileNamePos --;
	}
	strPath.UnlockBuffer();
	if (nFileNamePos == 0)
		return FALSE;

	strCurPath = strPath.Left(nFileNamePos);
	strFileName = strPath.Right(strPath.GetLength() - nFileNamePos -1);

	CString cCmd;

	cCmd.Format(TEXT("kbuild -s %s/%s %s"),m_csCygdrive,strCurPath,strFileName);
	cCmd.Replace (TEXT(":\\"), TEXT("/"));
	cCmd.Replace (TEXT('\\'), TEXT('/'));
	Execute(cCmd);
	*/
	ActiveBuildMessage();
	m_OutBar.GetTabEdit(m_BuildIndex).Clear();
	m_BuildClass.CompileFile(strPath);   
	return TRUE;
}

// 编译当前打开的工程
BOOL CAmboOutBar::Build()
{
/*	CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

	if (pMainFrame)
	{
		if (pMainFrame->m_workBar.m_pProjectInfo)
		{
			//已经打开了一个Project,我们将编译这个Project
//			MessageBox(pMainFrame->m_workBar.m_pProjectInfo->m_strFileName);
//			Execute("");
			CString cCmd;

//				csCommand.Format (TEXT("%s/%s"),m_csCygdrive, pMainFrame->m_workBar.m_pProjectInfo->m_strProjectRoot);
				cCmd.Format(TEXT("kbuild %s/%s -bm -r"),m_csCygdrive,pMainFrame->m_workBar.m_pProjectInfo->m_strProjectRoot);
				cCmd.Replace (TEXT(":\\"), TEXT("/"));
				cCmd.Replace (TEXT('\\'), TEXT('/'));
				Execute(cCmd);
				return TRUE;
		}
		else if (pMainFrame->m_workBar.m_pWorkSpaceInfo)
		{
			//已经打开了一个WorkSpace,我们将编译这个WorkSpace
//			MessageBox(pMainFrame->m_workBar.m_pWorkSpaceInfo->m_strFileName);
//			Execute("build");
			Execute("kbuild -b");
			//MakeImage();
			return TRUE;
		}
	}
	*/

	CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;
	ASSERT(pMainFrame);
	if (pMainFrame->m_workBar.m_pProjectInfo)
	{
		if (pMainFrame->m_workBar.m_pProjectInfo->IsProjectOpen())
		{
			ActiveBuildMessage();
			m_OutBar.GetTabEdit(m_BuildIndex).Clear();
			m_BuildClass.Build();
			return TRUE;
		}
	}

	if (pMainFrame->m_workBar.m_pWorkSpaceInfo)
	{
		if (pMainFrame->m_workBar.m_pWorkSpaceInfo->IsWorkSpaceOpen())
		{
			ActiveBuildMessage();
			m_OutBar.GetTabEdit(m_BuildIndex).Clear();
			m_BuildClass.Build();
			pMainFrame->m_workBar.m_pWorkSpaceInfo->SetBuildFlag(TRUE);
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CAmboOutBar::ReBuild()
{
	//Execute("kbuild kingmos -cb");
//	Execute("kbuild -cb");
	ActiveBuildMessage();
	m_OutBar.GetTabEdit(m_BuildIndex).Clear();
	m_BuildClass.ReBuild();
	//MakeImage();
	return FALSE;
}
BOOL CAmboOutBar::MakeImage()
{
/*	CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;
	int iVersion = VERSION_B; 

	if (pMainFrame)
	{
		if (pMainFrame->m_workBar.m_pProjectInfo)
		{
			m_Cygwin.SetProjectPath(pMainFrame->m_workBar.m_pProjectInfo->m_strProjectRoot);
		}
		else if (pMainFrame->m_workBar.m_pWorkSpaceInfo)
		{
			//已经打开了一个WorkSpace,我们将编译这个WorkSpace
			m_Cygwin.SetProjectPath(pMainFrame->m_workBar.m_pWorkSpaceInfo->m_strWorkSpaceRoot);
//			iVersion = pMainFrame->m_workBar.m_pWorkSpaceInfo->GetVersion();
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	m_OutBar.GetTabEdit(m_BuildIndex).Clear();
	CreateThread(NULL,0,BuildThreadProc,this,0,0);
	if (pMainFrame->m_workBar.m_pWorkSpaceInfo->GetBuildFlag())
	{
		
		m_Cygwin.MakeimgConsole(m_hWriteHandle, m_hReadHandle, m_hWritePipe,iVersion);
	}
	else
	{
		//MessageBox(NULL, TEXT("The project haven't builded !!!"), TEXT("ERROR"), MB_OK);				
	}
*/
	ActiveBuildMessage();
	m_OutBar.GetTabEdit(m_BuildIndex).Clear();
	m_BuildClass.MakeImage();
	return FALSE;

}

BOOL CAmboOutBar::Clean()
{
/*	CString		csPath;
	CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

	if (!pMainFrame){
		return FALSE;
	}

	if ((!pMainFrame->m_workBar.m_pProjectInfo) &&
		(!pMainFrame->m_workBar.m_pWorkSpaceInfo)){
		return FALSE;
	}

	
	if (pMainFrame->m_workBar.m_pWorkSpaceInfo->GetBuildFlag()){			

		if(pMainFrame->m_workBar.m_pProjectInfo){
			csPath.Format(TEXT("kbuild -c %s/%s"),m_csCygdrive, pMainFrame->m_workBar.m_pProjectInfo->m_strProjectRoot);
		}else{
			csPath.Format(TEXT("kbuild -c %s/%s"),m_csCygdrive, pMainFrame->m_workBar.m_pWorkSpaceInfo->m_strWorkSpaceRoot);
		}
		//cCmd.Format(TEXT("kbuild -cm %s/%s"),m_csCygdrive,strPath);
		csPath.Replace (TEXT(":\\"), TEXT("/"));
		csPath.Replace (TEXT('\\'), TEXT('/'));
			
		Execute(csPath.GetBuffer(csPath.GetLength()));
	}
*/
	ActiveBuildMessage();
	m_OutBar.GetTabEdit(m_BuildIndex).Clear();
	m_BuildClass.Clean();
	return TRUE;
}
BOOL CAmboOutBar::Console()
{/*
	CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

	if (pMainFrame)
	{
		if (pMainFrame->m_workBar.m_pProjectInfo)
		{
			m_Cygwin.SetProjectPath(pMainFrame->m_workBar.m_pProjectInfo->m_strProjectRoot);
		}
		else if (pMainFrame->m_workBar.m_pWorkSpaceInfo)
		{
			//已经打开了一个WorkSpace,我们将编译这个WorkSpace
			m_Cygwin.SetProjectPath(pMainFrame->m_workBar.m_pWorkSpaceInfo->m_strWorkSpaceRoot);
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	return m_Cygwin.Console();
	*/
	return 	m_BuildClass.Console();
}
/*
BOOL CAmboOutBar::CompileFearure(CString &strPath)
{
	m_OutBar.GetTabEdit(m_BuildIndex).Clear();
	m_BuildClass.CompileModuleorDir(strPath, TRUE);
	return TRUE;
}
*/
BOOL CAmboOutBar::CompileDirectory(CString &strPath)
{
	CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;
	ASSERT(pMainFrame);
	ActiveBuildMessage();
	m_OutBar.GetTabEdit(m_BuildIndex).Clear();
	if (pMainFrame->m_workBar.m_pWorkSpaceInfo->GetBuildFlag())
	{
		m_BuildClass.CompileModuleorDir(strPath, FALSE);
	}
	else
	{
		m_BuildClass.Build();
		pMainFrame->m_workBar.m_pWorkSpaceInfo->SetBuildFlag(TRUE);
	}
	return TRUE;
}
BOOL CAmboOutBar::CompileModule(CString &strPath)
{
	CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;
	ASSERT(pMainFrame);
	ActiveBuildMessage();
	m_OutBar.GetTabEdit(m_BuildIndex).Clear();
	if (pMainFrame->m_workBar.m_pWorkSpaceInfo->GetBuildFlag())
	{
	m_BuildClass.CompileModuleorDir(strPath, TRUE);
	}
	else
	{
		m_BuildClass.Build();
		pMainFrame->m_workBar.m_pWorkSpaceInfo->SetBuildFlag(TRUE);
	}
	return TRUE;
}

void CAmboOutBar::SetWorkspacePath(CString &strWorkspacePath)
{
//	m_BuildClass.SetPlatformLocation(strWorkspacePath);	//设置workspace路径
}

void CAmboOutBar::SetProjectPath(CString &strProjectPath)
{
//	m_BuildClass.SetProjectLocation(strProjectPath);	//设置project路径
}

void CAmboOutBar::OutputFindmessage(CString strMessage)
{
	m_OutBar.AddString(m_FindInFilesIndex,strMessage);
}

void CAmboOutBar::ClearFindInFilesMessage()
{
	m_OutBar.GetTabEdit(m_FindInFilesIndex).Clear();
}

void CAmboOutBar::ActiveFindInFilesMessage()
{
//	m_OutBar.SetCurSel(m_FindInFilesIndex);
//	m_OutBar.SetCurFocus(m_FindInFilesIndex);
//	m_OutBar.ShowWindow(TRUE);
	CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;
	ASSERT(pMainFrame);

	pMainFrame->ShowControlBar(this, TRUE, FALSE);
	m_OutBar.SetActiveView (m_FindInFilesIndex);
}

BOOL CAmboOutBar::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
//	if (pMsg->message == WM_KEYDOWN)
//	{
//
//		return 0;
//	}
	
	return CmlControlBar::PreTranslateMessage(pMsg);
}

void CAmboOutBar::ActiveBuildMessage()
{
	CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;
	ASSERT(pMainFrame);

	pMainFrame->ShowControlBar(this, TRUE, FALSE);
	m_OutBar.SetActiveView (m_BuildIndex);
}

CView * CAmboOutBar::GetActiveView()
{
	return m_OutBar.GetActiveView();
}
