// CommandBar.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
//#include "OSDevEx.h"
#include "CommandBar.h"
#include "MainFrame.h"
#include "SpecialView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommandBar
//#define COMBOBOX_INDEX 2
//#define COMBOBOX_WIDTH 100
#define COMBOBOX_HEIGHT 150
#define COMMANDBAR_HEIGHT 32

CCommandBar::CCommandBar()
{
	m_ComboList = NULL;
}

CCommandBar::~CCommandBar()
{
}

BEGIN_MESSAGE_MAP(CCommandBar, CToolBar)
	//{{AFX_MSG_MAP(CCommandBar)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommandBar message handlers

BOOL CCommandBar::CreateEx(CWnd *pParent, DWORD dwCtrlStyle ,DWORD dwStyle,UINT nID)
{
	// start out with no borders
	CRect rcBorders(0,0,0,0);

//	DWORD dwStyle = TBSTYLE_FLAT|CBRS_TOP|WS_VISIBLE|TBSTYLE_AUTOSIZE;
//	dwStyle |= (CBRS_TOOLTIPS | CBRS_FLYBY);

	if (! CToolBar::CreateEx (pParent, dwCtrlStyle,dwStyle,rcBorders,nID))
	{
		return FALSE;
	}
/*
	CRect rect(-COMBOBOX_WIDTH, -COMBOBOX_HEIGHT, 0, 0);
	// The ID of the ComboBox is important for two reasons.  One, so you
	// can receive notifications from the control.  And also for ToolTips.
	// During HitTesting if the ToolBar sees that the mouse is one a child
	// control, the toolbar will lookup the controls ID and search for a
	// string in the string table with the same ID to use for ToolTips
	// and StatusBar info.
	if (!m_Search.Create(WS_CHILD | CBS_DROPDOWN |
		CBS_AUTOHSCROLL | WS_VSCROLL | CBS_HASSTRINGS, rect, this,
		ID_COMMANDBAR_SEARCH))
	{
		return FALSE;
	}

	HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	if (hFont == NULL)
		hFont = (HFONT)GetStockObject(ANSI_VAR_FONT);

	m_Search.SendMessage(WM_SETFONT, (WPARAM)hFont);

	SetButtonInfo (COMBOBOX_INDEX, ID_COMMANDBAR_SEARCH, TBBS_SEPARATOR, COMBOBOX_WIDTH);


	if (m_Search.m_hWnd != NULL)
	{
		CRect rect;
		GetItemRect(COMBOBOX_INDEX, rect);

		m_Search.SetWindowPos(NULL, rect.left, rect.top, 0, 0, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOCOPYBITS);
		m_Search.ShowWindow(SW_SHOW);
	}
*/
	SetHeight(COMMANDBAR_HEIGHT);
	return TRUE;
}
BOOL CCommandBar::AddComboBox(UINT uID,int iWidth,DWORD dwStyle)
{
	CRect rect(-iWidth, -COMBOBOX_HEIGHT, 0, 0);
	CComboBox *pComboBox;
	LPCOMBOBOXLIST pListItem;
	
	pComboBox = new CComboBox;
	pListItem = new COMBOBOXLIST;

	dwStyle |=WS_CHILD;

	// The ID of the ComboBox is important for two reasons.  One, so you
	// can receive notifications from the control.  And also for ToolTips.
	// During HitTesting if the ToolBar sees that the mouse is one a child
	// control, the toolbar will lookup the controls ID and search for a
	// string in the string table with the same ID to use for ToolTips
	// and StatusBar info.
	if (!pComboBox->Create(dwStyle, rect, this,
		uID))
	{
		return FALSE;
	}

//	HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	HFONT hFont = (HFONT)GetStockObject(SYSTEM_FONT);
	
	if (hFont == NULL)
		hFont = (HFONT)GetStockObject(ANSI_VAR_FONT);

	pComboBox->SendMessage(WM_SETFONT, (WPARAM)hFont);

	INT iIndex = CommandToIndex( uID);
	SetButtonInfo (iIndex, uID, TBBS_SEPARATOR, iWidth);


	if (pComboBox->m_hWnd != NULL)
	{
		CRect rect;
		GetItemRect(iIndex, rect);

		pComboBox->SetWindowPos(NULL, rect.left, rect.top, 0, 0, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOCOPYBITS);
		pComboBox->ShowWindow(SW_SHOW);
	}
	pListItem->pComboBox = pComboBox;
	pListItem->uID = uID;
	pListItem->pNext = NULL;

	if (m_ComboList == NULL)
	{
		m_ComboList = pListItem;
	}
	else
	{
		LPCOMBOBOXLIST pTemp = m_ComboList;
		while(pTemp->pNext)
		{
			pTemp = pTemp->pNext;
		}
		pTemp->pNext = pListItem;
	}
	return TRUE;
}

CComboBox * CCommandBar::GetComboBox(UINT uID)
{
	LPCOMBOBOXLIST pTemp = m_ComboList;
	while(pTemp)
	{
		if (pTemp->uID == uID)
			return pTemp->pComboBox;
		pTemp = pTemp->pNext;
	}
	return NULL;
}

UINT CCommandBar::GetComboBoxID(CWnd *pItem)
{
	LPCOMBOBOXLIST pTemp = m_ComboList;
	while(pTemp)
	{
		if (pTemp->pComboBox == pItem)
			return pTemp->uID;
		if (::IsChild(pTemp->pComboBox->m_hWnd,pItem->m_hWnd))
			return pTemp->uID;
		pTemp = pTemp->pNext;
	}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CCommandBar message handlers
BOOL CCommandBar::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (LOWORD(wParam) == IDOK)
	{
		CWnd *pItem;
		UINT id;
		pItem = GetFocus();
		id = GetComboBoxID(pItem);
		if (id>0)
		{
			//ON_COMMAND(id,0);
			::SendMessage(GetParent()->m_hWnd,WM_COMMAND, MAKEWPARAM(id,IDOK),0);
			return TRUE;
		}
	}
	if (LOWORD(wParam) == IDCANCEL)
	{
		// 将焦点设置到编辑窗口
//		CMainFrame* pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
//		CView* pView = (CView*)pMainFrame->GetActiveFrame()->GetActiveView();
//		if (pView)
//			pView->SetFocus();
		CWnd *pItem;
		UINT id;
		pItem = GetFocus();
		id = GetComboBoxID(pItem);
		if (id>0)
		{
			//ON_COMMAND(id,0);
			::SendMessage(GetParent()->m_hWnd,WM_COMMAND, MAKEWPARAM(id,IDCANCEL),0);
			return TRUE;
		}
	}
	return FALSE;
}

void CCommandBar::OnDestroy() 
{
	LPCOMBOBOXLIST pTemp ;
	while(m_ComboList)
	{
		pTemp = m_ComboList;
		m_ComboList = m_ComboList->pNext;
		if (pTemp->pComboBox)
			delete pTemp->pComboBox;
		delete pTemp;
	}
	CToolBar::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
