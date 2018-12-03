// FindInFiles.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "FindInFiles.h"
#include "shlobj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFindInFiles dialog


CFindInFiles::CFindInFiles(CWnd* pParent /*=NULL*/)
	: CDialog(CFindInFiles::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindInFiles)
	m_strFindFilter = _T("");
	m_FindFolder = _T("");
	m_strFindText = _T("");
	m_bLookInSubFolder = TRUE;
	m_bCaseMatch = FALSE;
	m_bMatchWholeWord = FALSE;
	//}}AFX_DATA_INIT
}


void CFindInFiles::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindInFiles)
	DDX_Control(pDX, IDC_FIND, m_ctrlFind);
	DDX_Control(pDX, IDC_FINDFOLDER, m_ctrlFindFolder);
	DDX_Control(pDX, IDC_FINDFILTER, m_ctrlFindFilter);
	DDX_Control(pDX, IDC_FINDTEXTCOMBO, m_ctrlFindText);
	DDX_CBString(pDX, IDC_FINDFILTER, m_strFindFilter);
	DDX_CBString(pDX, IDC_FINDFOLDER, m_FindFolder);
	DDX_CBString(pDX, IDC_FINDTEXTCOMBO, m_strFindText);
	DDX_Check(pDX, IDC_LOOKINSUBFOLDER, m_bLookInSubFolder);
	DDX_Check(pDX, IDC_MATCH_CASE, m_bCaseMatch);
	DDX_Check(pDX, IDC_MATCHWHOLEWORD, m_bMatchWholeWord);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFindInFiles, CDialog)
	//{{AFX_MSG_MAP(CFindInFiles)
	ON_BN_CLICKED(IDC_FIND, OnFind)
	ON_CBN_EDITCHANGE(IDC_FINDFILTER, OnEditchangeFindfilter)
	ON_CBN_EDITCHANGE(IDC_FINDFOLDER, OnEditchangeFindfolder)
	ON_CBN_EDITCHANGE(IDC_FINDTEXTCOMBO, OnEditchangeFindtextcombo)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindInFiles message handlers

BOOL CFindInFiles::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ctrlFindFilter.AddString("*.cpp;*.c;*.hpp;*.h");
	m_ctrlFindFilter.AddString("*.cpp;*.c");
	m_ctrlFindFilter.AddString("*.hpp;*.h");
	m_ctrlFindFilter.AddString("*.*");
	m_ctrlFindFilter.SetCurSel(0);

	((CAmboApp *)AfxGetApp())->InitialFindTextComboBox(&m_ctrlFindText);
	((CAmboApp *)AfxGetApp())->InitialFindFolderComboBox(&m_ctrlFindFolder);

	OnEditchangeFindfilter();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFindInFiles::OnFind() 
{
	// TODO: Add your control notification handler code here

		UpdateData();
		((CAmboApp *)AfxGetApp())->m_pRecentFindFolderList->Add(m_FindFolder);
		CDialog::OnOK();
}

void CFindInFiles::OnEditchangeFindfilter() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_strFindFilter.IsEmpty() || m_FindFolder.IsEmpty() || m_strFindText.IsEmpty())
	{
		m_ctrlFind.EnableWindow(FALSE);
	}
	else
	{
		m_ctrlFind.EnableWindow(TRUE);
	}
}

void CFindInFiles::OnEditchangeFindfolder() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_strFindFilter.IsEmpty() || m_FindFolder.IsEmpty() || m_strFindText.IsEmpty())
	{
		m_ctrlFind.EnableWindow(FALSE);
	}
	else
	{
		m_ctrlFind.EnableWindow(TRUE);
	}
	
}

void CFindInFiles::OnEditchangeFindtextcombo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_strFindFilter.IsEmpty() || m_FindFolder.IsEmpty() || m_strFindText.IsEmpty())
	{
		m_ctrlFind.EnableWindow(FALSE);
	}
	else
	{
		m_ctrlFind.EnableWindow(TRUE);
	}
	
}

void CFindInFiles::OnBrowse() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
//	Browse(m_strLocation);
	CString strNewRoot;
	if (Browse(strNewRoot) == TRUE)
	{
		m_FindFolder = strNewRoot;
		if (m_strFindFilter.IsEmpty() || m_FindFolder.IsEmpty() || m_strFindText.IsEmpty())
		{
			m_ctrlFind.EnableWindow(FALSE);
		}
		else
		{
			m_ctrlFind.EnableWindow(TRUE);
		}
	}
	UpdateData(FALSE);
	
}

BOOL CFindInFiles::Browse(CString &csPath)
{
	BROWSEINFO bi;
//	LPITEMIDLIST  iilOld;

//	SHGetSpecialFolderLocation(this->m_hWnd,CSIDL_COMMON_DESKTOPDIRECTORY ,&iilOld);

	bi.hwndOwner = this->m_hWnd; 
//	bi.pidlRoot = iilOld;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = NULL;
	bi.lpszTitle = TEXT("Browse ...");
	bi.ulFlags = BIF_RETURNONLYFSDIRS|BIF_EDITBOX ;             
	bi.lpfn = NULL;
	bi.lParam = NULL;

    LPITEMIDLIST iil = SHBrowseForFolder(&bi);

	if(iil)
	{
		TCHAR	*pBuf = new TCHAR[MAX_PATH];

		SHGetPathFromIDList (iil, pBuf);
		csPath = pBuf;
		delete[] pBuf;

		return TRUE;
	}
	csPath.Empty ();
	return FALSE;
}
