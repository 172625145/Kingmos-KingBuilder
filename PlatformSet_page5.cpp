// PlatformSet_page5.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "PlatformSet_page5.h"
#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlatformSet_page5 property page

IMPLEMENT_DYNCREATE(CPlatformSet_page5, CPropertyPage)

CPlatformSet_page5::CPlatformSet_page5() : CPropertyPage(CPlatformSet_page5::IDD)
{
	//{{AFX_DATA_INIT(CPlatformSet_page5)
	m_iGategory = 0;
	m_strPreProcessor = _T("");
	m_strProjectOptions = _T("");
	m_nWarningLevel = 0;
	//}}AFX_DATA_INIT
}

CPlatformSet_page5::~CPlatformSet_page5()
{
}

void CPlatformSet_page5::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlatformSet_page5)
	DDX_CBIndex(pDX, IDC_CATEGORYLIST, m_iGategory);
	DDX_Text(pDX, IDC_PREPROCESSOR, m_strPreProcessor);
	DDX_Text(pDX, IDC_PROJECTOPTIONS, m_strProjectOptions);
	DDX_CBIndex(pDX, IDC_WARNINGLEVELLIST, m_nWarningLevel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlatformSet_page5, CPropertyPage)
	//{{AFX_MSG_MAP(CPlatformSet_page5)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlatformSet_page5 message handlers

void CPlatformSet_page5::LoadCurrentSetting()
{

	CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

	ASSERT(pMainFrame);
	if (pMainFrame->m_workBar.m_pWorkSpaceInfo)
	{
		if (pMainFrame->m_workBar.m_pWorkSpaceInfo->IsWorkSpaceOpen())
		{
			// Will Get WorkSpace Option
			pMainFrame->m_workBar.m_pWorkSpaceInfo->GetOption("AssemberWarnLevel", m_nWarningLevel);
			pMainFrame->m_workBar.m_pWorkSpaceInfo->GetOption("AssemberPredefines", m_strPreProcessor);
			pMainFrame->m_workBar.m_pWorkSpaceInfo->GetOption("AsseberOptions", m_strProjectOptions);
		}
	}
	if (pMainFrame->m_workBar.m_pProjectInfo)
	{
		if (pMainFrame->m_workBar.m_pProjectInfo->IsProjectOpen())
		{
			// Will Get Project Option
			pMainFrame->m_workBar.m_pProjectInfo->GetOption("AssemberWarnLevel", m_nWarningLevel);
			pMainFrame->m_workBar.m_pProjectInfo->GetOption("AssemberPredefines", m_strPreProcessor);
			pMainFrame->m_workBar.m_pProjectInfo->GetOption("AsseberOptions", m_strProjectOptions);
		}
	}
}

void CPlatformSet_page5::SetCurrentSetting()
{
	CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

	ASSERT(pMainFrame);
	if (pMainFrame->m_workBar.m_pWorkSpaceInfo)
	{
		if (pMainFrame->m_workBar.m_pWorkSpaceInfo->IsWorkSpaceOpen())
		{
			// Will Set WorkSpace Option
			pMainFrame->m_workBar.m_pWorkSpaceInfo->SetOption("AssemberWarnLevel", m_nWarningLevel);
			pMainFrame->m_workBar.m_pWorkSpaceInfo->SetOption("AssemberPredefines", m_strPreProcessor);
			pMainFrame->m_workBar.m_pWorkSpaceInfo->SetOption("AsseberOptions", m_strProjectOptions);
		}
	}
	if (pMainFrame->m_workBar.m_pProjectInfo)
	{
		if (pMainFrame->m_workBar.m_pProjectInfo->IsProjectOpen())
		{
			pMainFrame->m_workBar.m_pProjectInfo->SetOption("AssemberWarnLevel", m_nWarningLevel);
			pMainFrame->m_workBar.m_pProjectInfo->SetOption("AssemberPredefines", m_strPreProcessor);
			pMainFrame->m_workBar.m_pProjectInfo->SetOption("AsseberOptions", m_strProjectOptions);
		}
	}
}

void CPlatformSet_page5::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
	SetCurrentSetting();	
	
	CPropertyPage::OnOK();
}

BOOL CPlatformSet_page5::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	LoadCurrentSetting();
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
