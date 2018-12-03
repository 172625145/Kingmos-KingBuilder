// ReplaceText.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "ReplaceText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReplaceText dialog


CReplaceText::CReplaceText(CWnd* pParent /*=NULL*/)
	: CDialog(CReplaceText::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReplaceText)
	m_strSearch = _T("");
	m_bMatchAllWordOnly = FALSE;
	m_bMatchCase = FALSE;
	m_strReplaceText = _T("");
	//}}AFX_DATA_INIT
	m_bCanReplace = FALSE;
}


void CReplaceText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReplaceText)
	DDX_Control(pDX, IDC_REPLACETEXTCOMBO, m_ctrlReplaceText);
	DDX_Control(pDX, IDC_FINDTEXTCOMBO, m_ctrlFindText);
	DDX_Control(pDX, IDC_REPLACEALL, m_ctrlReplaceAll);
	DDX_Control(pDX, IDC_REPLACE, m_ctrlReplace);
	DDX_Control(pDX, IDC_FIND_NEXT, m_ctrlFindNext);
	DDX_CBString(pDX, IDC_FINDTEXTCOMBO, m_strSearch);
	DDX_Check(pDX, IDC_MATCHWHOLEWORD, m_bMatchAllWordOnly);
	DDX_Check(pDX, IDC_MATCH_CASE, m_bMatchCase);
	DDX_CBString(pDX, IDC_REPLACETEXTCOMBO, m_strReplaceText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReplaceText, CDialog)
	//{{AFX_MSG_MAP(CReplaceText)
	ON_BN_CLICKED(IDC_FIND_NEXT, OnFindNext)
	ON_CBN_EDITCHANGE(IDC_FINDTEXTCOMBO, OnEditchangeFindtextcombo)
	ON_BN_CLICKED(IDC_REPLACE, OnReplace)
	ON_BN_CLICKED(IDC_REPLACEALL, OnReplaceall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReplaceText message handlers

void CReplaceText::OnFindNext() 
{
	// TODO: Add your control notification handler code here
	if (m_pEdit->SearchString(CPoint(-1,-1),m_strSearch,m_bMatchAllWordOnly, m_bMatchCase,DIRECTION_DOWN) == TRUE)
	{
		m_bCanReplace = TRUE;
	}
	else
	{
		m_bCanReplace = FALSE;
	}
}

void CReplaceText::OnEditchangeFindtextcombo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_strSearch.IsEmpty())
	{
		m_ctrlFindNext.EnableWindow(FALSE);
		m_ctrlReplace.EnableWindow(FALSE);
		m_ctrlReplaceAll.EnableWindow(FALSE);
	}
	else
	{
		m_ctrlFindNext.EnableWindow(TRUE);
		m_ctrlReplace.EnableWindow(TRUE);
		m_ctrlReplaceAll.EnableWindow(TRUE);
	}
	m_bCanReplace = FALSE;
}

void CReplaceText::OnReplace() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	InsertReplaceTextCombo(m_strReplaceText);
	if (m_bCanReplace == FALSE)
	{
		OnFindNext();
		return;
	}
	if (m_bCanReplace == TRUE)
	{
		m_pEdit->OnReplace(m_strReplaceText);
		OnFindNext();
	}
}

void CReplaceText::OnReplaceall() 
{
	// TODO: Add your control notification handler code here
	InsertReplaceTextCombo(m_strReplaceText);
	CDialog::OnOK();	
}

BOOL CReplaceText::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	((CAmboApp *)AfxGetApp())->InitialFindTextComboBox(&m_ctrlFindText);
	((CAmboApp *)AfxGetApp())->InitialReplaceTextComboBox(&m_ctrlReplaceText);
	
	OnEditchangeFindtextcombo();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CReplaceText::InsertReplaceTextCombo(CString strReplaceText)
{

	if (strReplaceText.IsEmpty())
		return ;

	if (m_ctrlReplaceText.SelectString(0, strReplaceText) == CB_ERR )
	{
		int iIndex = m_ctrlReplaceText.InsertString(0,strReplaceText);
		m_ctrlReplaceText.SetCurSel(iIndex);
	}
	((CAmboApp *)AfxGetApp())->SetReplaceText(strReplaceText);

}
