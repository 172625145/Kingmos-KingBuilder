// mlOutTab.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "OutputView.h"
#include "MessageView.h"
#include "mlOutTab.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define SCROLLBAR_WIDTH					16
#define SCROLLBAR						40
#define TABS_WIDTH						200
#define DRAW_GAP						4
/////////////////////////////////////////////////////////////////////////////
// CmlOutTab

CmlOutTab::CmlOutTab()
{
}

CmlOutTab::~CmlOutTab()
{
}
COutputMessageEdit& CmlOutTab::GetTabEdit(int iItem)
{
	OTC_ITEM *pTabCtrlItem;
	pTabCtrlItem = (OTC_ITEM *)GetItemData(iItem);
	COutputView *pView = (COutputView *)pTabCtrlItem->pWnd;
	return *(pView->m_hMessageEdit);
}

BEGIN_MESSAGE_MAP(CmlOutTab, CTabCtrl)
	//{{AFX_MSG_MAP(CmlOutTab)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TCN_SELCHANGING, OnSelchanging)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CmlOutTab message handlers

void CmlOutTab::OnSize(UINT nType, int cx, int cy) 
{
	CTabCtrl::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	int nTotal = GetItemCount ();
	CRect rect;
	GetViewRect (rect);

	for (int i = 0; i < nTotal; i ++)
	{
		OTC_ITEM *pItem = GetItemData (i);
		ASSERT (pItem != NULL);

		pItem->pWnd->MoveWindow (&rect);
	}
	
}

OTC_ITEM * CmlOutTab::GetItemData(int nItem)
{
	ASSERT ((nItem >= 0) && (nItem < GetItemCount ()));
	
	TC_ITEM tci;
	tci.mask = TCIF_PARAM;

	if (! GetItem (nItem, &tci))
		return FALSE;

	return (OTC_ITEM *)(tci.lParam);
}

int CmlOutTab::AddView(LPCTSTR lpszLabel, CRuntimeClass *pViewClass,int iIndex)
{
//	int iIndex;
	ASSERT(lpszLabel != NULL);
	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CView)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));

	COutputView *pView;
	TRY
	{
		pView = (COutputView*)pViewClass->CreateObject();
		if (pView == NULL)
			AfxThrowMemoryException();
	}
	CATCH_ALL(e)
	{
		TRACE0("Out of memory creating a view.\n");
		// Note: DELETE_EXCEPTION(e) not required
		return FALSE;
	}
	END_CATCH_ALL
		
	DWORD dwStyle = AFX_WS_DEFAULT_VIEW;
	CCreateContext context;
	CRect rect;
	GetViewRect (rect);
	// Create with the right size and position
	if (! pView->Create (NULL, NULL/*lpszLabel*/, dwStyle, rect, this, AFX_IDW_PANE_FIRST, &context))
	{
		TRACE0("Warning: couldn't create client pane for view.\n");
		// pWnd will be cleaned up by PostNcDestroy
		return FALSE;
	}
	pView->ModifyStyle (WS_HSCROLL, WS_CLIPSIBLINGS);
//	pView->ModifyStyle (WS_HSCROLL|WS_VSCROLL, WS_CLIPSIBLINGS);
//	pView->ModifyStyleEx (WS_EX_CLIENTEDGE, 0);
	pView->SendMessage(WM_INITIALUPDATE, 0, 0);
	
	OTC_ITEM *pItem = new OTC_ITEM;
	pItem->pWnd = pView;
	_tcscpy (pItem->lpszLabel, lpszLabel);

	TC_ITEM tci;
	tci.mask = TCIF_TEXT | TCIF_PARAM;
	tci.pszText = (LPTSTR)(LPCTSTR)lpszLabel;
	tci.lParam = (LPARAM)pItem;
//	iIndex = InsertItem (GetItemCount (), &tci);
	iIndex = InsertItem (iIndex, &tci);


//	pView->GetEditCtrl().SetWindowText("Abcd\r\nefgh");

	return iIndex;
}

void CmlOutTab::GetViewRect(CRect &rect)
{
	GetClientRect (&rect);
//	rect.DeflateRect (0, 0, SCROLLBAR_WIDTH, SCROLLBAR_WIDTH);
	AdjustRect (FALSE, &rect);
	rect.InflateRect (1, 1, 1, -1);
}

int CmlOutTab::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
//	ModifyStyleEx (0, WS_EX_CLIENTEDGE);

	return 0;
}

void CmlOutTab::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OTC_ITEM *pItem = GetItemData (GetCurSel ());
	ASSERT (pItem);

	pItem->pWnd->ShowWindow (SW_HIDE);
	pItem->pWnd->UpdateWindow ();

	*pResult = 0;
}

void CmlOutTab::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	OTC_ITEM *pItem = GetItemData (GetCurSel ());
	ASSERT (pItem);

	pItem->pWnd->ShowWindow (SW_SHOW);
	pItem->pWnd->UpdateWindow ();

	*pResult = 0;
}

// 在指定索引的View中插入一个字串
BOOL CmlOutTab::AddString (UINT iViewIndex,LPCTSTR lpstrContent)
{
	OTC_ITEM *pItem ;
	COutputView *pView;
	CString strMsg;

	pItem = GetItemData(iViewIndex);
	pView = (COutputView *)pItem->pWnd;
	pView->AddString(lpstrContent);
	// 得到原有的内容
//	pView->GetEditCtrl().GetWindowText(strMsg);
	// 追加要加入的串
//	strMsg += lpstrContent;
//	strMsg += "\r\n";
	// 设置新的内容
//	pView->GetEditCtrl().SetWindowText(strMsg);
	return TRUE;
}
BOOL CmlOutTab::Empty (UINT iViewIndex)
{
	OTC_ITEM *pItem ;
	COutputView *pView;

	pItem = GetItemData(iViewIndex);
	pView = (COutputView *)pItem->pWnd;
	// 设置新的内容为空
//	pView->GetEditCtrl().SetWindowText("");
	return TRUE;
}

//删除一个指定的VIEW
void CmlOutTab::RemoveViewAt (int nView)
{
	ASSERT ((nView >= 0) && (nView < GetItemCount ()));

	OTC_ITEM *pMember = GetItemData (nView);
	ASSERT (pMember != NULL);

	DeleteItem (nView);

	delete pMember;
}

//删除所有的VIEW
void CmlOutTab::RemoveAllView()
{
	int nTotal = GetItemCount ();

	while(nTotal > 0)
	{
		OTC_ITEM *pMember = GetItemData (--nTotal);
		ASSERT (pMember != NULL);

		COutputView *pView = (COutputView *)pMember->pWnd;
		//delete pView;
		pView->DestroyWindow();

		delete pMember;
	}
	DeleteAllItems ();
}

void CmlOutTab::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	//关闭窗口之前要删除所有已创建的VIEW
	RemoveAllView();
	
	CTabCtrl::OnClose();
}

void CmlOutTab::OnDestroy() 
{
	RemoveAllView();

	CTabCtrl::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
void CmlOutTab::OnKeyDown(int nChar, int nRepCnt, int nFlags) 
{
	// TODO: Add your message handler code here and/or call default
		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
void CmlOutTab::SetActiveView(int nNewTab)
{
	int nCurSel = GetCurSel ();

	if ((nNewTab >=0) && (nNewTab < GetItemCount ()) && (nNewTab != nCurSel))
	{
		OTC_ITEM *newMember = GetItemData (nNewTab);
		OTC_ITEM *oldMember = GetItemData (nCurSel);
		ASSERT (newMember != NULL);
		ASSERT (oldMember != NULL);

//		oldMember->pWnd->EnableWindow (FALSE);
		oldMember->pWnd->ShowWindow (SW_HIDE);
		oldMember->pWnd->UpdateWindow ();

//		newMember->pWnd->EnableWindow(TRUE);
		newMember->pWnd->ShowWindow(SW_SHOW);
//		newMember->pWnd->EnableWindow (TRUE);
		newMember->pWnd->UpdateWindow ();
		newMember->pWnd->SetFocus();

		// select the tab (if tab programmatically changed)
		SetCurSel (nNewTab);
    }
	
}

void CmlOutTab::SetWindowsFileInfo(UINT iViewIndex,BOOL bWindowsFileInfo)
{
	OTC_ITEM *pItem ;
	COutputView *pView;
	CString strMsg;

	pItem = GetItemData(iViewIndex);
	pView = (COutputView *)pItem->pWnd;
	pView->SetWindowsFileInfo(bWindowsFileInfo);
}

void CmlOutTab::OnSetFocus(CWnd* pOldWnd) 
{
//	CTabCtrl::OnSetFocus(pOldWnd);
	
	pOldWnd->SetFocus();
	// TODO: Add your message handler code here
	
}

BOOL CmlOutTab::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class	
//	if (pMsg->message == WM_KEYDOWN)
//	{
//
//		return 0;
//	}
	return CTabCtrl::PreTranslateMessage(pMsg);
}

CView * CmlOutTab::GetActiveView()
{
	int nCurSel = GetCurSel ();
	OTC_ITEM *pItem ;
	CView *pView;

	pItem = GetItemData(nCurSel);
	pView = (CView *)pItem->pWnd;
	return pView;
}
