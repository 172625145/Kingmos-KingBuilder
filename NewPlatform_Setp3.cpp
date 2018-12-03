// NewPlatform_Setp3.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "NewPlatform_Setp3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BSP_GROUP		0
#define BSP_UNSELECTED	1
#define BSP_SELECTED	2

/////////////////////////////////////////////////////////////////////////////
// CNewPlatform_Setp3 property page

IMPLEMENT_DYNCREATE(CNewPlatform_Setp3, CPropertyPage)

CNewPlatform_Setp3::CNewPlatform_Setp3() : CPropertyPage(CNewPlatform_Setp3::IDD)
{
	//{{AFX_DATA_INIT(CNewPlatform_Setp3)
		// NOTE: the ClassWizard will add member initialization here
		m_pImageList = NULL;
	//}}AFX_DATA_INIT
}

CNewPlatform_Setp3::~CNewPlatform_Setp3()
{
	if (m_pImageList)
		delete m_pImageList;
}

void CNewPlatform_Setp3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewPlatform_Setp3)
	DDX_Control(pDX, IDC_FEATURE, m_ctrlFeature);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewPlatform_Setp3, CPropertyPage)
	//{{AFX_MSG_MAP(CNewPlatform_Setp3)
	ON_NOTIFY(NM_CLICK, IDC_FEATURE, OnClickFeature)
	ON_NOTIFY(TVN_SELCHANGED, IDC_FEATURE, OnSelchangedFeature)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewPlatform_Setp3 message handlers

BOOL CNewPlatform_Setp3::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
   CPropertySheet* psheet = (CPropertySheet*) GetParent();   
   psheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH);
	
	return CPropertyPage::OnSetActive();
}

BOOL CNewPlatform_Setp3::OnWizardFinish() 
{
	// TODO: Add your specialized code here and/or call the base class
	return CPropertyPage::OnWizardFinish();
}

BOOL CNewPlatform_Setp3::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
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

/*
	HTREEITEM hDevice,hAP;
	HTREEITEM hItem;
	hDevice = m_ctrlFeature.InsertItem("Device",BSP_GROUP,BSP_GROUP);
	hItem = m_ctrlFeature.InsertItem("AC97",BSP_UNSELECTED,BSP_UNSELECTED,hDevice);
	m_ctrlFeature.SetItemData(hItem,BSP_UNSELECTED);
	hItem = m_ctrlFeature.InsertItem("PCMCIA",BSP_UNSELECTED,BSP_UNSELECTED,hDevice);
	m_ctrlFeature.SetItemData(hItem,BSP_UNSELECTED);
	hItem = m_ctrlFeature.InsertItem("TOUCH",BSP_UNSELECTED,BSP_UNSELECTED,hDevice);
	m_ctrlFeature.SetItemData(hItem,BSP_UNSELECTED);

	hAP = m_ctrlFeature.InsertItem("Application",BSP_GROUP,BSP_GROUP);
	hItem = m_ctrlFeature.InsertItem("notepad",BSP_UNSELECTED,BSP_UNSELECTED,hAP);
	m_ctrlFeature.SetItemData(hItem,BSP_UNSELECTED);
	hItem = m_ctrlFeature.InsertItem("telephone",BSP_UNSELECTED,BSP_UNSELECTED,hAP);
	m_ctrlFeature.SetItemData(hItem,BSP_UNSELECTED);
	hItem = m_ctrlFeature.InsertItem("Iexplorer",BSP_UNSELECTED,BSP_UNSELECTED,hAP);
	m_ctrlFeature.SetItemData(hItem,BSP_UNSELECTED);
*/
	
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

void CNewPlatform_Setp3::OnClickFeature(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CNewPlatform_Setp3::OnSelchangedFeature(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
/*	
	HTREEITEM hItem;
	DWORD dwSelect;

	hItem = m_ctrlFeature.GetSelectedItem();
	dwSelect = m_ctrlFeature.GetItemData(hItem);
	if (dwSelect == BSP_UNSELECTED)
		dwSelect = BSP_SELECTED;
	else
		dwSelect = BSP_UNSELECTED;
	m_ctrlFeature.SetItemData(hItem,dwSelect);
	m_ctrlFeature.SetItemImage(hItem,dwSelect,dwSelect);
*/
	*pResult = 0;
}

void CNewPlatform_Setp3::InsertFeature(LPFEATURESTRUCT lpCurFeature,HTREEITEM hParent)
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
//			hItem = m_ctrlFeature.InsertItem(lpModuleDefine->strModuleName,BSP_UNSELECTED,BSP_UNSELECTED,hFeature);
			hItem = m_ctrlFeature.InsertItem(lpModuleDefine->strModuleName,BSP_SELECTED,BSP_SELECTED,hFeature);
			m_ctrlFeature.SetItemData(hItem,(DWORD)lpModuleDefine);
		}
		iIndex++;
	}
}
