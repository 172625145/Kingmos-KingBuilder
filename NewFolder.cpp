// NewFolder.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "NewFolder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewFolder dialog


CNewFolder::CNewFolder(CWnd* pParent /*=NULL*/)
	: CDialog(CNewFolder::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewFolder)
	m_strNewFolderText = _T("");
	//}}AFX_DATA_INIT
}


void CNewFolder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewFolder)
	DDX_Control(pDX, IDC_NEWFOLDERTEXT, m_ctrlNewFolderText);
	DDX_Text(pDX, IDC_NEWFOLDERTEXT, m_strNewFolderText);
	DDV_MaxChars(pDX, m_strNewFolderText, 16);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewFolder, CDialog)
	//{{AFX_MSG_MAP(CNewFolder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewFolder message handlers

BOOL CNewFolder::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_ctrlNewFolderText.SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
