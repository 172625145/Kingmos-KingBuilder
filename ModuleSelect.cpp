// ModuleSelect.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "ModuleSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModuleSelect dialog
#define BSP_GROUP		0
#define BSP_UNSELECTED	1
#define BSP_SELECTED	2


CModuleSelect::CModuleSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CModuleSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModuleSelect)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pImageList = NULL;
}


void CModuleSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModuleSelect)
	DDX_Control(pDX, IDC_FEATURE, m_ctrlFeature);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModuleSelect, CDialog)
	//{{AFX_MSG_MAP(CModuleSelect)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_FEATURE, OnClickFeature)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModuleSelect message handlers

BOOL CModuleSelect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pImageList = new CImageList;
		
	m_pImageList->Create(16,16,ILC_COLOR8|ILC_MASK,16,16);

	HICON hIcon;

	hIcon = AfxGetApp()->LoadIcon(IDI_GROUP);
	m_pImageList->Add(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDI_UNSELECTED);
	m_pImageList->Add(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDI_SELECTED);
	m_pImageList->Add(hIcon);
	
	m_ctrlFeature.SetImageList(m_pImageList, TVSIL_NORMAL);
	
	LPFEATURESTRUCT lpCurFeature;
	CString strKBuilder;

	((CAmboApp *)AfxGetApp())->GetKingbuilderRoot(strKBuilder);

	strKBuilder += "\\script\\Module.";
	m_ModuleRead.LoadModule(strKBuilder);
	lpCurFeature = m_ModuleRead.m_root->child;
	if (lpCurFeature)
		InsertFeature(lpCurFeature);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CModuleSelect::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_pImageList)
		delete m_pImageList;
	
}

void CModuleSelect::InsertFeature(LPFEATURESTRUCT lpCurFeature, HTREEITEM hParent)
{
	HTREEITEM hFeature,hItem;
	hFeature = m_ctrlFeature.InsertItem(lpCurFeature->strFeatureName,BSP_GROUP,BSP_GROUP,hParent);
	if (lpCurFeature->next)
		InsertFeature(lpCurFeature->next,hParent);
	if (lpCurFeature->child)
		InsertFeature(lpCurFeature->child,hFeature);

	// 插入他的模块
	LPDEFMODULE lpModuleDefine;
	int iSize,iIndex;
	iSize = lpCurFeature->moduleArray.GetSize();
	iIndex = 0;
	while(iIndex<iSize)
	{
		lpModuleDefine = (LPDEFMODULE)lpCurFeature->moduleArray.GetAt(iIndex);
		if (lpModuleDefine)
		{
			int iSelected;
//			hItem = m_ctrlFeature.InsertItem(lpModuleDefine->strModuleName,BSP_UNSELECTED,BSP_UNSELECTED,hFeature);
			if (((CAmboApp *)AfxGetApp())->GetModuleValue(lpModuleDefine->lpModuleVar->VarName) == 1)
			{
				// had Select The Module
				lpModuleDefine->lpModuleVar->iValue = 1;
				iSelected = BSP_SELECTED;
			}
			else
			{
				lpModuleDefine->lpModuleVar->iValue = 0;
				iSelected = BSP_UNSELECTED;
			}
			hItem = m_ctrlFeature.InsertItem(lpModuleDefine->strModuleName,iSelected,iSelected,hFeature);
			m_ctrlFeature.SetItemData(hItem,(DWORD)lpModuleDefine);
		}
		iIndex++;
	}
}

void CModuleSelect::OnClickFeature(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM hItem;
	DWORD dwSelect;
	POINT pt;

	GetCursorPos(&pt);

	m_ctrlFeature.ScreenToClient(&pt);
	UINT uFlags ;
	hItem = m_ctrlFeature.HitTest(pt, &uFlags );
	if (hItem == NULL)
		return ;
//	hItem = m_ctrlFeature.GetSelectedItem();
	LPDEFMODULE lpModuleDefine;

	lpModuleDefine = (LPDEFMODULE)m_ctrlFeature.GetItemData(hItem);
	if (lpModuleDefine == 0)
		return ;
	if (lpModuleDefine->lpModuleVar->iValue == 0)
	{
		lpModuleDefine->lpModuleVar->iValue = 1;
		dwSelect = BSP_SELECTED;
	}
	else
	{
		lpModuleDefine->lpModuleVar->iValue = 0;
		dwSelect = BSP_UNSELECTED;
	}
//	m_ctrlFeature.SetItemData(hItem,dwSelect);
	m_ctrlFeature.SetItemImage(hItem,dwSelect,dwSelect);
	
	*pResult = 0;
}

void CModuleSelect::OnOK() 
{
	// TODO: Add extra validation here
	CString strModuleSetFile;
	if (((CAmboApp *)AfxGetApp())->GetWorkSpaceRoot(strModuleSetFile) == FALSE)
		return ;
	strModuleSetFile += "\\script\\";
	strModuleSetFile += MODULESETFILE;
	m_ModuleRead.SaveCurModuleState(strModuleSetFile);
	
	((CAmboApp *)AfxGetApp())->RefreshModuleSet();
	CDialog::OnOK();
}

