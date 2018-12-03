// AmboFrame.cpp : implementation of the CAmboFrame class
//

#include "stdafx.h"
#include "Ambo.h"

#include "AmboFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAmboFrame

IMPLEMENT_DYNCREATE(CAmboFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CAmboFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CAmboFrame)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_MDIACTIVATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAmboFrame construction/destruction

CAmboFrame::CAmboFrame()
{
	// TODO: add member initialization code here
	
}

CAmboFrame::~CAmboFrame()
{
}

BOOL CAmboFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
/*
	BOOL bMaximized;
	if (cs.hwndParent != NULL)
	{
		CMDIFrameWnd* pFrameWnd;
		pFrameWnd = (CMDIFrameWnd*)CWnd::FromHandle(cs.hwndParent);
		CMDIChildWnd* pChild = pFrameWnd->MDIGetActive(&bMaximized);
		if (pChild == NULL)
		{
			if (((CAmboApp *)AfxGetApp())->GetWindowMax())
				cs.style |= WS_MAXIMIZE ;
		}
	}
	*/
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CAmboFrame diagnostics

#ifdef _DEBUG
void CAmboFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CAmboFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAmboFrame message handlers

//DEL void CAmboFrame::OnViewOutput() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	
//DEL }

//DEL void CAmboFrame::OnViewStatusBar() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	
//DEL }

//DEL void CAmboFrame::OnViewTesttoolbar() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	
//DEL }

//DEL void CAmboFrame::OnViewToolbar() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	
//DEL }

//DEL void CAmboFrame::OnViewWorkspace() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	
//DEL }

BOOL CAmboFrame::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	return CMDIChildWnd::DestroyWindow();
}

void CAmboFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
		
	CMDIChildWnd::OnClose();
}



BOOL CAmboFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CMDIChildWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext);

}

void CAmboFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
//	CMDIFrameWnd* pFrameWnd = GetMDIFrame();
//	ASSERT_VALID(pFrameWnd);

	if (nType == SIZE_MAXIMIZED)
	{
		// get maximized state of frame window (previously active child)
//	BOOL bMaximized;
//	pFrameWnd->MDIGetActive(&bMaximized);
	((CAmboApp *)AfxGetApp())->SetWindowMax(TRUE);
	}	
}

void CAmboFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	
	// TODO: Add your message handler code here
}

void CAmboFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (((CAmboApp *)AfxGetApp())->GetWindowMax())
	{
		BOOL bMaximized;
		CMDIFrameWnd* pFrameWnd = GetMDIFrame();
		ASSERT_VALID(pFrameWnd);
		pFrameWnd->MDIGetActive(&bMaximized);
		
		if (bMaximized == FALSE)
		{  // 判断是否需要重新最大化
			// get maximized state of frame window (previously active child)
			MDIMaximize();
		}
	}
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

LRESULT CAmboFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class	
	switch(message)
	{
		case WM_SYSCOMMAND:
			switch(wParam)
			{
				case SC_MAXIMIZE:
					((CAmboApp *)AfxGetApp())->SetWindowMax(TRUE);
					break;
				case SC_RESTORE:
					((CAmboApp *)AfxGetApp())->SetWindowMax(FALSE);
					break;
			}
			break;
	}
	return CMDIChildWnd::WindowProc(message, wParam, lParam);
}

BOOL CAmboFrame::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
//	return CMDIChildWnd::OnEraseBkgnd(pDC);
	return TRUE;
}

BOOL CAmboFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CMDIFrameWnd* pParentWnd, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	dwStyle &= ~WS_VISIBLE;
	return CMDIChildWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, pContext);
}
