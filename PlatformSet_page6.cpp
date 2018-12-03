// PlatformSet_page6.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "PlatformSet_page6.h"
#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlatformSet_page6 property page

IMPLEMENT_DYNCREATE(CPlatformSet_page6, CPropertyPage)

CPlatformSet_page6::CPlatformSet_page6() : CPropertyPage(CPlatformSet_page6::IDD)
{
	//{{AFX_DATA_INIT(CPlatformSet_page6)
	m_strOutputFileName = _T("");
	m_nCategory = 0;
	m_bOutputMap = FALSE;
	m_strProjectOption = _T("");
	//}}AFX_DATA_INIT
//	m_pctrlDirectoryList = new CDirectoryList;
}

CPlatformSet_page6::~CPlatformSet_page6()
{
//	if (m_pctrlDirectoryList)
//		delete m_pctrlDirectoryList ;
}

void CPlatformSet_page6::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlatformSet_page6)
//	DDX_Control(pDX, IDC_PPP, m_ppp);
	DDX_Text(pDX, IDC_OUTPUTFILENAME, m_strOutputFileName);
	DDX_CBIndex(pDX, IDC_CATEGORYLIST, m_nCategory);
	DDX_Check(pDX, IDC_OUTPUTMAP, m_bOutputMap);
	DDX_Text(pDX, IDC_PROJECTOPTIONS, m_strProjectOption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlatformSet_page6, CPropertyPage)
	//{{AFX_MSG_MAP(CPlatformSet_page6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlatformSet_page6 message handlers

BOOL CPlatformSet_page6::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
/*
	RECT rect ;
	m_ppp.GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_pctrlDirectoryList->Create(NULL,NULL, WS_CHILD|WS_VISIBLE,
					rect,
					this,
					101,NULL);
	m_pctrlDirectoryList->SetTitle("Library or Object Modules:");
	m_pctrlDirectoryList->SetContentType(FILE_LIST);

	CString strLibrary;
	VERIFY(strLibrary.LoadString(IDS_LIBRARY));
	m_pctrlDirectoryList->SetSpecialString(strLibrary);

	CString strLibraryFilter;
	VERIFY(strLibraryFilter.LoadString(IDS_LIBRARYFILTER));
	m_pctrlDirectoryList->SetSpecialFilterString(strLibraryFilter);
*/

	LoadCurrentSetting();
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPlatformSet_page6::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class

	UpdateData(TRUE);
	SetCurrentSetting();
	
	CPropertyPage::OnOK();
}

void CPlatformSet_page6::SetCurrentSetting()
{
	CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

	ASSERT(pMainFrame);
	if (pMainFrame->m_workBar.m_pProjectInfo)
	{
		if (pMainFrame->m_workBar.m_pProjectInfo->IsProjectOpen())
		{
			pMainFrame->m_workBar.m_pProjectInfo->SetOption("LinkOutputFileName", m_strOutputFileName);
			pMainFrame->m_workBar.m_pProjectInfo->SetOption("LinkOutputMap", m_bOutputMap);
			pMainFrame->m_workBar.m_pProjectInfo->SetOption("LinkOptions", m_strProjectOption);
		}
	}
}

void CPlatformSet_page6::LoadCurrentSetting()
{
	CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

	ASSERT(pMainFrame);
	if (pMainFrame->m_workBar.m_pProjectInfo)
	{
		if (pMainFrame->m_workBar.m_pProjectInfo->IsProjectOpen())
		{
			// Will Get Project Option
			pMainFrame->m_workBar.m_pProjectInfo->GetOption("LinkOutputFileName", m_strOutputFileName);
			pMainFrame->m_workBar.m_pProjectInfo->GetOption("LinkOutputMap", m_bOutputMap);
			pMainFrame->m_workBar.m_pProjectInfo->GetOption("LinkOptions", m_strProjectOption);
		}
	}
}
