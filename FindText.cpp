// FindText.cpp : implementation file
//

#include "stdafx.h"
#include "Ambo.h"
#include "specialView.h"
#include "FindText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFindText dialog


CFindText::CFindText(CWnd* pParent /*=NULL*/)
	: CDialog(CFindText::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindText)
	m_strText = _T("");
	m_bMatchAllWordOnly = FALSE;
	m_bMatchCase = FALSE;
	m_iDir = -1;
	//}}AFX_DATA_INIT
	nFindStyle = -1;
}


void CFindText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindText)
	DDX_Control(pDX, IDC_FIND_NEXT, m_FindNext);
	DDX_Control(pDX, IDC_FINDTEXTCOMBO, m_ComboBox);
	DDX_Control(pDX, IDC_MARKALL, m_MarkAll);
	DDX_CBString(pDX, IDC_FINDTEXTCOMBO, m_strText);
	DDX_Check(pDX, IDC_MATCHWHOLEWORD, m_bMatchAllWordOnly);
	DDX_Check(pDX, IDC_MATCH_CASE, m_bMatchCase);
	DDX_Radio(pDX, IDC_UP_DIR, m_iDir);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFindText, CDialog)
	//{{AFX_MSG_MAP(CFindText)
	ON_BN_CLICKED(IDC_FIND_NEXT, OnFindnext)
	ON_BN_CLICKED(IDC_MARKALL, OnMarkall)
	ON_CBN_EDITCHANGE(IDC_FINDTEXTCOMBO, OnEditchangeCombo1)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindText message handlers

void CFindText::OnFindnext() 
{
	// TODO: Add your control notification handler code here
	
	nFindStyle = FIND_FINDNEXT;
	CDialog::OnOK();
}

void CFindText::OnMarkall() 
{
	// TODO: Add your control notification handler code here
	
	nFindStyle = FIND_MARKALL;
	CDialog::OnOK();
}


void CFindText::OnEditchangeCombo1() 
{
	// TODO: Add your control notification handler code here

//	if (m_strText.GetLength() == 0)
	if (m_ComboBox.GetWindowTextLength() == 0)
	{
		m_FindNext.EnableWindow(FALSE);
		m_MarkAll.EnableWindow(FALSE);
	}
	else
	{
		m_FindNext.EnableWindow(TRUE);
		m_MarkAll.EnableWindow(TRUE);
	}
}

void CFindText::CheckEdit()
{
	if (m_ComboBox.GetWindowTextLength() == 0)
	{
		m_FindNext.EnableWindow(FALSE);
		m_MarkAll.EnableWindow(FALSE);
	}
	else
	{
		m_FindNext.EnableWindow(TRUE);
		m_MarkAll.EnableWindow(TRUE);
	}
}

int CFindText::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
//	CheckEdit();
//	dlg.m_ComboBox = pSearchInputComboBox;

	return 0;
}

void CFindText::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	CheckEdit();	
}

BOOL CFindText::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
/*	{
		CString rString;
		int nIndex = 0 ,nCount;

		nCount = m_pSearchComboBox->GetCount();
		while(nIndex < nCount)
		{
			m_pSearchComboBox->GetLBText(nIndex,rString);	
			m_ComboBox.AddString(rString);
			nIndex ++;
		}
		m_ComboBox.SetCurSel(m_pSearchComboBox->GetCurSel());
	}
*/
	((CAmboApp *)AfxGetApp())->InitialFindTextComboBox(&m_ComboBox);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
