// MessageView.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "MessageView.h"
#include "FindText.h"
#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMessageView

IMPLEMENT_DYNCREATE(CMessageView, CView)

CMessageView::CMessageView()
{
	m_hMessageEdit = new COutputMessageEdit;
}

CMessageView::~CMessageView()
{
	if (m_hMessageEdit)
		delete m_hMessageEdit;
}


BEGIN_MESSAGE_MAP(CMessageView, CView)
	//{{AFX_MSG_MAP(CMessageView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessageView drawing

void CMessageView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CMessageView diagnostics

#ifdef _DEBUG
void CMessageView::AssertValid() const
{
	CView::AssertValid();
}

void CMessageView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMessageView message handlers

void CMessageView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_hMessageEdit->SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE|SWP_NOZORDER);
	
}

int CMessageView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	RECT rectClient;
	GetClientRect(&rectClient);
	
	// TODO: Add your specialized creation code here
	m_hMessageEdit->Create( ES_AUTOHSCROLL|ES_AUTOVSCROLL|ES_MULTILINE|ES_WANTRETURN|WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL,
					rectClient,
					this,
					1101);
	
	return 0;
}
void CMessageView::OnKeyDown(int nChar, int nRepCnt, int nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	m_hMessageEdit->OnKeyDown(nChar, nRepCnt, nFlags);
	//CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CMessageView::AddString(LPCTSTR lpMessage)
{
	return m_hMessageEdit->AddString(lpMessage);
}

void CMessageView::SetWindowsFileInfo(BOOL bWindowsFileInfo)
{
	m_hMessageEdit->SetWindowsFileInfo(bWindowsFileInfo);
}

void CMessageView::OnEditFind() 
{
	// TODO: Add your command handler code here
	CFindText dlg;
	CString strSearch;

	CMainFrame* pMainFrame ;
	CComboBox *pSearchInputComboBox;
	pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

	m_hMessageEdit->GetCursorWord(strSearch);
//	m_hMessageEdit->SetSearchString(strSearch);
	((CAmboApp *)AfxGetApp())->SetSearchString(strSearch);
	pSearchInputComboBox = pMainFrame->m_wndEditBar.GetComboBox(ID_SEARCHINPUT);

//	dlg.m_pSearchComboBox = pSearchInputComboBox;
	dlg.m_strText = strSearch;//m_hMessageEdit->m_strSearch;
	dlg.m_bMatchAllWordOnly = m_hMessageEdit->m_bMatchAllWordOnly;
	dlg.m_bMatchCase = m_hMessageEdit->m_bMatchCase;
	if (m_hMessageEdit->iDirection == DIRECTION_UP)
		dlg.m_iDir = 0;
	else
		dlg.m_iDir = 1;

	if (dlg.DoModal() == IDCANCEL)
	{
		return ;
	}

	if (dlg.nFindStyle == -1 )
	{
		return ;
	}
	
	m_hMessageEdit->m_strSearch = dlg.m_strText;
	m_hMessageEdit->m_bMatchAllWordOnly = dlg.m_bMatchAllWordOnly;
	m_hMessageEdit->m_bMatchCase = dlg.m_bMatchCase;
	if (dlg.m_iDir == 0)
		m_hMessageEdit->iDirection = DIRECTION_UP;
	else
		m_hMessageEdit->iDirection = DIRECTION_DOWN;

	if (dlg.nFindStyle == FIND_FINDNEXT)
	{
		 ((CAmboApp *)AfxGetApp())->SetSearchString(dlg.m_strText);
		 m_hMessageEdit->SearchString(CPoint(-1,-1), dlg.m_strText,dlg.m_bMatchAllWordOnly,dlg.m_bMatchCase,m_hMessageEdit->iDirection);
	}
//	else
//	{
//		 m_hMessageEdit->MarkAllString(dlg.m_strText,dlg.m_bMatchAllWordOnly,dlg.m_bMatchCase);
//	}
//	m_hMessageEdit->SetSearchString(dlg.m_strText);
	
	::SetFocus(m_hMessageEdit->m_hWnd);
	
}
/*
void CMessageView::OnUpdateEditFind(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(::GetFocus() == m_hMessageEdit->m_hWnd);	
	
}
*/

BOOL CMessageView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
//	if (pMsg->message == WM_KEYDOWN)
//	{
//		return 0;
//	}
	
	return CView::PreTranslateMessage(pMsg);
}

void CMessageView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	
	m_hMessageEdit->SetFocus();
}
