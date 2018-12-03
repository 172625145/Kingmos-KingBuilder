// MainFrame.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Ambo.h"

#include "MainFrame.h"
#include "TradeMarkFrame.h"

#include "AmboFrame.h"
#include "AmboDoc.h"
#include "AmboView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_VIEW_OUTPUT, OnViewOutput)
	ON_COMMAND(ID_VIEW_STATUS_BAR, OnViewStatusBar)
	ON_COMMAND(ID_VIEW_WORKSPACE, OnViewWorkspace)
	ON_COMMAND(ID_VIEW_EDITBAR, OnViewEditbar)
	ON_COMMAND(ID_VIEW_TOOLBAR, OnViewToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EDITBAR, OnUpdateViewEditbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT, OnUpdateViewOutput)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS_BAR, OnUpdateViewStatusBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, OnUpdateViewToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE, OnUpdateViewWorkspace)
	ON_COMMAND(ID_VIEW_BUILDBAR, OnViewBuildbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BUILDBAR, OnUpdateViewBuildbar)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_NEXT, OnUpdateWindowNext)
	ON_COMMAND(ID_WINDOW_NEXT, OnWindowNext)
	ON_WM_SETFOCUS()
	ON_WM_ACTIVATEAPP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

AFX_STATIC_DATA const TCHAR szWndClass[] = _T("KingBuilder");

#define ID_INDICATOR_POSITION  1000

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,            // special separator value
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	CTradeMarkFrame dlgTradeMark;

	dlgTradeMark.DoModal();		
	RegisterWindowClass();
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,IDW_MAINTOOLBAR) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	TRACE("****m_wndToolBar = %X\n",m_wndToolBar);

	if (!m_wndEditBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,IDW_EDITBAR) ||
		!m_wndEditBar.LoadToolBar(IDR_EDITBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndEditBar.AddComboBox(ID_SEARCHINPUT,150,CBS_DROPDOWN |CBS_AUTOHSCROLL | WS_VSCROLL | CBS_HASSTRINGS);
	((CAmboApp *)AfxGetApp())->InitialFindTextComboBox(m_wndEditBar.GetComboBox(ID_SEARCHINPUT));


	//添加编译工具条
	if (!m_wndBuildBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,IDW_BUILDBAR) ||
		!m_wndBuildBar.LoadToolBar(IDR_BUILDBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
//	int iIndex = m_wndStatusBar.CommandToIndex( ID_SEPARATOR );
	m_wndStatusBar.SetPaneInfo( 1, ID_INDICATOR_POSITION, SBPS_NORMAL, 60);
	

	if (! m_workBar.Create (TEXT("WorkSpace"), this, IDW_AMBOWORKBAR,
		WS_VISIBLE|WS_CHILD|CBRS_LEFT|CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))
	{
		TRACE0("Failed to create workBar \n");
		return -1;      // fail to create
	}
	
	TRACE("****m_workBar= %X\n",m_workBar);
	if (! m_outBar.Create (TEXT("OutPut"), this, IDW_AMBOOUTBAR,
		WS_VISIBLE|WS_CHILD|CBRS_BOTTOM|CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))
	{
		TRACE0("Failed to create workBar \n");
		return -1;      // fail to create
	}

	TRACE("****m_outBar= %X\n",m_outBar);
	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	EnableDocking(CBRS_ALIGN_ANY);
//	m_pFloatingFrameClass = RUNTIME_CLASS(CmlMiniDockFrameWnd);


	m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP);
	DockControlBar(&m_wndToolBar);

	m_wndEditBar.EnableDocking(CBRS_ALIGN_TOP);
	DockControlBar(&m_wndEditBar);

	m_wndBuildBar.EnableDocking(CBRS_ALIGN_TOP);
	DockControlBar(&m_wndBuildBar);

	RECT rectWorkBar;
	m_workBar.GetWindowRect(&rectWorkBar);
	rectWorkBar.right = rectWorkBar.right + 230 ;
	m_workBar.EnableDocking (CBRS_ALIGN_ANY);
	DockControlBar(&m_workBar,(CDockBar *)0, &rectWorkBar);

	m_outBar.EnableDocking (CBRS_ALIGN_ANY);
	DockControlBar(&m_outBar);

	LoadBarState("Ambo.pfl");

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.lpszClass = szWndClass;
	cs.style |= WS_MAXIMIZE ;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::OpenProject(LPCTSTR lpstrDoc)
{
	if (! m_workBar.OpenDocument (lpstrDoc))
		return FALSE;

	return TRUE;
}



void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	// 设置没有激活窗口
	SetActiveView(NULL,FALSE);

	if (m_outBar.IsRun())
	{
		MessageBox("系统正在编译，请首先停止编译");
		return;
	}

	if(((CAmboApp *)AfxGetApp())->m_grep.IsComplete() == FALSE)
	{
		//正在进行查找，停止查找
		MessageBox("系统正在查找，请首先停止运行");
		return ;
	}
	// 首先要关闭工程
	if (m_workBar.m_pWorkSpaceInfo)
	{
		if (m_workBar.m_pWorkSpaceInfo->IsWorkSpaceOpen() == TRUE)		
		{
			m_workBar.m_pWorkSpaceInfo->CloseWorkSpace();
		}
	}
	if (m_workBar.m_pProjectInfo)
	{
		if (m_workBar.m_pProjectInfo->IsProjectOpen() == TRUE)
		{
			m_workBar.m_pProjectInfo->CloseProject();
		}
	}

	SaveBarState("Ambo.pfl");
		
	CMDIFrameWnd::OnClose();
}

void CMainFrame::OnViewOutput() 
{
	// TODO: Add your command handler code here
	OnBarCheck(IDW_AMBOOUTBAR);
	
}

void CMainFrame::OnViewStatusBar() 
{
	// TODO: Add your command handler code here
//	CMDIFrameWnd::OnViewStatusBar();	
	OnBarCheck(AFX_IDW_STATUS_BAR);
}

void CMainFrame::OnViewWorkspace() 
{
	// TODO: Add your command handler code here
	OnBarCheck(IDW_AMBOWORKBAR);	
}


void CMainFrame::OnViewEditbar() 
{
	// TODO: Add your command handler code here
	OnBarCheck(IDW_EDITBAR);		
}

void CMainFrame::OnViewToolbar() 
{
	// TODO: Add your command handler code here
	OnBarCheck(IDW_MAINTOOLBAR);			
}

void CMainFrame::OnUpdateViewEditbar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
 	CControlBar* pBar = GetControlBar(IDW_EDITBAR);
 	if (pBar != NULL)
 	{
 		pCmdUI->SetCheck((pBar->GetStyle() & WS_VISIBLE) != 0);
 		return;
 	}
}

void CMainFrame::OnUpdateViewOutput(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
 	CControlBar* pBar = GetControlBar(IDW_AMBOOUTBAR);
 	if (pBar != NULL)
 	{
 		pCmdUI->SetCheck((pBar->GetStyle() & WS_VISIBLE) != 0);
 		return;
 	}
}

void CMainFrame::OnUpdateViewStatusBar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
 	CControlBar* pBar = GetControlBar(AFX_IDW_STATUS_BAR);
 	if (pBar != NULL)
 	{
 		pCmdUI->SetCheck((pBar->GetStyle() & WS_VISIBLE) != 0);
 		return;
 	}
}

void CMainFrame::OnUpdateViewToolbar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
 	CControlBar* pBar = GetControlBar(IDW_MAINTOOLBAR);
 	if (pBar != NULL)
 	{
 		pCmdUI->SetCheck((pBar->GetStyle() & WS_VISIBLE) != 0);
 		return;
 	}
}

void CMainFrame::OnUpdateViewWorkspace(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
 	CControlBar* pBar = GetControlBar(IDW_AMBOWORKBAR);
 	if (pBar != NULL)
 	{
 		pCmdUI->SetCheck((pBar->GetStyle() & WS_VISIBLE) != 0);
 		return;
 	}
}

void CMainFrame::OnViewBuildbar() 
{
	// TODO: Add your command handler code here
	OnBarCheck(IDW_BUILDBAR);			
}

void CMainFrame::OnUpdateViewBuildbar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
 	CControlBar* pBar = GetControlBar(IDW_BUILDBAR);
 	if (pBar != NULL)
 	{
 		pCmdUI->SetCheck((pBar->GetStyle() & WS_VISIBLE) != 0);
 		return;
 	}	
}

//BOOL CMainFrame::CreateNewWorkSpace(CString strWorkSpaceName, CString strLocation, UINT uCPUType)
BOOL CMainFrame::CreateNewWorkSpace(CString strWorkSpaceName, CString strLocation, CString m_strBSPName, CString strPlatformRoot)
{
//	return 	m_workBar.CreateNewWorkSpace(strWorkSpaceName, strLocation, uCPUType);
	return 	m_workBar.CreateNewWorkSpace(strWorkSpaceName, strLocation, m_strBSPName, strPlatformRoot);
}

void CMainFrame::OnUpdateWindowNext(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnWindowNext() 
{
	// TODO: Add your command handler code here
	SHORT nCtrl;
//	CMDIChildWnd *pActiveWindow;

		nCtrl = GetAsyncKeyState(VK_CONTROL); 

		MDINext();

		CAmboFrame* pChildFrame = (CAmboFrame*)MDIGetActive();
		if (pChildFrame)
		{
			CAmboView* pView = (CAmboView*)pChildFrame->GetActiveView();
			if (pView)
			{
				CSpecialEdit *hEdit;
					hEdit = pView->h_Edit;
					hEdit->SetFocus();
			}	
		}

}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( (LOWORD(wParam) == ID_SEARCHINPUT) &&(HIWORD(wParam) == IDCANCEL) )
	{
		// 将焦点设置到编辑窗口
/*		CView* pView = GetActiveFrame()->GetActiveView();
		if (pView)
			pView->SetFocus();
		return FALSE;
		*/
		CAmboFrame* pChildFrame = (CAmboFrame*)MDIGetActive();
		if (pChildFrame)
		{
			CAmboView* pView = (CAmboView*)pChildFrame->GetActiveView();
			if (pView)
			{
				CSpecialEdit *hEdit;
					hEdit = pView->h_Edit;
					hEdit->SetFocus();
			}	
		}
		return FALSE;
	}
//	if (LOWORD(wParam) >= AFX_IDM_FIRST_MDICHILD)
//	{
//		return CMDIFrameWnd::OnCommand(wParam, lParam);
//	}
	
	return CMDIFrameWnd::OnCommand(wParam, lParam);
}


BOOL CMainFrame::RegisterWindowClass()
{
	HINSTANCE hInst;
	hInst = AfxGetInstanceHandle();//(HINSTANCE)GetWindowLong(m_hWnd,GWL_HINSTANCE);

	HICON hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDR_MAINFRAME));
	WNDCLASS wndcls;

	// otherwise we need to register a new class
	wndcls.style = 0;
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
	wndcls.hInstance = hInst;
	wndcls.hIcon = hIcon;
	wndcls.hCursor = LoadCursor(hInst,IDC_ARROW);
	wndcls.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndcls.lpszMenuName = NULL;
	wndcls.lpszClassName = szWndClass;
	if (!AfxRegisterClass(&wndcls))
		AfxThrowResourceException();
	return TRUE;
}



/*
void CMainFrame::OnRestoreWindow()
{
}
*/

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

	return CMDIFrameWnd::WindowProc(message, wParam, lParam);
}

void CMainFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CMDIFrameWnd::OnSetFocus(pOldWnd);
	
//	((CAmboApp *)AfxGetApp())->ShowMessageAtDesktop(10,90,"CMainFrame:OnSetFocus");
	// TODO: Add your message handler code here
/*	CAmboFrame* pChildFrame = (CAmboFrame*)MDIGetActive();
	if (pChildFrame)
	{
		CAmboView* pView = (CAmboView*)pChildFrame->GetActiveView();
		if (pView)
		{
			CSpecialEdit *hEdit;
				hEdit = pView->h_Edit;
				hEdit->SetFocus();
		}	
	}
*/
}

void CMainFrame::OnActivateApp(BOOL bActive, HTASK hTask) 
{
	CMDIFrameWnd::OnActivateApp(bActive, hTask);
	
	// TODO: Add your message handler code here
	if( bActive )
		((CAmboApp *)AfxGetApp())->CheckAllDoc();
	
}
