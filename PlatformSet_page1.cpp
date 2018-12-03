// PlatformSet_page1.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "PlatformSet_page1.h"
#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlatformSet_page1 property page

IMPLEMENT_DYNCREATE(CPlatformSet_page1, CPropertyPage)

CPlatformSet_page1::CPlatformSet_page1() : CPropertyPage(CPlatformSet_page1::IDD)
{
	//{{AFX_DATA_INIT(CPlatformSet_page1)
	m_strBuildType = _T("");
	//}}AFX_DATA_INIT
}

CPlatformSet_page1::~CPlatformSet_page1()
{
}

void CPlatformSet_page1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlatformSet_page1)
	DDX_Control(pDX, IDC_BUILDTYPE, m_ctrlBuildType);
	DDX_CBString(pDX, IDC_BUILDTYPE, m_strBuildType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlatformSet_page1, CPropertyPage)
	//{{AFX_MSG_MAP(CPlatformSet_page1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlatformSet_page1 message handlers

BOOL CPlatformSet_page1::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CString strVariable,strValue;
	// TODO: Add extra initialization here
	CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

	m_ctrlBuildType.SetCurSel(0);
	if (pMainFrame == NULL)
		return FALSE;

	strVariable = "BUILDTYPE" ;
	if (pMainFrame->m_workBar.m_pWorkSpaceInfo)
	{
		if (pMainFrame->m_workBar.m_pWorkSpaceInfo->IsWorkSpaceOpen())
		{
			pMainFrame->m_workBar.m_pWorkSpaceInfo->GetOption(strVariable,strValue);
		}
	}
	if (pMainFrame->m_workBar.m_pProjectInfo)
	{
		if (pMainFrame->m_workBar.m_pProjectInfo->IsProjectOpen())
		{
			pMainFrame->m_workBar.m_pProjectInfo->GetOption(strVariable,strValue);
		}
	}

	if (strValue.IsEmpty() == FALSE)
	{
		m_strBuildType = strValue;
	}
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPlatformSet_page1::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	CString strVariable,strValue;

	UpdateData(TRUE);
	CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

	if (pMainFrame == NULL)
		return;
//	if (pMainFrame->m_workBar.m_pWorkSpaceInfo == NULL )
//			return ;

	strVariable = "BUILDTYPE" ;
	strValue = m_strBuildType;
//	pMainFrame->m_workBar.m_pWorkSpaceInfo->SetOption(strVariable,strValue);
	if (pMainFrame->m_workBar.m_pWorkSpaceInfo)
	{
		if (pMainFrame->m_workBar.m_pWorkSpaceInfo->IsWorkSpaceOpen())
		{
			pMainFrame->m_workBar.m_pWorkSpaceInfo->SetOption(strVariable,strValue);
		}
	}
	if (pMainFrame->m_workBar.m_pProjectInfo)
	{
		if (pMainFrame->m_workBar.m_pProjectInfo->IsProjectOpen())
		{
			pMainFrame->m_workBar.m_pProjectInfo->SetOption(strVariable,strValue);
		}
	}
	
	
	CPropertyPage::OnOK();
}
