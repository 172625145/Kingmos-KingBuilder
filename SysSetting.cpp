// SysSetting.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "SysSetting.h"
#include "WorkSpaceInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysSetting dialog


CSysSetting::CSysSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CSysSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSysSetting)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iVersion = VERSION_B;
}


void CSysSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysSetting)
	DDX_Control(pDX, IDC_COMBO1, m_VersionSel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSysSetting, CDialog)
	//{{AFX_MSG_MAP(CSysSetting)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysSetting message handlers

void CSysSetting::SetVersion(int iVersion)
{
	//m_VersionSel.SetCurSel(iVersion);
	m_iVersion = iVersion;
}

int CSysSetting::GetVersion()
{
	return m_iVersion;
}

BOOL CSysSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_VersionSel.SetCurSel(m_iVersion);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSysSetting::OnOK() 
{
	// TODO: Add extra validation here
	
	m_iVersion = m_VersionSel.GetCurSel();
	
	CDialog::OnOK();
}
