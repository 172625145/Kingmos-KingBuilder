// ActiveSoftWare.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "ActiveSoftWare.h"
#include "MainFrame.h"
#include <atlbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActiveSoftWare dialog


CActiveSoftWare::CActiveSoftWare(CWnd* pParent /*=NULL*/)
	: CDialog(CActiveSoftWare::IDD, pParent)
{
	//{{AFX_DATA_INIT(CActiveSoftWare)
	m_strSerialNO = _T("");
	//}}AFX_DATA_INIT
}


void CActiveSoftWare::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActiveSoftWare)
	DDX_Control(pDX, IDC_EDIT1, m_ctrlSreialNO);
	DDX_Text(pDX, IDC_EDIT1, m_strSerialNO);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActiveSoftWare, CDialog)
	//{{AFX_MSG_MAP(CActiveSoftWare)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActiveSoftWare message handlers

void CActiveSoftWare::OnOK() 
{
	// TODO: Add extra validation here
	LPCTSTR lpKey;
	CString strKey;

	UpdateData(TRUE);
	if (m_strSerialNO.IsEmpty())
		return;
	strKey = m_strSerialNO;
	strKey.MakeUpper();
	CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

	if (pMainFrame == NULL)
		return;

	lpKey = (LPCTSTR)strKey;
	pMainFrame->m_workBar.m_uEnable = OBJECTNUM;
	while(*lpKey)
	{
		pMainFrame->m_workBar.m_uEnable -= *lpKey++;
	}
	if (pMainFrame->m_workBar.m_uEnable)
	{
		MessageBox("\"产品序列号\"不正确，请重新确认。");
		m_ctrlSreialNO.SetFocus();
		return;
	}
	CRegKey reg;
	
	LONG lKey = reg.Open(HKEY_LOCAL_MACHINE, TEXT("Software\\Kingmos\\kingbuilder\\1.00.000"));
	if (lKey == ERROR_SUCCESS)
	{
		reg.SetValue((LPCTSTR)strKey,TEXT("KeyNO"));
		reg.Close();
	}
	
	CDialog::OnOK();
}

BOOL CActiveSoftWare::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ctrlSreialNO.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
