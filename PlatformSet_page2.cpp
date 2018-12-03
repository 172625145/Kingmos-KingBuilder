// PlatformSet_page2.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "PlatformSet_page2.h"
#include "MainFrame.h"
#include "SetEnv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlatformSet_page2 property page

IMPLEMENT_DYNCREATE(CPlatformSet_page2, CPropertyPage)

CPlatformSet_page2::CPlatformSet_page2() : CPropertyPage(CPlatformSet_page2::IDD)
{
	//{{AFX_DATA_INIT(CPlatformSet_page2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPlatformSet_page2::~CPlatformSet_page2()
{
}

void CPlatformSet_page2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlatformSet_page2)
	DDX_Control(pDX, IDC_ENVIRONMENT, m_ctrlEnvironment);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlatformSet_page2, CPropertyPage)
	//{{AFX_MSG_MAP(CPlatformSet_page2)
	ON_BN_CLICKED(IDC_NEWENV, OnNewenv)
	ON_BN_CLICKED(IDC_EDITENV, OnEditenv)
	ON_BN_CLICKED(IDC_DELENV, OnDelenv)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlatformSet_page2 message handlers

BOOL CPlatformSet_page2::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here

	DWORD dwExStyle = m_ctrlEnvironment.GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT ;
	m_ctrlEnvironment.SetExtendedStyle(dwExStyle);
	m_ctrlEnvironment.InsertColumn(0,"Variable",LVCFMT_LEFT,150);
	m_ctrlEnvironment.InsertColumn(1,"Value",LVCFMT_LEFT,200);

	// 添加已经存在的环境变量
	CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

	if (pMainFrame == NULL)
		return -1;
	if (pMainFrame->m_workBar.m_pWorkSpaceInfo == NULL )
		return -1;

	int iCount = pMainFrame->m_workBar.m_pWorkSpaceInfo->arrEnvironmentVar.GetSize();
	int nIndex;
	LPENVIRONMENTVARIABLE lpCurEnv;

	nIndex = 0;
	while(nIndex < iCount)
	{
		lpCurEnv = (LPENVIRONMENTVARIABLE)pMainFrame->m_workBar.m_pWorkSpaceInfo->arrEnvironmentVar.GetAt(nIndex);
		if (lpCurEnv)
		{
			// 添加该环境到LISTCTRL
			m_ctrlEnvironment.InsertItem(nIndex,lpCurEnv->strVariable);
			m_ctrlEnvironment.SetItemText(nIndex,1,lpCurEnv->strValue);
		}
		nIndex ++;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPlatformSet_page2::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	CString strVariable,strValue;

	UpdateData(TRUE);
	CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

	if (pMainFrame == NULL)
		return;
	if (pMainFrame->m_workBar.m_pWorkSpaceInfo == NULL )
			return ;

	pMainFrame->m_workBar.m_pWorkSpaceInfo->ClearEnvionment();
	int iCount = m_ctrlEnvironment.GetItemCount();
	int nIndex;

	nIndex = 0;
	while(nIndex < iCount)
	{
		strVariable = m_ctrlEnvironment.GetItemText( nIndex, 0 ) ;
		strValue = m_ctrlEnvironment.GetItemText( nIndex, 1 ) ;
		pMainFrame->m_workBar.m_pWorkSpaceInfo->SetEnvironment(strVariable,strValue);
		nIndex ++;
	}	
	
	CPropertyPage::OnOK();
}

void CPlatformSet_page2::OnNewenv() 
{
	// TODO: Add your control notification handler code here
	CSetEnv dlgSetEnv;
	int nIndex = m_ctrlEnvironment.GetItemCount();

		if (dlgSetEnv.DoModal() == IDOK)
		{
			nIndex = m_ctrlEnvironment.InsertItem(nIndex,dlgSetEnv.m_strVariable);
			m_ctrlEnvironment.SetItemText(nIndex,1,dlgSetEnv.m_strValue);
		}
}

void CPlatformSet_page2::OnEditenv() 
{
	// TODO: Add your control notification handler code here
	int iSelIndex;
	
		iSelIndex = m_ctrlEnvironment.GetSelectionMark();
		if (iSelIndex == -1)
			return ;

		CSetEnv dlgSetEnv;

		dlgSetEnv.m_strVariable = m_ctrlEnvironment.GetItemText( iSelIndex, 0 ) ;
		dlgSetEnv.m_strValue = m_ctrlEnvironment.GetItemText( iSelIndex, 1 ) ;
		if (dlgSetEnv.DoModal() == IDOK)
		{
//			iSelIndex = m_ctrlEnvironment.InsertItem(iSelIndex,dlgSetEnv.m_strVariable);
			m_ctrlEnvironment.SetItemText(iSelIndex,0,dlgSetEnv.m_strVariable);
			m_ctrlEnvironment.SetItemText(iSelIndex,1,dlgSetEnv.m_strValue);
		}
      
}

void CPlatformSet_page2::OnDelenv() 
{
	// TODO: Add your control notification handler code here
	int iSelIndex;
	
		iSelIndex = m_ctrlEnvironment.GetSelectionMark();
		if (iSelIndex == -1)
			return ;
		m_ctrlEnvironment.DeleteItem(iSelIndex);
}
