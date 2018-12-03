// OutputView.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "OutputView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputView

IMPLEMENT_DYNCREATE(COutputView, CMessageView)

COutputView::COutputView()
{
}

COutputView::~COutputView()
{
}


BEGIN_MESSAGE_MAP(COutputView, CMessageView)
	//{{AFX_MSG_MAP(COutputView)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputView drawing

void COutputView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// COutputView diagnostics

#ifdef _DEBUG
void COutputView::AssertValid() const
{
	CMessageView::AssertValid();
}

void COutputView::Dump(CDumpContext& dc) const
{
	CMessageView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COutputView message handlers

void COutputView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
//	CMessageView::OnChar(nChar, nRepCnt, nFlags);
}

void COutputView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default

	if (nChar == VK_DELETE)
		return;
	
	CMessageView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void COutputView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(FALSE);			
}

void COutputView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(FALSE);			
}

void COutputView::OnChange() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CMessageView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void COutputView::OnEditPaste() 
{
	// TODO: Add your command handler code here
	
}

BOOL COutputView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		switch((UINT)pMsg->wParam)
		{
			case VK_LEFT:// to the left Char
			case VK_UP:// to the up line
			case VK_RIGHT://  to the right Char
			case VK_DOWN:// to the line down
				CMessageView::OnKeyDown(pMsg->wParam, 1, 0);
				return TRUE;
		}
	}
	
	return CMessageView::PreTranslateMessage(pMsg);
}

void COutputView::OnSetFocus(CWnd* pOldWnd) 
{
	CMessageView::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	
}
