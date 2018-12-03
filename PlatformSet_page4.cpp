// PlatformSet_page4.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "PlatformSet_page4.h"
#include "Mainframe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlatformSet_page4 property page

IMPLEMENT_DYNCREATE(CPlatformSet_page4, CPropertyPage)

CPlatformSet_page4::CPlatformSet_page4() : CPropertyPage(CPlatformSet_page4::IDD)
{
	//{{AFX_DATA_INIT(CPlatformSet_page4)
	m_strPreProcessor = "";
	m_ProjectOptions = "";
	m_nWarningLevel = 0;
	m_nOptimizations = 0;
	m_nCategory = 0;
	//}}AFX_DATA_INIT
}

CPlatformSet_page4::~CPlatformSet_page4()
{
}

void CPlatformSet_page4::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlatformSet_page4)
	DDX_Text(pDX, IDC_PREPROCESSOR, m_strPreProcessor);
	DDX_Text(pDX, IDC_PROJECTOPTIONS, m_ProjectOptions);
	DDX_CBIndex(pDX, IDC_WARNINGLEVELLIST, m_nWarningLevel);
	DDX_CBIndex(pDX, IDC_OPTIMIZATIONSLIST, m_nOptimizations);
	DDX_CBIndex(pDX, IDC_CATEGORYLIST, m_nCategory);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlatformSet_page4, CPropertyPage)
	//{{AFX_MSG_MAP(CPlatformSet_page4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlatformSet_page4 message handlers

BOOL CPlatformSet_page4::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here

	LoadCurrentSetting();
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPlatformSet_page4::LoadCurrentSetting()
{
	CString strOptimization;

	CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

	ASSERT(pMainFrame);
	if (pMainFrame->m_workBar.m_pWorkSpaceInfo)
	{
		if (pMainFrame->m_workBar.m_pWorkSpaceInfo->IsWorkSpaceOpen())
		{
			// Will Get WorkSpace Option
			pMainFrame->m_workBar.m_pWorkSpaceInfo->GetOption("CompileWarnLevel", m_nWarningLevel);
			pMainFrame->m_workBar.m_pWorkSpaceInfo->GetOption("CompileOptimization", strOptimization);
			pMainFrame->m_workBar.m_pWorkSpaceInfo->GetOption("CompilePredefines", m_strPreProcessor);
			pMainFrame->m_workBar.m_pWorkSpaceInfo->GetOption("CompileOptions", m_ProjectOptions);
		}
	}
	if (pMainFrame->m_workBar.m_pProjectInfo)
	{
		if (pMainFrame->m_workBar.m_pProjectInfo->IsProjectOpen())
		{
			// Will Get Project Option
			pMainFrame->m_workBar.m_pProjectInfo->GetOption("CompileWarnLevel", m_nWarningLevel);
			pMainFrame->m_workBar.m_pProjectInfo->GetOption("CompileOptimization", strOptimization);
			pMainFrame->m_workBar.m_pProjectInfo->GetOption("CompilePredefines", m_strPreProcessor);
			pMainFrame->m_workBar.m_pProjectInfo->GetOption("CompileOptions", m_ProjectOptions);
		}
	}

	if (strOptimization.CompareNoCase("default") == 0)
	{
		m_nOptimizations = 0;
	}
	else if (strOptimization.CompareNoCase("Disable") == 0)
	{
		m_nOptimizations = 1;
	}
	else if (strOptimization.CompareNoCase("Maximize Speed") == 0)
	{
		m_nOptimizations = 2;
	}
	else if (strOptimization.CompareNoCase("Minimize Size") == 0)
	{
		m_nOptimizations = 3;
	}
	else
	{
		m_nOptimizations = -1;
	}
}

void CPlatformSet_page4::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
	SetCurrentSetting();	
	CPropertyPage::OnOK();
}

void CPlatformSet_page4::SetCurrentSetting()
{
	CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

	ASSERT(pMainFrame);
	if (pMainFrame->m_workBar.m_pWorkSpaceInfo)
	{
		if (pMainFrame->m_workBar.m_pWorkSpaceInfo->IsWorkSpaceOpen())
		{
			// Will Set WorkSpace Option
			pMainFrame->m_workBar.m_pWorkSpaceInfo->SetOption("CompileWarnLevel", m_nWarningLevel);
			switch(m_nOptimizations)
			{
				case 0:
					pMainFrame->m_workBar.m_pWorkSpaceInfo->SetOption("CompileOptimization", "default");
					break;
				case 1:
					pMainFrame->m_workBar.m_pWorkSpaceInfo->SetOption("CompileOptimization", "Disable");
					break;
				case 2:
					pMainFrame->m_workBar.m_pWorkSpaceInfo->SetOption("CompileOptimization", "Maximize Speed");
					break;
				case 3:
					pMainFrame->m_workBar.m_pWorkSpaceInfo->SetOption("CompileOptimization", "Minimize Size");
					break;
			}
			pMainFrame->m_workBar.m_pWorkSpaceInfo->SetOption("CompilePredefines", m_strPreProcessor);
			pMainFrame->m_workBar.m_pWorkSpaceInfo->SetOption("CompileOptions", m_ProjectOptions);
		}
	}
	if (pMainFrame->m_workBar.m_pProjectInfo)
	{
		if (pMainFrame->m_workBar.m_pProjectInfo->IsProjectOpen())
		{
			pMainFrame->m_workBar.m_pProjectInfo->SetOption("CompileWarnLevel", m_nWarningLevel);
			switch(m_nOptimizations)
			{
				case 0:
					pMainFrame->m_workBar.m_pProjectInfo->SetOption("CompileOptimization", "default");
					break;
				case 1:
					pMainFrame->m_workBar.m_pProjectInfo->SetOption("CompileOptimization", "Disable");
					break;
				case 2:
					pMainFrame->m_workBar.m_pProjectInfo->SetOption("CompileOptimization", "Maximize Speed");
					break;
				case 3:
					pMainFrame->m_workBar.m_pProjectInfo->SetOption("CompileOptimization", "Minimize Size");
					break;
			}
			pMainFrame->m_workBar.m_pProjectInfo->SetOption("CompilePredefines", m_strPreProcessor);
			pMainFrame->m_workBar.m_pProjectInfo->SetOption("CompileOptions", m_ProjectOptions);
		}
	}
}
