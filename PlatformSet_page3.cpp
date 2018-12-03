// PlatformSet_page3.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "PlatformSet_page3.h"
#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlatformSet_page3 property page

IMPLEMENT_DYNCREATE(CPlatformSet_page3, CPropertyPage)

CPlatformSet_page3::CPlatformSet_page3() : CPropertyPage(CPlatformSet_page3::IDD)
{
	//{{AFX_DATA_INIT(CPlatformSet_page3)
	m_nSel = -1;
	m_nCurSel = -1;
	//}}AFX_DATA_INIT
	m_pctrlDirectoryList = new CDirectoryList;
}

CPlatformSet_page3::~CPlatformSet_page3()
{
	if (m_pctrlDirectoryList)
		delete m_pctrlDirectoryList ;
}

void CPlatformSet_page3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlatformSet_page3)
	DDX_Control(pDX, IDC_PPP, m_ppp);
	DDX_CBIndex(pDX, IDC_SELECTDIRFOR, m_nSel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlatformSet_page3, CPropertyPage)
	//{{AFX_MSG_MAP(CPlatformSet_page3)
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_SELECTDIRFOR, OnSelchangeSelectdirfor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlatformSet_page3 message handlers



int CPlatformSet_page3::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
//	CEdit *pWnd= (CEdit *)GetDlgItem(IDC_PPP);
//	m_ppp.GetClientRect(&rect);
//	m_pctrlDirectoryList->Create(NULL,NULL, WS_CHILD|WS_VISIBLE,
//					rect,
//					this,
//					101,NULL);
//	return CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID, pContext);
	// TODO: Add your specialized creation code here
	return 0;
}

BOOL CPlatformSet_page3::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	// TODO: Add extra initialization here

	RECT rect = {5,50,245,100};
	m_ppp.GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_pctrlDirectoryList->Create(NULL,NULL, WS_CHILD|WS_VISIBLE,
					rect,
					this,
					101,NULL);

	m_nSel = 0;
	m_nCurSel = 0;
	GetDirListFromProject();
	InsertDirList();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPlatformSet_page3::InsertDirList()
{
	m_pctrlDirectoryList->m_pListBox->ResetContent();// 清除列表框中的内容
	CStringArray *pDirList;
	if (m_nCurSel == -1)
		return;
	if (m_nCurSel == 0)
		pDirList = &m_arrIncludeDirList;  //存放当前设定的INCLUDE目录
	else
		pDirList = &m_arrLibraryDirList;  //存放当前设定的LIBRARY目录
	int iCount = pDirList->GetSize();
	for (int i = 0; i < iCount ; i++)
		m_pctrlDirectoryList->m_pListBox->AddString(pDirList->GetAt(i));

}



void CPlatformSet_page3::GetDirListFromProject()
{
	//从当前已经打开的工程中得到当前已经设定的路径
	CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

	if (pMainFrame == NULL)
		return ;

	if (pMainFrame->m_workBar.m_pWorkSpaceInfo)
	{
		if (pMainFrame->m_workBar.m_pWorkSpaceInfo->IsWorkSpaceOpen())
		{
			//m_arrIncludeDirList = pMainFrame->m_workBar.m_pWorkSpaceInfo->m_arrIncludeDirList;
			CopyStringArray(m_arrIncludeDirList,pMainFrame->m_workBar.m_pWorkSpaceInfo->m_arrIncludeDirList);
			//m_arrLibraryDirList = pMainFrame->m_workBar.m_pWorkSpaceInfo->m_arrLibraryDirList;
			CopyStringArray(m_arrLibraryDirList,pMainFrame->m_workBar.m_pWorkSpaceInfo->m_arrLibraryDirList);
		}
	}
	if (pMainFrame->m_workBar.m_pProjectInfo)
	{
		if (pMainFrame->m_workBar.m_pProjectInfo->IsProjectOpen())
		{
//			m_arrIncludeDirList = pMainFrame->m_workBar.m_pProjectInfo->m_arrIncludeDirList;
			CopyStringArray(m_arrIncludeDirList ,pMainFrame->m_workBar.m_pProjectInfo->m_arrIncludeDirList);
//			m_arrIncludeDirList = pMainFrame->m_workBar.m_pProjectInfo->m_arrLibraryDirList;
			CopyStringArray(m_arrLibraryDirList,pMainFrame->m_workBar.m_pProjectInfo->m_arrLibraryDirList);
		}
	}
}

void CPlatformSet_page3::SetDirListToProject()
{
	//将当前新设定的路径设定到当前已经打开的工程中去
	CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

	if (pMainFrame == NULL)
		return ;

	if (pMainFrame->m_workBar.m_pWorkSpaceInfo)
	{
		if (pMainFrame->m_workBar.m_pWorkSpaceInfo->IsWorkSpaceOpen())
		{
//			 pMainFrame->m_workBar.m_pWorkSpaceInfo->m_arrIncludeDirList = m_arrIncludeDirList;
			 CopyStringArray(pMainFrame->m_workBar.m_pWorkSpaceInfo->m_arrIncludeDirList,m_arrIncludeDirList);
//			 pMainFrame->m_workBar.m_pWorkSpaceInfo->m_arrLibraryDirList = m_arrLibraryDirList;
			 CopyStringArray(pMainFrame->m_workBar.m_pWorkSpaceInfo->m_arrLibraryDirList,m_arrLibraryDirList);
		}
	}
	if (pMainFrame->m_workBar.m_pProjectInfo)
	{
		if (pMainFrame->m_workBar.m_pProjectInfo->IsProjectOpen())
		{
//			 pMainFrame->m_workBar.m_pProjectInfo->m_arrIncludeDirList = m_arrIncludeDirList;
			 CopyStringArray(pMainFrame->m_workBar.m_pProjectInfo->m_arrIncludeDirList,m_arrIncludeDirList);
//			 pMainFrame->m_workBar.m_pProjectInfo->m_arrLibraryDirList = m_arrIncludeDirList;
			 CopyStringArray(pMainFrame->m_workBar.m_pProjectInfo->m_arrLibraryDirList,m_arrLibraryDirList);
			 pMainFrame->m_workBar.m_pProjectInfo->RefreshIncludePath();
			 pMainFrame->m_workBar.m_pProjectInfo->RefreshLibraryPath();
		}
	}
}

void CPlatformSet_page3::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
	GetListBoxContext();
	SetDirListToProject();
	CPropertyPage::OnOK();
}



void CPlatformSet_page3::CopyStringArray(CStringArray &arrObj, CStringArray &arrSrc)
{
	arrObj.RemoveAll(); // 首先将目的数组清空
	int iCount = arrSrc.GetSize();

	for (int i = 0; i < iCount; i++)
	{
		arrObj.Add(arrSrc.GetAt(i));
	}
}

void CPlatformSet_page3::OnSelchangeSelectdirfor() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);
	GetListBoxContext();
	m_nCurSel = m_nSel;
	InsertDirList();
}

void CPlatformSet_page3::GetListBoxContext()
{
	CStringArray *pDirList;
	if (m_nCurSel == -1)
		return;
	if (m_nCurSel == 0)
		pDirList = &m_arrIncludeDirList;  //存放当前设定的INCLUDE目录
	else
		pDirList = &m_arrLibraryDirList;  //存放当前设定的LIBRARY目录
	pDirList->RemoveAll(); //清除数组中的内容
	int iCount = m_pctrlDirectoryList->m_pListBox->GetCount(); // 得到数据个数
	CString strPath;
	for (int i = 0; i < iCount ; i++)
	{
		m_pctrlDirectoryList->m_pListBox->GetText(i,strPath);
		pDirList->Add(strPath);
	}
}
