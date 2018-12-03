// mlViewTab.cpp : implementation file
//

#include "stdafx.h"
//#include "mlClass.h"
#include "mlViewTab.h"
#include "resource.h"

#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define VIEWTAB_GAP						2
#define VIEWTAB_LEFT_GAP				2
#define VIEWTAB_RIGHT_GAP				3
#define VIEWTAB_TOP_GAP					2
#define VIEWTAB_BOTTOM_GAP				3
/////////////////////////////////////////////////////////////////////////////
// CmlViewTab

CmlViewTab::CmlViewTab() : m_pDocument (NULL)
{
}

CmlViewTab::~CmlViewTab()
{
}


BEGIN_MESSAGE_MAP(CmlViewTab, CTabCtrl)
//{{AFX_MSG_MAP(CmlViewTab)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(TCN_SELCHANGING, OnSelchanging)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CmlViewTab message handlers

void CmlViewTab::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	VTC_ITEM *pItem = GetItemData (GetCurSel());
	ASSERT (pItem != NULL);

	pItem->pWnd->EnableWindow(FALSE);
	pItem->pWnd->ShowWindow (SW_HIDE);
	pItem->pWnd->UpdateWindow ();

	*pResult = 0;
}

void CmlViewTab::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	VTC_ITEM *pItem = GetItemData (GetCurSel());
	ASSERT (pItem != NULL);

	pItem->pWnd->EnableWindow(TRUE);
	pItem->pWnd->ShowWindow (SW_SHOW);
	pItem->pWnd->UpdateWindow ();
	
	*pResult = 0;
}

int CmlViewTab::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
//	HINSTANCE hOldRes = AfxGetResourceHandle();

//	AfxSetResourceHandle (::GetModuleHandle (TEXT("mlClass.dll")));

//	if (! m_ImageList.Create (IDB_TABIMAGES, 16, 1, RGB(255,0,255)))
//	{
//		AfxSetResourceHandle (hOldRes);
//		return -1;
//	}
	m_ImageList.Create(16,16,ILC_COLOR8|ILC_MASK,16,16);
	//添加两个ICON到ImageList
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_MATTERVIEW));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_CONFIGVIEW));
	
//	AfxSetResourceHandle (hOldRes);


	SetImageList(&m_ImageList);
	
	// set "normal" GUI-font
	CFont *font = CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT));
	SetFont (font);
	
	return 0;
}

void CmlViewTab::OnSize(UINT nType, int cx, int cy) 
{
	CTabCtrl::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	RelayoutViews ();
}

void CmlViewTab::SetActiveView(int nNewTab)
{
	int nCurSel = GetCurSel ();

	if ((nNewTab >=0) && (nNewTab < GetItemCount ()) && (nNewTab != nCurSel))
	{
		VTC_ITEM *newMember = GetItemData (nNewTab);
		VTC_ITEM *oldMember = GetItemData (nCurSel);
		ASSERT (newMember != NULL);
		ASSERT (oldMember != NULL);

		oldMember->pWnd->EnableWindow (FALSE);
		oldMember->pWnd->ShowWindow (SW_HIDE);
		oldMember->pWnd->UpdateWindow ();

		newMember->pWnd->EnableWindow(TRUE);
		newMember->pWnd->ShowWindow(SW_SHOW);
		newMember->pWnd->EnableWindow (TRUE);
		newMember->pWnd->SetFocus();

		// select the tab (if tab programmatically changed)
		SetCurSel (nNewTab);
    }
}

CView* CmlViewTab::GetActiveView()
{
	if (GetItemCount () < 1)
		return NULL;

	VTC_ITEM *pItem = GetItemData (GetCurSel ());
	ASSERT (pItem != NULL);

	return (CView *)(pItem->pWnd);
}

CView* CmlViewTab::GetView(int nView)
{
	if ((nView >= 0) && (nView < GetItemCount ()))
	{
		VTC_ITEM *pMember = GetItemData (nView);
		ASSERT (pMember != NULL);

		return (CView*)(pMember->pWnd);
	}
	else
	{
		return NULL;
	}
}

void CmlViewTab::RemoveViewAt (int nView)
{
	ASSERT ((nView >= 0) && (nView < GetItemCount ()));

	VTC_ITEM *pMember = GetItemData (nView);
	ASSERT (pMember != NULL);

	DeleteItem (nView);

	delete pMember;
}

void CmlViewTab::RemoveAllView()
{
	int nTotal = GetItemCount ();

	while(nTotal > 0)
	{
		VTC_ITEM *pMember = GetItemData (--nTotal);
		ASSERT (pMember != NULL);
		CView *pView = (CView *)pMember->pWnd;
		//delete pView;
		pView->DestroyWindow();

		delete pMember;
	}
	DeleteAllItems ();
}

CDocument* CmlViewTab::SetDocument (CRuntimeClass *pDocClass)
{
	ASSERT(pDocClass != NULL);
	ASSERT(pDocClass->IsDerivedFrom(RUNTIME_CLASS(CDocument)));
	ASSERT(AfxIsValidAddress(pDocClass, sizeof(CRuntimeClass), FALSE));
	
	CDocument *pOldDoc = m_pDocument;
	TRY
	{
		m_pDocument = (CDocument *)pDocClass->CreateObject ();
		if (m_pDocument == NULL)
			AfxThrowMemoryException();
	}
	CATCH_ALL(e)
	{
		m_pDocument = pOldDoc;

		TRACE0("Out of memory creating a document.\n");

		return NULL;
	}
	END_CATCH_ALL

	return pOldDoc;
}

CDocument *CmlViewTab::GetDocument()
{
	return m_pDocument;
}

BOOL CmlViewTab::AddView (LPCTSTR lpszLabel, CRuntimeClass *pViewClass, int iIndex, int iImage)
{
	ASSERT(lpszLabel != NULL);
	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CView)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));

	CView *pView;
	TRY
	{
		pView = (CView*)pViewClass->CreateObject();
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
	context.m_pCurrentDoc = m_pDocument;

	// Create with the right size and position
	if (! pView->Create (NULL, lpszLabel, dwStyle, m_rectView, this, 0, &context))
	{
		TRACE0("Warning: couldn't create client pane for view.\n");
		// pWnd will be cleaned up by PostNcDestroy
		return FALSE;
	}
	pView->SendMessage(WM_INITIALUPDATE, 0, 0);
	
	VTC_ITEM *pMember = new VTC_ITEM;
	pMember->pWnd = pView;
	_tcscpy (pMember->szLabel, lpszLabel);
	
	if (iIndex == -1)
		iIndex = GetItemCount ();
	if (iImage == -1)
		iImage = iIndex;

	TC_ITEM tci;
	tci.mask = TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM;
	tci.pszText = (LPTSTR)(LPCTSTR)lpszLabel;
	tci.iImage = iImage;
	tci.lParam = (LPARAM)pMember;
	InsertItem (iIndex, &tci);

	RelayoutViews ();

	return TRUE;
}

VTC_ITEM *CmlViewTab::GetItemData (int nItem)
{
	ASSERT ((nItem >= 0) && (nItem < GetItemCount ()));
	
	TC_ITEM tci;
	tci.mask = TCIF_PARAM;

	if (! GetItem (nItem, &tci))
		return FALSE;

	return (VTC_ITEM *)(tci.lParam);
}


BOOL CmlViewTab::RelayoutViews()
{

	int nTotal = GetItemCount ();
	if (nTotal < 1)
	{
		InvalidateRect(NULL); // 我们要刷新画面
		return FALSE;
	}
	
	DWORD dwStyle = GetStyle ();
	CRect  rectItem;
	GetClientRect (&m_rectView);
	GetItemRect (GetCurSel (), &rectItem);
	
	if (dwStyle&TCS_BOTTOM)
	{
		if (dwStyle&TCS_VERTICAL)
			m_rectView.right -= rectItem.Width() + VIEWTAB_GAP;
		else
			m_rectView.bottom -= rectItem.Height() + VIEWTAB_GAP;
	}
	else
	{
		if (dwStyle&TCS_VERTICAL)
			m_rectView.left += rectItem.Width() + VIEWTAB_GAP;
		else
			m_rectView.top += rectItem.Height() + VIEWTAB_GAP;
	}
	m_rectView.DeflateRect (VIEWTAB_LEFT_GAP, VIEWTAB_TOP_GAP, VIEWTAB_RIGHT_GAP, VIEWTAB_BOTTOM_GAP);

	while (nTotal > 0)
	{
		VTC_ITEM *pItem = GetItemData (--nTotal);
		ASSERT (pItem != NULL);
		pItem->pWnd->MoveWindow (&m_rectView);
	}
	return TRUE;
}

void CmlViewTab::OnDestroy() 
{

	RemoveAllView();
	CTabCtrl::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
